# TASKS.md：当前任务

## 任务 11：复核第二问 Claude 版（xiehuiyu 本机）

背景见 `docs/第二问Claude版.md`。

1. `git pull`，切到新分支 `p2-claude-verify-xiehuiyu`，关掉 ATK 界面。依赖和任务 10 相同（numpy、scipy、pyerfa、openpyxl）。
2. 运行（会创建约 85 个 ATK 对象、调用几万次 ATK，可能要十几分钟，中途不要打断）：
   ```
   python tools/verify_p2.py solutions/p2_claude_27.json --out results/p2_verify_27
   ```
3. 预期结果：第 1 节机动约束全部满足；第 2 节 Python 复核 27/27；第 3 节 ATK 复核 27/27；结论累计 Δv 约 149.92 m/s。
4. 提交 `results/p2_verify_27/` 下的 `summary.md` 和 `observations.csv`（`raw/` 已被忽略），然后提 PR。PR 说明里写上三处复核结论。如果 ATK 部分太慢或出错，先加 `--no-atk` 跑完 Python 部分并提交，再在 PR 里说明 ATK 的情况；控制台输出存成 `report/task11_console.txt`。不要改 `tools/` 和 `code/`。
