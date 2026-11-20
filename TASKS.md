# TASKS.md：当前任务

## 任务 11：复核第二问 Claude 版（xiehuiyu 本机）

背景见 `docs/第二问Claude版.md`。

1. `git pull`，切到新分支 `p2-claude-verify-xiehuiyu`，关掉 ATK 界面。依赖和任务 10 相同（numpy、scipy、pyerfa、openpyxl）。
2. 运行（会创建约 85 个 ATK 对象、调用几万次 ATK，可能要十几分钟，中途不要打断）：
   ```
   python tools/verify_p2.py solutions/p2_claude_27.json --out results/p2_verify_27
   ```
3. 预期结果：第 1 节机动约束全部满足；第 2 节 Python 复核 27/27；第 3 节 ATK 复核 27/27；结论累计 Δv 约 149.92 m/s。
4. 提交 `results/p2_verify_27/` 下的 `summary.md` 和 `observations.csv`（`raw/` 已被忽略），然后提 PR。PR 说明里写上三处复核结论。如果 ATK 部分太慢或出错，先加 `--no-atk` 跑完 Python 部分并提交，再在 PR 里说明 ATK 的情况；控制台输出存成 `report/task11_console.txt`。不要改 `tools/` 和 `code/`。

## 任务 12：复核第二问 29 颗版本（任务 11 之后做，或者直接替代任务 11）

1. `git pull`，在同一个分支或新分支 `p2-claude29-verify-xiehuiyu` 上运行：
   ```
   python tools/verify_p2.py solutions/p2_claude_29.json --out results/p2_verify_29
   ```
2. 预期结果：机动约束全部满足（单次最大约 28.18 m/s）；Python 复核 29/29；ATK 复核 29/29；累计 Δv 约 172.09 m/s。
3. 提交 `results/p2_verify_29/` 下的 `summary.md` 和 `observations.csv`，然后提 PR。

## 任务 13：复核第二问 30 颗版本（优先做这个，任务 11、12 可以跳过）

1. `git pull`，切到新分支 `p2-claude30-verify-xiehuiyu`，关掉 ATK 界面，然后运行：
   ```
   python tools/verify_p2.py solutions/p2_claude_30.json --out results/p2_verify_30
   ```
2. 预期结果：机动约束全部满足；Python 复核 30/30；ATK 复核 30/30；累计 Δv 约 177.26 m/s。特别注意第 21、22 行的 b19 和 b20：它们是同一次飞越中先后观测的，b19 这次观测不需要机动。
3. 提交 `results/p2_verify_30/` 下的 `summary.md` 和 `observations.csv`，然后提 PR，PR 说明里写上三处复核结论。

## 任务 14：只用一颗 r1 卫星在 ATK 里搭出机动序列（不再一段一颗卫星）

`tools/build_single_r1.py` 把 r1 的传播器设为机动规划（MCS），段序列是“初始状态 →（预报段 → 机动段）× 每次脉冲 → 预报段”。整个场景里只有一颗 r1，目标星仍是各自的卫星。

1. `git pull`，切到新分支 `single-r1-xiehuiyu`，关掉 ATK 界面。
2. 先用第一问试（只有 2 次脉冲，几分钟内能跑完）：
   ```
   python tools/build_single_r1.py solutions/p1_claude_robust.json --out results/single_r1_p1
   ```
   看 `results/single_r1_p1/summary.md`：
   - 第 1 节：脚本会自动试几种 VNC 坐标约定，采用误差最小的那个，这个误差应该在 1 m 以内；
   - 第 3 节：“与解析重放位置差”应该在几米以内，并且观测满足约束。
3. 第一问通过后，再跑第二问（58 次脉冲，会慢一些）：
   ```
   python tools/build_single_r1.py solutions/p2_claude_30.json --out results/single_r1_p2
   ```
4. 用 ATK 界面打开 `results/single_r1_p2/raw/single_r1.atk`，确认场景里只有一颗 r1，r1 的机动规划里能看到各个预报段和机动段。截一张图放到 `report/single_r1_gui.png`。
5. 提交两个 `summary.md` 和截图，然后提 PR。如果第 1 节标定误差很大，或者脚本报错，先把 `summary.md` 和控制台输出（存成 `report/task14_console.txt`）提交上来，不要自己改脚本。

### 任务 14 第二轮（脚本已按 PR #18 的定位修好）

修改内容：
- 去掉了标定场景，全程只建一个场景，不再在同一进程里关闭后重开；
- VNC 约定直接采用任务 14 标定出的 (1, 1, False)；
- 去掉了会报“can not choose parent satellite's axes”的 `SetThrustAxesName`。

`git pull` 后，从任务 14 的第 2 步重新做一遍：先跑第一问，再跑第二问，最后截图。第 2 节新增了一行“第一次脉冲后 3 h 位置差”，应该在 1 m 以内。

### 任务 14 第三轮

已按 PR #19 恢复 `SetThrustAxesName(f"Satellite/{name} VNC(Earth)")`，结束预报段多留了 600 s。`git pull` 后从第 2 步重跑：第一问 → 第二问 → 截图。
