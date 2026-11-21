"""验证 ATK Component（Python 进程内调用）能否跑通，生成报告。

用法（仓库根目录，Windows）：
    <python> tools/try_component.py
ATK 目录读环境变量 ATK_HOME，也可以用 --atk-root 指定。

做的事情（不修改 ATK 安装目录里的任何文件）：
1. 把 Help/Examples/08-二次开发案例 里的示例脚本复制到 report/env/vendor_samples/examples/
2. 检查 _ATKComponentPythonModule.pyd 依赖哪个版本的 Python
3. 分别用 ATK 自带的 Python 和当前 Python，加上 DLL 搜索路径后尝试导入 Component 模块
4. 用第一个导入成功的解释器运行官方示例 ATKComponentPythonTest.py
结果写到 report/env/component_report.md
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
REPORT = REPO / "report" / "env" / "component_report.md"
EXAMPLES_OUT = REPO / "report" / "env" / "vendor_samples" / "examples"
TEXT_EXT = {".py", ".m", ".txt", ".md", ".ini", ".json", ".atks", ".bat"}

# 子进程里执行的代码：设置 DLL 搜索路径后导入模块，可选再运行一个示例脚本
CHILD = r"""
import os, sys, json, runpy
root = os.environ["ATK_ROOT"]; comp = os.environ["ATK_COMP"]
for d in (root, comp):
    os.add_dll_directory(d)
os.environ["PATH"] = comp + os.pathsep + root + os.pathsep + os.environ.get("PATH", "")
sys.path.insert(0, comp)
os.chdir(root)
import ATKComponentPythonModule as m
print("JSON:" + json.dumps({"python": sys.version, "names": [x for x in dir(m) if not x.startswith("_")]}))
sys.stdout.flush()
ex = os.environ.get("ATK_EXAMPLE")
if ex:
    sys.path.insert(0, os.path.dirname(ex))
    print("===== 运行示例 " + ex + " =====")
    runpy.run_path(ex, run_name="__main__")
"""

out = []


def log(s=""):
    print(s)
    out.append(s)


def run_child(py, root, comp, example=None, timeout=300):
    env = dict(os.environ, ATK_ROOT=str(root), ATK_COMP=str(comp))
    if example:
        env["ATK_EXAMPLE"] = str(example)
    try:
        r = subprocess.run([str(py), "-c", CHILD], capture_output=True, timeout=timeout, env=env, cwd=str(root))
        text = (r.stdout + r.stderr).decode("utf-8", errors="replace")
        if "�" in text:
            text = (r.stdout + r.stderr).decode("gbk", errors="replace")
        return r.returncode, text.strip()
    except subprocess.TimeoutExpired as e:
        partial = ((e.stdout or b"") + (e.stderr or b"")).decode("utf-8", errors="replace")
        return None, f"{timeout} 秒超时，已终止。已有输出：\n{partial[-4000:]}"
    except OSError as e:
        return None, f"无法启动：{e}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--atk-root", default=os.environ.get("ATK_HOME"))
    a = ap.parse_args()
    if not a.atk_root:
        sys.exit("请用 --atk-root 指定 ATK 目录，或设置环境变量 ATK_HOME")
    root = Path(a.atk_root)
    comp = root / "IntegratingWithATK" / "component" / "Python"

    log("# Component 验证报告")
    log()
    log(f"- ATK 目录：{root}")
    log(f"- 当前 Python：{sys.version.split()[0]}（{sys.executable}）")
    log()

    # 1. 复制官方二次开发示例
    log("## 1. 官方二次开发示例")
    ex_dirs = [p for p in (root / "Help").rglob("*") if p.is_dir() and "二次开发" in p.name]
    example_component = None
    for d in ex_dirs:
        log(f"`{d.relative_to(root)}`")
        log("```")
        for p in sorted(d.rglob("*")):
            if p.is_file():
                log(f"{p.relative_to(d)}  ({p.stat().st_size} 字节)")
                if p.suffix.lower() in TEXT_EXT and p.stat().st_size < 300_000:
                    dst = EXAMPLES_OUT / p.relative_to(d)
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(p, dst)
                if p.name == "ATKComponentPythonTest.py":
                    example_component = p
        log("```")
    if not ex_dirs:
        log("没找到名字含“二次开发”的示例目录")
    log()

    # 2. pyd 依赖的 Python 版本
    log("## 2. pyd 依赖的 Python 版本")
    for pyd in comp.glob("*.pyd"):
        deps = sorted({m.decode().lower() for m in re.findall(rb"python3\d+\.dll", pyd.read_bytes(), re.I)})
        log(f"- `{pyd.name}` 依赖：{', '.join(deps) or '没扫到 python3xx.dll 字样'}")
    log()

    # 3. 逐个解释器尝试导入
    log("## 3. 导入尝试")
    candidates = []
    for c in [root / "Python" / "_internal" / "python.exe", root / "Python" / "python.exe"]:
        if c.exists():
            candidates.append(c)
    candidates += [p for p in (root / "Python").rglob("python.exe") if p not in candidates][:2]
    candidates.append(Path(sys.executable))
    working = None
    for py in candidates:
        code, text = run_child(py, root, comp, timeout=120)
        ok = code == 0 and "JSON:" in text
        log(f"### `{py}` → {'成功' if ok else '失败'}")
        log("```")
        log(text[-4000:])
        log("```")
        if ok and working is None:
            working = py
    log()

    # 4. 运行官方示例
    log("## 4. 运行官方示例 ATKComponentPythonTest.py")
    if working and example_component:
        code, text = run_child(working, root, comp, example=example_component, timeout=600)
        log(f"解释器 `{working}`，返回码 {code}")
        log("```")
        log(text[-15000:])
        log("```")
    else:
        log(f"跳过：可用解释器={working}，示例={example_component}")

    REPORT.parent.mkdir(exist_ok=True)
    REPORT.write_text("\n".join(out), encoding="utf-8")
    print(f"\n报告已写入 {REPORT}")


if __name__ == "__main__":
    main()
