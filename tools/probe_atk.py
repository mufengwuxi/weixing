"""探测本机 ATK 的三种联动接口（Connect / Component / CLI），生成报告。

用法（在仓库根目录，Windows 上）：
    python tools/probe_atk.py --atk-root "D:\\ATK\\ATK4\\ATK-4.2.0-rc.2"

不传 --atk-root 时读环境变量 ATK_HOME。
只用 Python 标准库，不改动 ATK 安装目录里的任何文件。

输出：
    report/probe_report.md        探测结论
    report/vendor_samples/        包里示例脚本和配置的只读副本（供远端写代码参考）
跑完后把 report/ 提交推送即可。
"""

import argparse
import os
import shutil
import socket
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
REPORT_DIR = REPO / "report"
SAMPLES_DIR = REPORT_DIR / "vendor_samples"

TEXT_EXT = {".py", ".m", ".ini", ".txt", ".md", ".json", ".xml", ".bat", ".cmd", ".cs", ".cpp", ".h"}
MAX_SAMPLE_BYTES = 200_000
CLI_NAMES = ["ATKConsole.exe", "AstCLI.exe", "ATKServerConsole.exe", "UiATKServer.exe"]

lines = []


def log(s=""):
    print(s)
    lines.append(s)


def find_files(root, names):
    found = {}
    lower = {n.lower(): n for n in names}
    for p in root.rglob("*"):
        if p.is_file() and p.name.lower() in lower:
            found.setdefault(lower[p.name.lower()], []).append(p)
    return found


def tree(root, max_depth=4):
    out = []
    root_depth = len(root.parts)
    for dirpath, dirnames, filenames in os.walk(root):
        d = Path(dirpath)
        depth = len(d.parts) - root_depth
        if depth > max_depth:
            dirnames[:] = []
            continue
        out.append("  " * depth + d.name + "/")
        for f in sorted(filenames)[:40]:
            out.append("  " * (depth + 1) + f)
        if len(filenames) > 40:
            out.append("  " * (depth + 1) + f"... 还有 {len(filenames) - 40} 个文件")
    return out


def copy_samples(integ):
    n = 0
    for p in integ.rglob("*"):
        if p.is_file() and p.suffix.lower() in TEXT_EXT and p.stat().st_size <= MAX_SAMPLE_BYTES:
            dst = SAMPLES_DIR / p.relative_to(integ)
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, dst)
            n += 1
    return n


def try_port(host, port, timeout=2.0):
    try:
        with socket.create_connection((host, port), timeout=timeout):
            return True, "可以连接"
    except OSError as e:
        return False, f"连接失败：{e}"


def run_cmd(args, cwd, timeout=20):
    try:
        r = subprocess.run(args, cwd=cwd, capture_output=True, timeout=timeout)
        out = (r.stdout + r.stderr).decode("gbk", errors="replace").strip()
        return f"返回码 {r.returncode}\n{out[:3000]}"
    except subprocess.TimeoutExpired:
        return f"{timeout} 秒内没有退出（可能在等待输入或已作为服务运行），已终止"
    except OSError as e:
        return f"无法启动：{e}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--atk-root", default=os.environ.get("ATK_HOME"))
    ap.add_argument("--port", type=int, default=8080)
    a = ap.parse_args()

    if not a.atk_root:
        sys.exit("请用 --atk-root 指定 ATK 目录，或设置环境变量 ATK_HOME")
    root = Path(a.atk_root)
    if not root.exists():
        sys.exit(f"目录不存在：{root}")

    REPORT_DIR.mkdir(exist_ok=True)
    if SAMPLES_DIR.exists():
        shutil.rmtree(SAMPLES_DIR)
    SAMPLES_DIR.mkdir(parents=True)

    log("# ATK 接口探测报告")
    log()
    log(f"- Python：{sys.version.split()[0]}（{sys.executable}）")
    log(f"- ATK 目录：{root}")
    log()

    log("## 1. 目录结构")
    log("```")
    integ = next((p for p in root.rglob("IntegratingWithATK") if p.is_dir()), None)
    for s in tree(root, max_depth=1):
        log(s)
    if integ:
        log("")
        for s in tree(integ, max_depth=4):
            log(s)
    log("```")
    if integ:
        log(f"已复制 {copy_samples(integ)} 个示例文本文件到 report/vendor_samples/")
    else:
        log("没找到 IntegratingWithATK 目录")
    log()

    log("## 2. server_config.ini")
    for cfg in root.rglob("server_config.ini"):
        log(f"`{cfg.relative_to(root)}`")
        log("```ini")
        log(cfg.read_text(encoding="utf-8", errors="replace")[:3000])
        log("```")
        shutil.copy2(cfg, SAMPLES_DIR / "server_config.ini")
    log()

    log("## 3. Connect：TCP 端口")
    ok, msg = try_port("127.0.0.1", a.port)
    log(f"- 127.0.0.1:{a.port} {msg}")
    log("- 如果失败，请先打开 ATK 界面（或 UiATKServer.exe）再重跑本脚本，对比结果")
    log()

    log("## 4. CLI：命令行程序")
    found = find_files(root, CLI_NAMES)
    for name in CLI_NAMES:
        for p in found.get(name, []):
            log(f"### {p.relative_to(root)}")
            if name in ("ATKConsole.exe", "AstCLI.exe"):
                for flag in ("--help", "-h", "/?"):
                    log(f"`{name} {flag}`")
                    log("```")
                    log(run_cmd([str(p), flag], cwd=p.parent))
                    log("```")
            else:
                log("（服务程序，不自动启动）")
    if not found:
        log("没找到任何命令行程序")
    log()

    log("## 5. Component：导入 Python 模块")
    comps = list(root.rglob("ATKComponentPythonModule.py"))
    for mod in comps:
        log(f"`{mod.relative_to(root)}`")
        code = (
            "import sys; sys.path.insert(0, r'%s'); import ATKComponentPythonModule as m; "
            "print([x for x in dir(m) if not x.startswith('_')])" % mod.parent
        )
        log("```")
        log(run_cmd([sys.executable, "-c", code], cwd=mod.parent, timeout=60))
        log("```")
    if not comps:
        log("没找到 ATKComponentPythonModule.py")
    log()

    (REPORT_DIR / "probe_report.md").write_text("\n".join(lines), encoding="utf-8")
    print(f"\n报告已写入 {REPORT_DIR / 'probe_report.md'}")


if __name__ == "__main__":
    main()
