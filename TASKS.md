# TASKS.md：当前任务

## 任务 10：复核 Claude 独立求解的第一问方案（xiehuiyu 本机）

背景见 `docs/第一问独立求解.md`。

1. `git pull`，切到新分支 `p1-claude-verify-xiehuiyu`，关掉 ATK 界面。
2. 安装依赖（Python 用全路径）：
   ```
   python -m pip install numpy scipy pyerfa openpyxl
   ```
3. 用 ATK 分别复核两个方案，输出到不同目录：
   ```
   python tools/verify_p1_atk.py solutions/p1_claude_robust.json --out results/p1_verify_robust
   python tools/verify_p1_atk.py solutions/p1_claude_limit.json --out results/p1_verify_limit
   ```
   预期结果：
   - robust：所有组合都满足约束，最大角度约 44.0°，距离约 12.5–16.8 km，连续满足区间约 72 s；
   - limit：全精度的几种组合满足约束（角度约 44.9999°），“Δv 4 位小数”那一行**不满足**（约 45.03°），这是预期结果，用来说明贴边界方案对精度敏感。
4. 在本机重跑一次求解器，确认结果能复现（大约几分钟）：
   ```
   python code/p1/solver.py --out results/p1_solver_rerun/robust.json --margin-d 1 --margin-a 1 --duration 70 --starts 30 --seed 5
   ```
   最优 J 应该是 5.2703 m/s 左右。把控制台输出存成 `results/p1_solver_rerun/console.txt`。
5. 提交 `results/p1_verify_robust/`、`results/p1_verify_limit/`、`results/p1_solver_rerun/`（各目录的 `raw/` 和 `.atk` 已被忽略），然后提 PR。PR 说明里写上两个方案第 4 节表格的结论，以及重跑得到的 J。出错就把控制台输出存成 `report/task10_console.txt` 一起提交。不要改 `tools/` 和 `code/`。
