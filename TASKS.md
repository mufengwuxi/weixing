# TASKS.md：当前任务

已完成或已被取代的任务挪到了 `report/tasks/已完成任务.md`。

**2026-11-20 仓库目录已重整**：方案按问题放进 `solutions/q1/`、`q2/`、`q3/`，结果写进 `results/q1/`、`q2/`、`q3/`，日志和截图放到 `report/tasks/`。开工前先 `git pull`，然后照抄下面的新命令。如果手上有按旧路径做了一半的任务，就按新路径重跑；已经提交了的 PR 也请改成新路径。

## 任务 18：第三问主方案 v6 的 ATK 复核（在任务 17 完成后做）

第三问按题面"R2 提前发射"重新求解，主方案为 v6（δa = 5.90 km）：两颗星合计 33.460 m/s。r1 与 v4 相同，只需复核 r2。

1. `git pull`，在任务 17 的分支上继续，或新建分支 `verify-v5-xiehuiyu`。
2. 运行：
   ```
   python tools/verify_p2.py solutions/q3/v6_p3_r2.json --out results/q3/verify_v6_r2
   python tools/build_single_r1.py solutions/q3/v6_p3_r2.json --name r2 --out results/q3/single_v6_r2
   ```
   预期：20/20，累计 Δv 15.312 m/s。
3. 生成 r2 的粗采样查看场景，并截图为 `report/tasks/task18_r2_gui.png`。生成方法与任务 17 相同。
4. 把任务 17 里写的"查看场景生成器"脚本提交到 `tools/make_view_scene.py`。它的参数是方案文件、卫星名、输出路径和星历步长，开头用注释写明用法。以后截图统一用它，不再生成 1.9 GB 的官方场景。
5. 提交两个 `summary.md`、截图、脚本，以及控制台输出 `report/tasks/task18_console.txt`，提 PR。

## 任务 17（最优先）：当前最优方案的 ATK 复核（新数据）

数据已更新为高精度版（任务 16）。下面四个方案是目前各问的最优版本，都用新数据求解。旧数据的任务 13、14、15 不用再做。

| 问题 | 方案文件 | 预期结果 |
|---|---|---|
| 第一问 v4 | `solutions/q1/p1_claude_v4.json` | 5.212815 m/s |
| 第二问 v3-C | `solutions/q2/v3c_p2_lns_scp.json` | 30 颗，176.312 m/s |
| 第三问 v4 r1 | `solutions/q3/v4_p3_r1.json` | 10 颗，18.148 m/s |
| 第三问 v4 r2 | `solutions/q3/v4_p3_r2.json` | 20 颗，15.956 m/s |

1. `git pull`，切到新分支 `verify-best-xiehuiyu`，关掉 ATK 界面。
2. 第一问：
   ```
   python tools/verify_p1_atk.py solutions/q1/p1_claude_v4.json --out results/q1/p1_verify_v4
   ```
   预期：ATK 轨道上的观测检查全部为"是"，最大侦测角和光照角都不超过 45°。
3. 第二问、第三问的接力复核（每条命令单独运行）：
   ```
   python tools/verify_p2.py solutions/q2/v3c_p2_lns_scp.json --out results/q2/verify_v3c
   python tools/verify_p2.py solutions/q3/v4_p3_r1.json --out results/q3/verify_v4_r1
   python tools/verify_p2.py solutions/q3/v4_p3_r2.json --out results/q3/verify_v4_r2
   ```
   预期：上表中的观测数和 Δv，ATK 与 Python 的位置差在米级。
4. 用单颗卫星的 MCS 段序列复现（每条命令单独运行）：
   ```
   python tools/build_single_r1.py solutions/q2/v3c_p2_lns_scp.json --out results/q2/single_v3c
   python tools/build_single_r1.py solutions/q3/v4_p3_r1.json --out results/q3/single_v4_r1
   python tools/build_single_r1.py solutions/q3/v4_p3_r2.json --name r2 --out results/q3/single_v4_r2
   ```
   预期：第 3 节分别为 30/30、10/10、20/20。用 ATK 界面打开 `results/q2/single_v3c/raw/single_r1.atk` 和 `results/q3/single_v4_r2/raw/single_r1.atk`，展开机动规划段列表，截图存为 `report/tasks/task17_p2_gui.png`、`report/tasks/task17_r2_gui.png`。
5. 提交 `results/` 下各 `summary.md`、两张截图，以及控制台输出 `report/tasks/task17_console.txt`，提 PR。出错时把完整报错写进 `task17_console.txt`，同样提交。

## 任务 16（最优先）：上传组委会更新后的高精度数据

题目数据更新了，数值的小数位比原来更多。这个任务只换数据，不跑别的复核。

1. `git pull`，切到新分支 `data-update-xiehuiyu`。
2. 用新文件**直接覆盖**仓库里的 `data/数据.xlsx`。新文件在本机 `I:\建模比赛2\数据.xlsx`，以队员告诉你的实际路径为准。只复制文件，不要用 Excel 打开后另存，也不要改单元格格式。
3. 导出全精度 CSV，并检查数据：
   ```
   python tools/dump_data.py data/数据.xlsx > report/tasks/task16_console.txt
   python tools/check_data.py data/数据.xlsx >> report/tasks/task16_console.txt
   ```
   打开 `report/tasks/task16_console.txt` 看一下：每行的单元格类型应该是 `['float']`；如果出现 `str`，说明数值存成了文本，要在 PR 里写明。
4. 提交 `data/数据.xlsx`、`data/数据_全精度.csv`、`report/tasks/data_check.md`、`report/tasks/task16_console.txt`，推送后提 PR，标题写"任务16：更新高精度数据"。

## 任务 13：复核第二问 30 颗版本（优先做这个，任务 11、12 可以跳过）

1. `git pull`，切到新分支 `p2-claude30-verify-xiehuiyu`，关掉 ATK 界面，然后运行：
   ```
   python tools/verify_p2.py solutions/q2/p2_claude_30.json --out results/q2/p2_verify_30
   ```
2. 预期结果：机动约束全部满足；Python 复核 30/30；ATK 复核 30/30；累计 Δv 约 177.26 m/s。特别注意第 21、22 行的 b19 和 b20：它们是同一次飞越中先后观测的，b19 这次观测不需要机动。
3. 提交 `results/q2/p2_verify_30/` 下的 `summary.md` 和 `observations.csv`，然后提 PR，PR 说明里写上三处复核结论。

## 任务 14：只用一颗 r1 卫星在 ATK 里搭出机动序列（不再一段一颗卫星）

`tools/build_single_r1.py` 把 r1 的传播器设为机动规划（MCS），段序列是“初始状态 →（预报段 → 机动段）× 每次脉冲 → 预报段”。整个场景里只有一颗 r1，目标星仍是各自的卫星。

1. `git pull`，切到新分支 `single-r1-xiehuiyu`，关掉 ATK 界面。
2. 先用第一问试（只有 2 次脉冲，几分钟内能跑完）：
   ```
   python tools/build_single_r1.py solutions/q1/p1_claude_robust.json --out results/q1/single_r1_p1
   ```
   看 `results/q1/single_r1_p1/summary.md`：
   - 第 1 节：脚本会自动试几种 VNC 坐标约定，采用误差最小的那个，这个误差应该在 1 m 以内；
   - 第 3 节：“与解析重放位置差”应该在几米以内，并且观测满足约束。
3. 第一问通过后，再跑第二问（58 次脉冲，会慢一些）：
   ```
   python tools/build_single_r1.py solutions/q2/p2_claude_30.json --out results/q2/single_r1_p2
   ```
4. 用 ATK 界面打开 `results/q2/single_r1_p2/raw/single_r1.atk`，确认场景里只有一颗 r1，r1 的机动规划里能看到各个预报段和机动段。截一张图放到 `report/tasks/single_r1_gui.png`。
5. 提交两个 `summary.md` 和截图，然后提 PR。如果第 1 节标定误差很大，或者脚本报错，先把 `summary.md` 和控制台输出（存成 `report/tasks/task14_console.txt`）提交上来，不要自己改脚本。

### 任务 14 第二轮（脚本已按 PR #18 的定位修好）

修改内容：
- 去掉了标定场景，全程只建一个场景，不再在同一进程里关闭后重开；
- VNC 约定直接采用任务 14 标定出的 (1, 1, False)；
- 去掉了会报“can not choose parent satellite's axes”的 `SetThrustAxesName`。

`git pull` 后，从任务 14 的第 2 步重新做一遍：先跑第一问，再跑第二问，最后截图。第 2 节新增了一行“第一次脉冲后 3 h 位置差”，应该在 1 m 以内。

### 任务 14 第三轮

已按 PR #19 恢复 `SetThrustAxesName(f"Satellite/{name} VNC(Earth)")`，结束预报段多留了 600 s。`git pull` 后从第 2 步重跑：第一问 → 第二问 → 截图。

### 任务 14 第四轮

已合并 PR #12、#13、#16、#17、#20（`atkbridge.save()` 的修复已经在 main 上）。`build_single_r1.py` 现在会把恰在历元时刻的首脉冲并入初始速度。`git pull` 后，只需重跑第二问并截图：
```
python tools/build_single_r1.py solutions/q2/p2_claude_30.json --out results/q2/single_r1_p2
```
预期：第 2 节会多一行“首脉冲在历元时刻，已并入初始状态速度”，3 h 位置差在 1 m 以内，第 3 节 30/30 满足。截图请点开 r1 的机动规划，让段列表（Coast、Burn）可见。

## 任务 15：第三问（双星）方案的 ATK 复核

第三问方案：r1 观测西侧 10 颗，用 19.239 m/s；r2（初始轨道由我们设计）观测东侧 20 颗，用 36.409 m/s；合计 30 颗，55.648 m/s。说明见 `docs/q3/v1_第三问_claude.md`。

1. `git pull`，切到新分支 `p3-xiehuiyu`，关掉 ATK 界面。
2. 两星各跑一遍接力复核：
   ```
   python tools/verify_p2.py solutions/q3/p3_r1.json --out results/q3/verify_p3_r1
   python tools/verify_p2.py solutions/q3/p3_r2.json --out results/q3/verify_p3_r2
   ```
   预期结论分别为 10/10 和 20/20，ATK 与 Python 的位置差在米级。
3. 每颗星各用一颗卫星的 MCS 段序列复现，每条命令是一个独立进程：
   ```
   python tools/build_single_r1.py solutions/q3/p3_r1.json --out results/q3/single_p3_r1
   python tools/build_single_r1.py solutions/q3/p3_r2.json --name r2 --out results/q3/single_p3_r2
   ```
   预期：第 2 节"3 h 位置差"在 1 m 以内，第 3 节分别为 10/10 和 20/20。r2 的首脉冲已经并入初始轨道，不会出现"首脉冲在历元时刻"那一行。
4. 用 ATK 界面打开 `results/q3/single_p3_r2/raw/single_r1.atk`，展开 r2 的机动规划段列表，截图保存为 `report/tasks/single_p3_r2_gui.png`。
5. 提交 4 个 `summary.md` 和截图，提 PR。报错时按 AGENTS.md 的规则，把控制台输出存为 `report/tasks/task15_console.txt` 并提交。
