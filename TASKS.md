# TASKS.md：当前任务

已完成或已被取代的任务挪到了 `report/tasks/已完成任务.md`。

**2026-11-20 仓库目录已重整**：方案按问题放进 `solutions/q1/`、`q2/`、`q3/`，结果写进 `results/q1/`、`q2/`、`q3/`，日志和截图放到 `report/tasks/`。开工前先 `git pull`，然后照抄下面的新命令。如果手上有按旧路径做了一半的任务，就按新路径重跑；已经提交了的 PR 也请改成新路径。

## 任务 19：论文用 ATK 截图（11-21 修订）

论文第 5–7 节要插入 ATK 三维渲染截图。论文里卫星名一律大写（R1、R2、B1–B30），截图里的标注也要大写。

### 第一步：把查看场景生成器的大写命名正式入库

你在本机为第一问场景改成了大写 R1/B1 和场景名 Scenario1，这个改动还没进仓库。请把它做成 `tools/make_view_scene.py` 的选项，不要写死：

- 新增 `--upper`：卫星名和目标名全部转成大写（r1→R1，b12→B12）；
- 新增 `--scenario-name`：场景名，默认保持现在的值；
- 新增 `--step` 已有，不变。

### 第二步：重新生成 4 个查看场景（全部大写）

第一问是近距离飞越（相对速度约 0.23 km/s），60 s 步长会把近景轨迹画成折线，所以第一问用 1 s：
```
python tools/make_view_scene.py solutions/q1/p1_claude_v4.json r1 results/q1/single_v4_view/q1_view_1s.atk --step 1 --upper --scenario-name Scenario1
python tools/make_view_scene.py solutions/q2/v3c_p2_lns_scp.json r1 results/q2/single_v3c/raw_view/v3c_view_upper.atk --step 60 --upper
python tools/make_view_scene.py solutions/q3/v6_p3_r1.json r1 results/q3/single_v6_r1/raw_view/v6_r1_view_upper.atk --step 60 --upper
python tools/make_view_scene.py solutions/q3/v6_p3_r2.json r2 results/q3/single_v6_r2/raw_view/v6_r2_view_upper.atk --step 60 --upper
```
原来的小写场景文件不要删。生成后逐个在 ATK 里打开，确认对象浏览器里卫星都是大写、MCS 段完整、轨迹可见。

### 第三步：统一显示设置

- 3D 窗口调到不小于 1920×1080，只截 3D 视图区域（不含菜单栏、对象树、时间轴面板）。
- 背景用 ATK 默认星空；打开所有卫星的名称标注，字号调大到截图缩小到半页宽时仍能看清。
- 轨迹线宽 2 像素以上。颜色：观测卫星 R1、R2 红色；目标卫星统一浅蓝色。
- 第二、三问目标多，目标的轨道线可以关掉，只显示目标点和名称，避免满屏线条。

### 第四步：截图清单（存到 `paper/figures/atk/`）

| 文件名 | 场景 | 场景时间（UTC） | 相机 | 要能看出什么 |
|---|---|---|---|---|
| `q1_overview.png` | `q1_view_1s.atk` | 2027-01-02 14:26:16 | 中心为地球，从北极上方约 30° 斜视，视野刚好容纳整条同步轨道 | 地球、R1 在 48 h 内的轨迹（两次机动的折点）、B1 的位置 |
| `q1_closeup.png` | `q1_view_1s.atk` | 2027-01-02 14:26:16 | 中心设为 B1，视野约 100 km，从垂直于 R1 相对运动方向的一侧看 | R1 从 B1 旁边掠过的一段轨迹，R1、B1 标注清楚 |
| `q2_overview.png` | `v3c_view_upper.atk` | 2027-01-31 00:00:00 | 从北极正上方俯视，视野放大到 30 颗目标所在的约 40° 弧段 | R1 30 天的巡游轨迹和 30 颗目标 |
| `q2_oblique.png` | `v3c_view_upper.atk` | 2027-01-16 00:00:00 | 同一弧段，从赤道面上方约 20° 斜视 | 轨迹的起伏和目标的倾角差异 |
| `q3_r1.png` | `v6_r1_view_upper.atk` | 2027-01-31 00:00:00 | 与 `q2_overview.png` 完全相同（同一相机位置和视野） | R1 负责的西侧 10 颗目标和 R1 轨迹 |
| `q3_r2.png` | `v6_r2_view_upper.atk` | 2027-01-31 00:00:00 | 与 `q2_overview.png` 完全相同 | R2 负责的东侧 20 颗目标和 R2 轨迹 |

`q2_overview`、`q3_r1`、`q3_r2` 三张必须用同一个相机视角，论文里要对照着看。做法：先在第二问场景里调好视角，把相机参数（位置、朝向、视野）记下来，再在第三问两个场景里照着设置。

### 第五步：提交

- 每张图的场景时间、相机中心、相机位置、视野写进 `paper/figures/atk/README.md`，以后要重截能照着复原。
- 新建分支 `atk-figs-xiehuiyu`，提交：`make_view_scene.py` 的改动、6 张 PNG、`paper/figures/atk/README.md`、控制台输出 `report/tasks/task19_console.txt`。`.atk` 场景文件太大，不提交。
- 提 PR，PR 描述里贴 6 张图的缩略图。

### 11-21 复查结果：六张截图全部作废，需要重截

PR #27 合并后检查了六张 PNG，没有一张能用，已从仓库删除（工具改动和 README 保留）：

- `q1_overview`、`q1_closeup`、`q3_r1`、`q3_r2`：3D 区域整片是浅黄色，只有零星白色文字，看不到地球、轨迹和卫星；而且截进了左侧对象树和顶部标签栏。
- `q2_overview`、`q2_oblique`：截到的是桌面上的浏览器和文件管理器窗口，根本不是 ATK。**这两张包含了本机桌面的私人内容，以后截图前务必确认截的是哪个窗口。**

重截要求：
1. 先排查 3D 视图为什么整片发黄。可能是相机高度或视野设置使视点落进了某个对象里（比如太阳或地球）、光照/背景设置被改动，或者截的是另一个"三维视图"标签页。在 ATK 界面里确认画面正常（能看到地球和红色轨迹）后再截。
2. 截图只取 3D 视图区域，用窗口坐标裁剪，不要截全屏。
3. **每截完一张，自己用图片查看器打开 PNG 看一眼**，确认看得到地球（第一问）或目标弧段（第二、三问）、红色轨迹和大写标注，再继续下一张。
4. 提交前在 PR 描述里贴的缩略图必须是从提交的 PNG 文件生成的。
5. 还是在分支 `atk-figs-xiehuiyu` 上提新的 PR。
