"""检查 数据.xlsx：列出所有卫星的轨道根数，并把 r1 和第一问方案里用过的 r1 对比。

用法：python tools/check_data.py data/数据.xlsx
输出：report/tasks/data_check.md
"""

import json
import math
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "tools"))
from verify_p1_atk import elements, norm, sub  # noqa: E402

MU = 398600.4418
W_E = 7.2921159e-5


def main():
    if len(sys.argv) < 2:
        sys.exit("用法：python tools/check_data.py data/数据.xlsx")
    import openpyxl
    ws = openpyxl.load_workbook(sys.argv[1], data_only=True).worksheets[0]
    out = ["# 数据检查", "", f"文件：`{sys.argv[1]}`", ""]
    out += ["| 行 | 名称 | a (km) | e | i (°) | 方位角 (°) | 近地点地心距 (km) | 水平速度与近圆速度夹角 (°) |",
            "|---|---|---|---|---|---|---|---|"]
    states = {}
    for r in range(4, 36):
        name = ws.cell(row=r, column=1).value
        vals = [ws.cell(row=r, column=c).value for c in range(2, 8)]
        if name is None or any(v is None for v in vals):
            out.append(f"| {r} | {name} | 空 | | | | | |")
            continue
        x = [float(str(v).strip()) for v in vals]
        if math.sqrt(x[0] ** 2 + x[1] ** 2 + x[2] ** 2) > 1e6:  # 高精度版数据单位为 m、m/s
            x = [t / 1000.0 for t in x]
        states[str(name).strip()] = x
        el = elements(x[:3], x[3:], MU)
        circ = [-W_E * x[1], W_E * x[0]]
        c = (x[3] * circ[0] + x[4] * circ[1]) / (math.hypot(x[3], x[4]) * math.hypot(*circ))
        ang = math.degrees(math.acos(max(-1, min(1, c))))
        flag = " ⚠" if el["e"] > 0.01 or el["rp_km"] < 6378.137 else ""
        out.append(f"| {r} | {name}{flag} | {el['a_km']:.3f} | {el['e']:.6f} | {el['i_deg']:.4f} | "
                   f"{math.degrees(math.atan2(x[1], x[0])):.2f} | {el['rp_km']:.1f} | {ang:.2f} |")
    out.append("")
    out.append("⚠ 表示偏心率大于 0.01 或近地点在地球内部。")
    out.append("")

    old = REPO / "solutions" / "q1" / "p1_doc.json"
    if "r1" in states and old.exists():
        o = json.loads(old.read_text(encoding="utf-8"))
        r1 = states["r1"]
        out.append("## r1 与第一问方案用过的数据对比")
        for key, label in (("r1_original_km", "旧 数据.xlsx 原始值"), ("r1_used_km", "第一问方案实际使用值（zzx 修正）")):
            v = o[key]
            out.append(f"- 与{label}：位置差 {norm(sub(r1[:3], v[:3])):.6f} km，速度差 {norm(sub(r1[3:], v[3:])) * 1000:.6f} m/s")
        dp = norm(sub(r1[:3], o["r1_used_km"][:3]))
        dvv = norm(sub(r1[3:], o["r1_used_km"][3:]))
        out.append("")
        if dp < 1e-6 and dvv < 1e-9:
            out.append("**结论：新 r1 与第一问方案使用的 r1 完全一致，现有机动方案可以直接复核。**")
        elif dp < 1e-3 and dvv < 1e-6:
            out.append("**结论：新 r1 与第一问方案使用的 r1 只差旧数据的舍入误差（米级以下），轨道相同；正式结果应以新数据重新求解。**")
        else:
            out.append("**结论：新 r1 与第一问方案使用的 r1 不一致，现有机动方案是按旧数据求的，不能直接套用，第一问需要用新数据重新求解。**")
    if "b1" in states and old.exists():
        o = json.loads(old.read_text(encoding="utf-8"))
        out.append(f"- b1 与旧数据位置差 {norm(sub(states['b1'][:3], o['b1_km'][:3])):.6f} km")

    p = REPO / "report" / "tasks" / "data_check.md"
    p.write_text("\n".join(out), encoding="utf-8")
    print("\n".join(out))


if __name__ == "__main__":
    main()
