# TASKS.md：当前任务

联动环境已验收（任务 1 到 6）。现在开始复核第一问，背景见 `docs/第一问复核意见.md`。

## 任务 7：按文档数值复核第一问（由 xiehuiyu 在本机做）

zzx 的求解代码和全精度结果还没进仓库，先用文档表格里的数值复核。这一步本身就是在检验复核意见的第 3 条：只按文档公开的精度，能不能复现出可行方案。

1. `git pull`，切到新分支 `p1-doc-verify-xiehuiyu`，关掉 ATK 界面。
2. 把比赛的 `数据.xlsx` 复制到 `data/数据.xlsx`。如果本机找不到这个文件，就停下来，在 PR 说明里写明缺少数据文件。
3. 安装依赖并生成方案文件（Python 用全路径）：
   ```
   python -m pip install openpyxl
   python tools/make_p1_from_doc.py data/数据.xlsx
   ```
   脚本会打印 Sheet1 前 8 行，核对第 4 行是不是 r1、第 6 行是不是 b1。行号对不上就停下，把打印内容写进 PR 说明。
4. 运行 ATK 复核：
   ```
   python tools/verify_p1_atk.py solutions/p1_doc.json
   ```
   这一步会调用几千次 ATK，可能要几分钟，中途不要打断。
5. 提交以下内容并提 PR：
   - `data/数据.xlsx`
   - `solutions/p1_doc.json`
   - `results/p1_atk_verify/` 下的 `summary.md` 和三个 CSV（`raw/` 已被忽略）
6. 如果脚本报错，把控制台输出存成 `report/p1_verify_console.txt` 一起提交。不要改 `tools/` 和 `code/`。

## 之后：拿到 zzx 的全精度结果再复核一次

等 zzx 按 `solutions/README.md` 的格式提交 `solutions/p1_best.json`，再运行 `python tools/verify_p1_atk.py solutions/p1_best.json`。
