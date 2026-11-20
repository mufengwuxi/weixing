"""从 数据.xlsx 和 docs/第一问总结.md 里的数值生成第一问方案文件。

用于在拿不到求解代码时先做复核。文档里 Δv 只有 4 位小数（m/s）、时刻只到毫秒，
所以这个文件代表“按文档能复现到什么程度”，不是求解器的全精度结果。

用法（需要 openpyxl：python -m pip install openpyxl）：
    python tools/make_p1_from_doc.py data/数据.xlsx
输出：solutions/p1_doc.json
"""

import json
import sys
from datetime import datetime
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
EPOCH = datetime(2027, 1, 1)

# docs/第一问总结.md 第 5.1 节
DOC = {
    "maneuvers": [
        ("2027-01-01 14:45:24.673", [-4.0240, +0.6984, +0.3073]),
        ("2027-01-02 04:18:59.840", [-1.0659, -0.3258, +0.0745]),
    ],
    "obs_start": "2027-01-02 14:25:46.634",
    "claimed": {"total_dv_m_s": 5.2127, "d_min_km": 10.2750, "d_max_km": 14.0474,
                "alpha_max_deg": 44.9999, "theta_max_deg": 44.9999},
}
# 文档说明的 r1 修正规则：X、Y、Vy、Vz 取负
FLIP = [-1, -1, 1, 1, -1, -1]


def sec(s):
    return (datetime.strptime(s, "%Y-%m-%d %H:%M:%S.%f") - EPOCH).total_seconds()


def row(ws, r):
    vals = [ws.cell(row=r, column=c).value for c in range(2, 8)]  # B..G
    if any(v is None for v in vals):
        raise ValueError(f"第 {r} 行 B-G 列有空值：{vals}")
    return [float(v) for v in vals]


def main():
    if len(sys.argv) < 2:
        sys.exit("用法：python tools/make_p1_from_doc.py data/数据.xlsx")
    import openpyxl
    wb = openpyxl.load_workbook(sys.argv[1], data_only=True)
    ws = wb.worksheets[0]
    print("Sheet1 前 8 行 A-G 列（核对行号）：")
    for r in range(1, 9):
        print(r, [ws.cell(row=r, column=c).value for c in range(1, 8)])
    r1 = row(ws, 4)
    b1 = row(ws, 6)
    sol = {
        "epoch_utc": "2027-01-01T00:00:00",
        "mu_km3_s2": 398600.4418,
        "r1_original_km": r1,
        "r1_used_km": [f * x for f, x in zip(FLIP, r1)],
        "b1_km": b1,
        "maneuvers": [{"t_s": sec(t), "dv_km_s": [x / 1000 for x in dv]} for t, dv in DOC["maneuvers"]],
        "obs_start_s": sec(DOC["obs_start"]),
        "obs_duration_s": 60,
        "claimed": DOC["claimed"],
        "note": "由 tools/make_p1_from_doc.py 根据 第一问总结.md 的表格数值生成（Δv 4 位小数、时刻毫秒），不是求解器全精度输出",
    }
    out = REPO / "solutions" / "p1_doc.json"
    out.write_text(json.dumps(sol, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"已写入 {out}")


if __name__ == "__main__":
    main()
