# 探测补充说明

本文件由本机 agent 在任务 1 执行后补充，只记录 `probe_atk.py` 报告里看不出来的环境和判断，不含任何授权信息。

## 环境

- 系统：Windows 11（本机），ATK 已注册，版本 `4.2.0-rc.2+20261107`
- ATK 目录：`I:\建模比赛2\ATK4\ATK-4.2.0-rc.2`（本次通过 `ATK_HOME` 传入，未写死进代码）
- Python：`C:\Users\xie\AppData\Local\Programs\Python\Python312\python.exe`
  - 注意：系统 PATH 里的 `python` 是 Microsoft Store 的占位符（运行报 “Python was not found”），所以本次用全路径执行。远端如果写 `python tools/probe_atk.py`，在同类机器上可能失败。

## 两次运行的差异

| 项 | ATK 关闭 | ATK 打开并载入 AccessExample.atk |
|---|---|---|
| 127.0.0.1:8080 | 连接超时 | **仍然连接超时** |
| 其余探测项 | 相同 | 相同 |

结论：**ATK 图形界面本身不监听 Connect 端口**。要跑 Connect 模式，必须先起服务端（`UiATKServer.exe` 或 `ATKServerConsole.exe`），而不是打开 GUI 就行。

## 三个接口的现状

1. **Connect**：端口未开。`ATKServerConsole.exe` 启动时提示需要**单独的服务器授权**（和桌面端的注册码不是一回事），本机未注册该项，因此没有继续尝试。`server_config.ini` 里端口写死 `8080`。
2. **Component**：`import ATKComponentPythonModule` 失败：
   ```
   ImportError: DLL load failed while importing _ATKComponentPythonModule: 找不到指定的模块。
   ```
   判断是 `_ATKComponentPythonModule.pyd` 依赖 ATK 根目录下的 Qt/ATK DLL，而 Python 默认不会去那里找。建议远端试：
   ```python
   import os, sys
   os.add_dll_directory(r"<ATK_HOME>")          # 关键：把 ATK 根目录加进 DLL 搜索路径
   sys.path.insert(0, r"<ATK_HOME>\IntegratingWithATK\component\Python")
   import ATKComponentPythonModule as atk
   ```
   成功与否需要远端改一版再测，本机没有擅改探测脚本的逻辑。
3. **CLI**：`ATKConsole.exe --help` 返回码 0，但**没有打印 usage**（直接进入脚本模式，等输入）；`AstCLI.exe --help` 只输出 AI 相关警告（api key 为空），看着不是脚本入口；`ATKConsole` 的正式用法是 `ATKConsole [options] ... script file`。

## 给远端的建议

- 优先验证 **Component 模式 + `os.add_dll_directory`**：不需要服务器授权，能直接在 Python 进程里调引擎，最适合做批量参数扫描。
- 若要 Connect，得先解决 `ATKServerConsole` 的服务器授权，可能要单独申请，时间成本高。
- 场景文件是 XML（`<Scenario Name=...>`），可以用脚本生成/改写，不必手点界面。
