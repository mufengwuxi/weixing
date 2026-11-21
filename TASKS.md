# TASKS.md：当前任务

已完成或已被取代的任务挪到了 `report/tasks/已完成任务.md`。

**2026-11-20 仓库目录已重整**：方案按问题放进 `solutions/q1/`、`q2/`、`q3/`，结果写进 `results/q1/`、`q2/`、`q3/`，日志和截图放到 `report/tasks/`。开工前先 `git pull`，然后照抄下面的新命令。如果手上有按旧路径做了一半的任务，就按新路径重跑；已经提交了的 PR 也请改成新路径。

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
