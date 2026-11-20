# TASKS.md：当前任务

任务 7 已完成，但它的光照角结果作废：ATK 太阳对象的读数不是地心太阳位置，旧版脚本没有检查就用了。详见 `docs/第一问复核意见.md` 开头。

## 任务 8：用修正后的脚本重新复核第一问（xiehuiyu 本机）

1. `git pull`，切到新分支 `p1-verify2-xiehuiyu`，关掉 ATK 界面。
2. 安装 ERFA 星历库（Python 用全路径）：
   ```
   python -m pip install pyerfa
   ```
3. 运行：
   ```
   python tools/verify_p1_atk.py solutions/p1_doc.json
   ```
   脚本会先删掉 `results/p1_atk_verify/` 下旧的 CSV，再写入新结果。
4. 核对 `summary.md` 里的这几项，并写进 PR 说明：
   - 第 2 节：ATK 太阳对象的读数长度、和 ERFA 方向相差多少角秒，是否被弃用。
   - 第 4 节：最大光照角和最大侦测角**不应再完全相等**；各种组合是否都满足约束。
   - 第 5 节：连续满足区间有多长。
5. 提交 `results/p1_atk_verify/`（`summary.md`、`samples.csv`、`scan.csv`；`raw/` 已被忽略），然后提 PR。如果出错，把控制台输出存成 `report/p1_verify2_console.txt` 一起提交。不要改 `tools/` 和 `code/`。
