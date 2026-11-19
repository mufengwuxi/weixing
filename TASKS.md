# TASKS.md：当前任务

任务 1 到 5 已完成。任务 5 发现：`OutputDataReport` 不会自己建 `Output` 子目录，目录不存在时直接不写文件，也不报错。现在 `ATK()` 初始化时会先建好这个目录；`report()` 也会检查文件是否真的生成，没生成就报错。

## 任务 6：最终验收

1. `git pull`，切到新分支 `final-<名字>`，关掉 ATK 界面。**不要手动建任何目录。**
2. 运行：
   ```
   python run.py params/example.csv --name final_inproc
   python run.py params/example.csv --name final_isolate --isolate
   ```
3. 预期结果：
   - 任务 1、2、3 的状态是 `ok`，任务 4 的状态是 `error`。
   - 两次运行都生成了报告文件，路径分别是 `results/final_inproc/raw/Output/J2000 Position Velocity.txt` 和 `results/final_isolate/raw/1/Output/J2000 Position Velocity.txt`。
   - 两次运行的 `access_*.csv` 内容一致。
4. 结论写进 `report/final_notes.md`，提交 `results/final_*/` 和这份说明，然后提 PR。
