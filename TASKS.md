# TASKS.md：当前任务

任务 8 已完成（第一问方案按旧数据复核可行，但卡在约束边界上）。现在 `数据.xlsx` 更新了，要先确认新数据，再决定能否复核。

## 任务 9：用更新后的数据重新检查（xiehuiyu 本机）

1. `git pull`，切到新分支 `data-update-xiehuiyu`，关掉 ATK 界面。
2. 用新数据覆盖仓库里的旧数据：
   ```
   copy /Y "I:\建模比赛2\数据.xlsx" data\数据.xlsx
   ```
3. **先做数据检查，这一步必须在第 4 步之前做**（它要和仓库里旧的 `solutions/p1_doc.json` 对比）：
   ```
   python tools/check_data.py data/数据.xlsx
   ```
   结果写在 `report/data_check.md`。看最后的“结论”那一行：
   - **如果是“新 r1 与第一问方案使用的 r1 一致”**：继续做第 4 步。
   - **如果是“不一致”**：不要做第 4 步，直接跳到第 5 步。现有机动方案是按旧数据求的，套在新数据上没有意义，需要 zzx 用新数据重新求解第一问。
4. 重新生成方案文件并复核：
   ```
   python tools/make_p1_from_doc.py data/数据.xlsx
   python tools/verify_p1_atk.py solutions/p1_doc.json
   ```
   `make_p1_from_doc.py` 会自动判断：新表里的 r1 如果已经是正常的近圆轨道，就直接使用，不再取反。
5. 提交并提 PR：
   - `data/数据.xlsx`、`report/data_check.md`
   - 做了第 4 步的话，再加上 `solutions/p1_doc.json` 和 `results/p1_atk_verify/`（`summary.md`、`samples.csv`、`scan.csv`）

   PR 说明里写清楚：新表里 r1 的六个数，`data_check.md` 的结论，以及做了第 4 步的话第 4、5 节的关键数字。出错就把控制台输出存成 `report/data_update_console.txt` 一起提交。不要改 `tools/` 和 `code/`。
