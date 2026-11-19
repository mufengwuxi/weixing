# TASKS.md：当前任务

任务 1 到 3（探测接口、验证 Component、导出接口定义）已完成。

## 任务 4：验证联动环境

1. `git pull`，切到新分支 `verify-<名字>`。确认已设置 `ATK_HOME`，并关掉 ATK 界面。
2. 依次运行下面两条命令（Python 路径按本机实际情况填）：
   ```
   python run.py params/example.csv --name verify_inproc
   python run.py params/example.csv --name verify_isolate --isolate
   ```
3. 对照下面的预期检查结果，结论写进 `report/verify_notes.md`：
   - 任务 1 到 3 的状态是 `ok`。任务 4 的状态是 `error`，报错提示找不到对象，这是故意设计的。
   - 任务 2、3 的 `r_expected_m` 和 `r_actual_m` 应该基本相等（误差在 1 米以内）。如果差很多，或者差了 1000 倍，说明单位理解错了，照实写下来。
   - `results/verify_*/raw/` 下面有 ATK 输出，ATK 安装目录里没有新增文件。
   - 同一进程运行和单独进程运行的结果是否一致。
4. 提交 `results/verify_inproc/` 和 `results/verify_isolate/`（`raw/` 已被忽略）以及 `report/verify_notes.md`，然后提 PR。
5. 如果出错，把控制台输出存成 `report/verify_console.txt` 一起提交。不要改 `code/` 和 `run.py`。
