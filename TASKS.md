# TASKS.md：当前任务

任务 1 到 4 已完成。任务 4 发现：计算可见性之后，子对象集合里会多出一个 None 空槽，已在 `code/atkbridge.py` 里修复。

## 任务 5：复验

1. `git pull`，切到新分支 `verify2-<名字>`，关掉 ATK 界面。
2. 运行：
   ```
   python run.py params/example.csv --name verify2_inproc
   python run.py params/example.csv --name verify2_isolate --isolate
   ```
3. 预期结果：
   - 任务 1、2、3 的状态是 `ok`，任务 4 的状态是 `error`（故意的）。
   - 任务 1 的 `report_file` 指向 `results/verify2_*/raw/` 下的报告文件，并且该文件确实存在、有内容。
   - 两种模式下的 `access_*.csv` 内容一致。
4. 结论写进 `report/verify2_notes.md`，提交 `results/verify2_*/` 和这份说明，然后提 PR。报告文件是在 `raw/` 里，不提交，只需在说明里写上它的前 10 行。
