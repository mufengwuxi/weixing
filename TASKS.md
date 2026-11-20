# TASKS.md：当前任务

联动环境已验收（任务 1 到 6）。现在开始复核第一问，先读 `docs/第一问复核意见.md`。

## 任务 7A：导出第一问方案（由第一问的求解者 zzx 来做）

1. `git pull`，切到新分支 `zzx-p1-export`。
2. 把 `数据.xlsx` 复制到 `data/数据.xlsx`。
3. 把第一问的求解代码放进 `code/p1/`，附一个简短的 `code/p1/README.md`，写清楚运行命令和依赖。
4. 按 `solutions/README.md` 的格式，**用求解程序直接导出**最佳方案，存成 `solutions/p1_best.json`。数值必须是全精度，不要从文档里抄。`r1_original_km` 填 `数据.xlsx` 里的原值，`r1_used_km` 填实际求解用的值。
5. 在 `solutions/p1_best.json` 的 `note` 字段里写一句：r1 为什么要做符号修正，依据是什么。
6. 提交并推送，提 PR。

## 任务 7B：ATK 复核（7A 合并后，在任意一台装了 ATK 的电脑上做）

1. `git pull`，切到新分支 `p1-atk-verify-<名字>`，关掉 ATK 界面。
2. 运行：
   ```
   python tools/verify_p1_atk.py solutions/p1_best.json
   ```
   这一步会调用几千次 ATK，可能要几分钟，中途不要打断。
3. 提交 `results/p1_atk_verify/` 下的 `summary.md` 和三个 CSV 文件（`raw/` 已被忽略），然后提 PR。
4. 如果脚本报错，把控制台输出存成 `report/p1_verify_console.txt` 一起提交。不要改 `tools/` 和 `code/`。
