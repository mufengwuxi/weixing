"""按参数表批量调用 ATK，结果写进 results/<运行名>/。

用法（仓库根目录，Windows，先设置 ATK_HOME）：
    python run.py params/example.csv              # 所有任务在同一个进程里依次跑
    python run.py params/example.csv --isolate    # 每个任务单独起一个进程，最稳，慢一点
    python run.py params/example.csv --only 2     # 只跑 job_id 为 2 的任务

参数表每行一个任务，列含义见 params/README.md。
输出：
    results/<运行名>/summary.csv        每个任务一行：状态、窗口数、总时长、轨道自检、报错
    results/<运行名>/access_<id>.csv    每个任务的可见窗口明细
    results/<运行名>/raw/               ATK 原始输出（报告、场景），不提交
"""

import argparse
import csv
import json
import math
import os
import subprocess
import sys
import time
import traceback
from datetime import datetime
from pathlib import Path

REPO = Path(__file__).resolve().parent
sys.path.insert(0, str(REPO / "code"))

ORBIT_COLS = ["epoch", "sma_m", "ecc", "inc_deg", "raan_deg", "argp_deg", "ta_deg"]
SUMMARY_COLS = ["job_id", "status", "propagator", "n_windows", "total_access_s", "first_start", "last_stop",
                "r_expected_m", "r_actual_m", "report_file", "elapsed_s", "error"]


def read_jobs(path):
    with open(path, encoding="utf-8-sig", newline="") as f:
        return [{k.strip(): (v or "").strip() for k, v in r.items() if k} for r in csv.DictReader(f)
                if any((v or "").strip() for v in r.values()) and not (r.get("job_id") or "").startswith("#")]


def run_job(atk, job, run_dir):
    """执行一个任务，返回 summary 行。"""
    t0 = time.time()
    s = {c: "" for c in SUMMARY_COLS}
    s["job_id"] = job["job_id"]
    try:
        atk.load(REPO / job["scenario"] if not os.path.isabs(job["scenario"]) else job["scenario"])
        sat = job.get("satellite", "")
        if sat:
            s["propagator"] = atk.propagator_type(sat)
        if sat and all(job.get(c) for c in ORBIT_COLS):
            v = {c: (job[c] if c == "epoch" else float(job[c])) for c in ORBIT_COLS}
            if v["epoch"].lower() == "start":  # 用场景开始时刻作为历元
                v["epoch"] = atk.time_period()[0]
            atk.set_orbit(sat, **v)
            # 轨道自检：初始时刻的地心距应等于 a(1-e^2)/(1+e cos f)，用来确认单位（m、度）理解正确
            e, f = v["ecc"], math.radians(v["ta_deg"])
            s["r_expected_m"] = round(v["sma_m"] * (1 - e * e) / (1 + e * math.cos(f)), 1)
            try:
                pos, _ = atk.position_j2000(sat, v["epoch"])
                s["r_actual_m"] = round(math.sqrt(sum(x * x for x in pos)), 1)
            except Exception as ex:  # 自检失败不影响主流程
                s["r_actual_m"] = f"自检失败: {ex}"
        if sat and job.get("access_to"):
            wins = atk.access(sat, job["access_to"], job.get("start") or None, job.get("stop") or None,
                              float(job.get("step_s") or 60))
            with open(run_dir / f"access_{job['job_id']}.csv", "w", encoding="utf-8-sig", newline="") as f:
                w = csv.DictWriter(f, fieldnames=["start", "stop", "duration_s", "raw"])
                w.writeheader()
                w.writerows(wins)
            s["n_windows"] = len(wins)
            durs = [x["duration_s"] for x in wins if isinstance(x["duration_s"], float)]
            s["total_access_s"] = round(sum(durs), 3) if durs else ""
            if wins:
                s["first_start"], s["last_stop"] = wins[0]["start"], wins[-1]["stop"]
        if sat and job.get("report"):
            s["report_file"] = atk.report(sat, job["report"], job.get("start") or None, job.get("stop") or None)
        s["status"] = "ok"
    except Exception as ex:
        s["status"] = "error"
        s["error"] = f"{type(ex).__name__}: {ex}"
        (run_dir / f"error_{job['job_id']}.txt").write_text(traceback.format_exc(), encoding="utf-8")
    s["elapsed_s"] = round(time.time() - t0, 2)
    return s


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("params")
    ap.add_argument("--name", help="运行名，默认 参数表名_时间")
    ap.add_argument("--isolate", action="store_true", help="每个任务单独一个进程")
    ap.add_argument("--only", help="只跑这些 job_id，逗号分隔")
    ap.add_argument("--_child", help=argparse.SUPPRESS)  # 内部用：子进程模式，值为运行目录
    a = ap.parse_args()

    jobs = read_jobs(a.params)
    if a.only:
        keep = set(a.only.split(","))
        jobs = [j for j in jobs if j["job_id"] in keep]

    if a._child:  # 子进程：只跑一个任务，把结果以 JSON 打到最后一行
        run_dir = Path(a._child)
        from atkbridge import ATK
        atk = ATK(out_dir=run_dir / "raw" / jobs[0]["job_id"])
        print("SUMMARY_JSON:" + json.dumps(run_job(atk, jobs[0], run_dir), ensure_ascii=False), flush=True)
        return

    name = a.name or f"{Path(a.params).stem}_{datetime.now():%Y%m%d_%H%M%S}"
    run_dir = REPO / "results" / name
    run_dir.mkdir(parents=True, exist_ok=True)
    print(f"运行目录：{run_dir}，共 {len(jobs)} 个任务，模式：{'每任务单独进程' if a.isolate else '同一进程'}")

    rows = []
    if a.isolate:
        for j in jobs:
            r = subprocess.run([sys.executable, __file__, a.params, "--only", j["job_id"], "--_child", str(run_dir)],
                               capture_output=True, text=True, encoding="utf-8", errors="replace", cwd=REPO,
                               env=dict(os.environ, PYTHONIOENCODING="utf-8"))
            line = next((x for x in r.stdout.splitlines()[::-1] if x.startswith("SUMMARY_JSON:")), None)
            if line:
                rows.append(json.loads(line[len("SUMMARY_JSON:"):]))
            else:
                (run_dir / f"crash_{j['job_id']}.txt").write_text(r.stdout[-5000:] + "\n" + r.stderr[-5000:], encoding="utf-8")
                rows.append({**{c: "" for c in SUMMARY_COLS}, "job_id": j["job_id"], "status": "crash",
                             "error": f"子进程退出码 {r.returncode}，见 crash_{j['job_id']}.txt"})
            print(f"  任务 {j['job_id']}: {rows[-1]['status']}")
    else:
        from atkbridge import ATK
        atk = ATK(out_dir=run_dir / "raw")
        for j in jobs:
            rows.append(run_job(atk, j, run_dir))
            print(f"  任务 {j['job_id']}: {rows[-1]['status']} {rows[-1]['error']}")
        atk.close()

    with open(run_dir / "summary.csv", "w", encoding="utf-8-sig", newline="") as f:
        w = csv.DictWriter(f, fieldnames=SUMMARY_COLS)
        w.writeheader()
        w.writerows(rows)
    print(f"汇总：{run_dir / 'summary.csv'}")


if __name__ == "__main__":
    main()
