"""导出 ATK Component 的完整 Python 接口，并尝试载入比赛场景，生成报告。

用法（仓库根目录，Windows）：
    <python> tools/dump_api.py --scenario "<测试.atk 的完整路径>"
ATK 目录读环境变量 ATK_HOME。

输出（不修改 ATK 安装目录）：
    report/env/vendor_samples/component/Python/ATKComponentPythonModule.py   SWIG 接口定义（全部类和方法）
    report/env/vendor_samples/examples/*.java                                Java 示例（含 AccessTest.java）
    scenes/<场景文件名>                                                    比赛场景副本
    report/env/api_report.md                                                 根对象方法 + 场景载入结果 + 对象树
    report/atk_output/                                                   ATK 运行时输出目录（不提交）
"""

import argparse
import inspect
import os
import shutil
import sys
import traceback
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
REPORT = REPO / "report" / "env" / "api_report.md"
OUTDIR = REPO / "report" / "env" / "atk_output"
out = []


def log(s=""):
    print(s, flush=True)
    out.append(str(s))
    REPORT.write_text("\n".join(out), encoding="utf-8")  # 每行都落盘，进程崩溃也能留下已有内容


def call0(obj, *names):
    """依次尝试无参方法，返回第一个成功的结果。"""
    for n in names:
        f = getattr(obj, n, None)
        if callable(f):
            try:
                return f()
            except Exception:
                pass
    return None


def methods(obj):
    return [m for m in dir(obj) if not m.startswith("_") and callable(getattr(obj, m, None))]


def walk(obj, depth=0, max_depth=4):
    name = call0(obj, "GetInstanceName", "GetName", "GetPath") or "?"
    cls = type(obj).__name__
    log("  " * depth + f"- {name}  [{cls}]")
    if depth >= max_depth:
        return
    kids = call0(obj, "GetChildren")
    if kids is None:
        return
    n = call0(kids, "GetCount", "Count", "size", "__len__")
    if not isinstance(n, int):
        return
    for i in range(n):
        try:
            walk(kids.Item(i), depth + 1, max_depth)
        except Exception as e:
            log("  " * (depth + 1) + f"- Item({i}) 失败：{e}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--atk-root", default=os.environ.get("ATK_HOME"))
    ap.add_argument("--scenario", help="比赛场景 .atk 文件路径，比如 MMC10 文件夹里的 测试.atk")
    a = ap.parse_args()
    if not a.atk_root:
        sys.exit("请设置环境变量 ATK_HOME")
    root_dir = Path(a.atk_root)
    comp = root_dir / "IntegratingWithATK" / "component" / "Python"
    REPORT.parent.mkdir(exist_ok=True)
    OUTDIR.mkdir(parents=True, exist_ok=True)

    log("# ATK 接口与场景报告")
    log()

    # 1. 复制接口定义和 Java 示例
    log("## 1. 复制的文件")
    dst = REPO / "report" / "env" / "vendor_samples" / "component" / "Python"
    dst.mkdir(parents=True, exist_ok=True)
    src = comp / "ATKComponentPythonModule.py"
    if src.exists():
        shutil.copy2(src, dst / src.name)
        log(f"- {src.name}（{src.stat().st_size} 字节）")
    ex_dst = REPO / "report" / "env" / "vendor_samples" / "examples"
    ex_dst.mkdir(parents=True, exist_ok=True)
    for d in (root_dir / "Help").rglob("*"):
        if d.is_dir() and "二次开发" in d.name:
            for j in d.glob("*.java"):
                shutil.copy2(j, ex_dst / j.name)
                log(f"- {j.name}")
    if a.scenario:
        sp = Path(a.scenario)
        if sp.exists():
            (REPO / "scenes").mkdir(exist_ok=True)
            shutil.copy2(sp, REPO / "scenes" / sp.name)
            log(f"- 场景副本 scenes/{sp.name}（{sp.stat().st_size} 字节）")
        else:
            log(f"- 场景文件不存在：{sp}")
    log()

    # 2. 初始化 Component
    log("## 2. 初始化")
    for d in (root_dir, comp):
        os.add_dll_directory(str(d))
    os.environ["PATH"] = f"{comp}{os.pathsep}{root_dir}{os.pathsep}{os.environ.get('PATH', '')}"
    sys.path.insert(0, str(comp))
    os.chdir(root_dir)
    import ATKComponentPythonModule as atk

    atk.SetSaveFileBasePath(str(OUTDIR))
    atk.SetCallCodeType("python")
    root = atk.IAtkObjectRoot()
    log(f"- 输出目录设为 {OUTDIR}")
    funcs = [n for n in dir(atk) if not n.startswith(("_", "e")) and callable(getattr(atk, n))]
    log(f"- 模块级函数和类（非枚举）共 {len(funcs)} 个：")
    log("  " + ", ".join(funcs))
    log()

    log("## 3. IAtkObjectRoot 的方法")
    for m in methods(root):
        doc = (inspect.getdoc(getattr(root, m)) or "").splitlines()
        log(f"- `{m}` {doc[0] if doc else ''}")
    log()

    # 3. 载入场景
    log("## 4. 载入场景")
    if not a.scenario:
        log("未提供 --scenario，跳过")
    else:
        sp = str(Path(a.scenario).resolve())
        loaders = [m for m in methods(root) if any(k in m for k in ("Load", "Open"))]
        log(f"候选载入方法：{loaders}")
        loaded = False
        for m in loaders:
            try:
                r = getattr(root, m)(sp)
                log(f"- `{m}(path)` 返回 {r!r}")
                loaded = True
                break
            except Exception as e:
                log(f"- `{m}(path)` 失败：{type(e).__name__}: {e}")
        if loaded:
            log()
            log("### 对象树")
            scen = call0(root, "GetCurrentScenario", "GetScenario", "CurrentScenario")
            walk(scen if scen is not None else root)
            for name in ("GetCurrentScenario", "GetScenario"):
                if scen is not None:
                    log()
                    log(f"### 场景对象方法（{type(scen).__name__}）")
                    log(", ".join(methods(scen)))
                    break
    log()
    log("完成")


if __name__ == "__main__":
    try:
        main()
    except Exception:
        log("```")
        log(traceback.format_exc())
        log("```")
        raise
