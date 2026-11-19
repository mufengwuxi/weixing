# TASKS.md：当前任务

## 任务 1：探测 ATK 接口（已完成，见 report/probe_report*.md）

## 任务 2：验证 Component 模式（在已注册 ATK 的电脑上做一次）

1. `git pull`，切到自己的新分支，比如 `component-<名字>`。
2. 设置 ATK 目录（改成本机实际路径）：
   ```
   set ATK_HOME=I:\建模比赛2\ATK4\ATK-4.2.0-rc.2
   ```
3. 关掉 ATK 界面，运行（Python 用全路径，见 AGENTS.md）：
   ```
   C:\Users\xie\AppData\Local\Programs\Python\Python312\python.exe tools/try_component.py
   ```
   这个脚本最多会运行十分钟左右（会跑一遍官方示例），中途不要打断。
4. 把 `report/component_report.md` 和 `report/vendor_samples/examples/` 提交推送，提 PR。
5. 如果脚本本身报错退出，把完整的控制台输出存成 `report/component_console.txt` 一起提交。

不要运行 `GetAllDeps.bat`，它会往 ATK 安装目录里复制文件。
