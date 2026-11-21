"""统一读取 数据.xlsx 中的卫星初始状态，返回千米、千米/秒。

组委会的数据先后出现过两种写法：旧版单位是 km、km/s，单元格存为数字；
2026-11-20 的高精度版单位是 m、m/s，单元格存为文本（20 位有效数字）。
这里按位置矢量的量级自动判断单位：|r| > 1e6 视为米制，整体除以 1000。
"""
from pathlib import Path

import numpy as np

DEFAULT = Path(__file__).resolve().parents[1] / "data" / "数据.xlsx"


def read_states(xlsx=DEFAULT):
    """返回 (R1, TG)：R1 为 r1 的状态，TG 为 {目标名: 状态}，均为 km、km/s。"""
    import openpyxl
    ws = openpyxl.load_workbook(str(xlsx), data_only=True).worksheets[0]
    out = {}
    for r in range(4, 36):
        name = ws.cell(row=r, column=1).value
        vals = [ws.cell(row=r, column=c).value for c in range(2, 8)]
        if name is None or any(v is None or str(v).strip() == "" for v in vals):
            continue
        x = np.array([float(str(v).strip()) for v in vals])
        if np.linalg.norm(x[:3]) > 1e6:  # 米制
            x = x / 1000.0
        out[str(name).strip()] = x
    R1 = out.pop("r1")
    out.pop("r2", None)
    return R1, out


def write_states_json(path, xlsx=DEFAULT):
    """生成求解器用的 states.json（km、km/s，全精度浮点）。"""
    import json
    R1, TG = read_states(xlsx)
    json.dump({"r1": R1.tolist(), "targets": {k: v.tolist() for k, v in TG.items()}}, open(path, "w"))


if __name__ == "__main__":
    # python code/datafile.py  ->  重新生成 code/p3/v3/states.json（v1、v2 保留旧数据的 states.json，便于复现）
    root = Path(__file__).resolve().parent
    for p in (root / "p3" / "v3" / "states.json",):
        write_states_json(p)
        print("已写入", p.relative_to(root.parent))
