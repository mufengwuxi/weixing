"""把 数据.xlsx 中各卫星的初始状态导出成全精度 CSV，便于在 GitHub 上查看和比较版本差异。

用法：python tools/dump_data.py data/数据.xlsx
输出：data/数据_全精度.csv（每颗卫星一行，数值用 repr 输出，不做四舍五入）
      控制台另外列出每个单元格是数字还是文本、有效数字位数，方便核对精度
"""

import csv
import sys
from pathlib import Path

import openpyxl

REPO = Path(__file__).resolve().parents[1]


def main():
    if len(sys.argv) < 2:
        sys.exit("用法：python tools/dump_data.py data/数据.xlsx")
    ws = openpyxl.load_workbook(sys.argv[1], data_only=True).worksheets[0]
    out = REPO / "data" / "数据_全精度.csv"
    rows, notes = [], []
    for r in range(4, 36):
        name = ws.cell(row=r, column=1).value
        vals = [ws.cell(row=r, column=c).value for c in range(2, 8)]
        rows.append([r, name] + [repr(float(v)) if isinstance(v, (int, float)) else ("" if v is None else str(v)) for v in vals])
        kinds = {type(v).__name__ for v in vals}
        digits = max((len(repr(float(v)).replace("-", "").replace(".", "").lstrip("0")) for v in vals if isinstance(v, (int, float))), default=0)
        notes.append(f"第 {r} 行 {name}: 单元格类型 {sorted(kinds)}，最多 {digits} 位有效数字")
    with open(out, "w", newline="", encoding="utf-8-sig") as f:
        w = csv.writer(f)
        w.writerow(["行", "名称", "x_km", "y_km", "z_km", "vx_km_s", "vy_km_s", "vz_km_s"])
        w.writerows(rows)
    print("\n".join(notes))
    print(f"已写入 {out.relative_to(REPO)}")


if __name__ == "__main__":
    main()
