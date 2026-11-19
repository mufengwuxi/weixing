# TASKS.md：当前任务

## 任务 1：探测 ATK 接口（已完成）
## 任务 2：验证 Component 模式（已完成，3.12 和 ATK 自带 Python 都能导入）

## 任务 3：导出接口定义并载入比赛场景

1. `git pull`，切到新分支 `api-<名字>`。
2. 设置 ATK 目录：
   ```
   set ATK_HOME=I:\建模比赛2\ATK4\ATK-4.2.0-rc.2
   ```
3. 找到竞赛软件包 MMC10 文件夹里的 `测试.atk`，把它的完整路径填进下面的命令。关掉 ATK 界面后运行：
   ```
   C:\Users\xie\AppData\Local\Programs\Python\Python312\python.exe tools/dump_api.py --scenario "<测试.atk 的完整路径>"
   ```
4. 提交并推送以下内容，然后提 PR：
   - `report/api_report.md`
   - `report/vendor_samples/component/Python/ATKComponentPythonModule.py`
   - `report/vendor_samples/examples/*.java`
   - `scenes/测试.atk`
5. 如果脚本中途崩溃（包括 Python 进程直接退出、没有报错信息），`report/api_report.md` 里会保留崩溃前的内容。照样提交，再把控制台输出的最后 100 行存成 `report/api_console.txt` 一起提交。

`report/atk_output/` 是 ATK 运行时的输出目录，已被忽略，不用提交。
