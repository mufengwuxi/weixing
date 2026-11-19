# AGENTS.md：给本机 agent 的工作约定

本仓库用于建模比赛 A 题（卫星机动规划与观测窗口计算）。分工如下：远端 Claude 负责写 Python 代码并推到仓库；各队员本机的 agent 负责在装有 ATK 的 Windows 电脑上运行代码，把输出提交回仓库。

## 本机 agent 每次开工先做

1. `git pull` 拉取最新代码。
2. 读本文件和 `TASKS.md`，只做 `TASKS.md` 里分配给本机的任务。
3. ATK 目录通过环境变量 `ATK_HOME` 指定，不要把绝对路径写死进代码。
4. 如果 PATH 里的 `python` 是 Microsoft Store 占位符（运行时提示 "Python was not found"），就改用 Python 的全路径运行脚本。
5. 控制台日志一律存成 `report/` 下的 `.txt` 文件，`*.log` 会被 `.gitignore` 忽略。

## 运行与回传

- 按 `TASKS.md` 里写的命令运行脚本，不要自行改动 `tools/` 和 `code/` 下的代码逻辑。
- 运行报错时，把完整报错写进 `report/` 下对应的日志文件并推送，交给远端修改，不要自己绕过。
- 如果确实需要临时修补才能跑通，在单独分支上改，并在提交说明里写清楚改了什么、为什么改。

## Git 规则

- 不直接推 `main`。每个队员用自己的分支，比如 `zhangsan-run`，推上去后提 PR。
- 提交说明写清楚：在哪台电脑、跑了什么命令、结果在哪个文件。
- `scenes/*.atk` 只由指定的场景负责人修改，其他人只读。
- 参数统一写在 `params/*.csv` 里，不用 xlsx，方便看改动、合并冲突。
- 大批量的中间结果不提交；论文要用的汇总表和图放 `results/` 并提交。

## 安全

- 不提交注册码、账号密码、本机 ID 之类的授权信息。
- 不修改 ATK 安装目录里的任何文件。
- 不把 ATK 的 C++ 源码、DLL、数据文件提交进仓库，只提交 TASKS.md 要求的报告和示例脚本。

## 给远端的技术备忘（Component 模式，已在本机验证）

- 启动方式：`os.add_dll_directory(ATK_HOME)`，`os.chdir(ATK_HOME)`，再把 `IntegratingWithATK/component/Python` 加进 `sys.path`，然后 `import ATKComponentPythonModule`。
- 调用前必须先 `SetSaveFileBasePath(<仓库内目录>)`，否则输出会写进 ATK 安装目录。
- ATK 自带的示例脚本是 GBK 编码。
