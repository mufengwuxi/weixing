# TASKS.md：当前任务

## 任务 1：探测 ATK 接口（任意一台已注册 ATK 的电脑做一次即可）

1. 设置 ATK 目录（改成本机实际路径）：
   ```
   set ATK_HOME=D:\ATK\ATK4\ATK-4.2.0-rc.2
   ```
2. 先**不打开** ATK，运行：
   ```
   python tools/probe_atk.py
   ```
   把生成的 `report/probe_report.md` 重命名为 `report/probe_report_closed.md`。
3. 打开 ATK 界面，载入 `AccessExample.atk`，再运行一次：
   ```
   python tools/probe_atk.py
   ```
4. 新建分支 `probe-<名字>`，把整个 `report/` 目录提交推送，提 PR。

完成后由远端根据报告确定用哪种接口，再写正式代码。
