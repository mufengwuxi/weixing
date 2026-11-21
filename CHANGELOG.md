# 迭代记录

按问题记录每个版本的结果、相对上一版的改进，以及对应的文件和提交。写论文"模型的改进与检验"部分时以本表为准。新版本加在各节表格的最后一行。

## 数据版本

| 时间 | 内容 | 提交 |
|---|---|---|
| 11-20 | 组委会高精度数据（单位 m、m/s，单元格为文本，20 位有效数字）；b3、b5、b9、b15 四颗目标有实质修正，其余只差舍入。新增 `code/datafile.py` 统一读取 | 5d08960、3a7c52e |

## 环境搭建（2026-11-19 至 11-20）

| 时间 | 内容 | 提交 |
|---|---|---|
| 11-19 | 搭建 ATK Component 模式的 Python 联动环境（`code/atkbridge.py`、`run.py`），在本机验收通过 | 2280e77 |
| 11-20 | 发现 ATK 太阳对象读数为零，光照角改用 ERFA 太阳星历计算 | b73bc11 |
| 11-20 | 修复场景保存：`SaveScenario` 不接受路径参数 | d0b1cee |
| 11-20 | 单颗 r1 的 MCS 机动序列复核脚本，修了三轮：单场景、VNC 推力轴、历元首脉冲并入初始速度 | 1bed5c2 → ad5f7cc |

## 第一问：单星单目标

| 版本 | 作者 | 结果 | 改进点 | 文件 | 提交 |
|---|---|---|---|---|---|
| v1 | zzx | 5.2127 m/s | 规范化模型，两脉冲转移加观测窗口 | `docs/q1/v1_第一问总结_zzx.md`、`solutions/q1/p1_doc.json` | 137be49 |
| v1 复核 | Claude、xiehuiyu | 方案可行，但贴在约束边界上 | 用 ATK 独立复核，修正光照角的太阳位置，换用新数据重新检查 | `docs/q1/v1_复核意见_claude.md`、`results/q1/p1_atk_verify/` | d348e1d → 79aaf3f |
| v2 极限版 | Claude | 5.212705 m/s | 独立求解器，约束取边界 | `solutions/q1/p1_claude_limit.json` | b0acd7f |
| v2 稳健版 | Claude | 5.270312 m/s | 留出 1 km / 1° / 70 s 的裕量，ATK 复核和本机重跑均复现 | `solutions/q1/p1_claude_robust.json`、`docs/q1/v2_独立求解_claude.md` | b0acd7f、31b77ec |
| v3（旧数据） | Claude | **5.212816 m/s** | 裕量只保留 0.01 km / 0.01°。极限版在 ATK 中侦测角为 45.00004°，超出约束；0.01° 足以覆盖这一差异。还需 ATK 复核 | `solutions/q1/p1_claude_v3.json` | 09d2831 |
| **v4（新数据）** | Claude | **5.212815 m/s** | 方法同 v3，换成高精度新数据（r1、b1 只有厘米级的舍入差异） | `solutions/q1/p1_claude_v4.json` | 本次 |

## 第二问：单星多目标巡游

| 版本 | 作者 | 结果 | 改进点 | 文件 | 提交 |
|---|---|---|---|---|---|
| v1 | zzx | 25 颗，188.285 m/s | 事件束搜索加全链条重放验证 | `docs/q2/v1_第二问进展_zzx.md`、`solutions/q2/zzx_Q2_25targets_model_reproducible.zip` | 049d272 |
| v2-27 | Claude | 27 颗，149.92 m/s | 漂移相位模型排日程，逐段 SLSQP 精确求解；ATK 复核 27/27 | `solutions/q2/p2_claude_27.json`、`results/q2/p2_verify_27/` | 51351ee、2320b26 |
| v2-29 | Claude | 29 颗，172.09 m/s | 束搜索加宽，末段插入 | `solutions/q2/p2_claude_29.json` | 66d6bf5 |
| **v2-30** | Claude | **30 颗，177.2571 m/s** | b19 与 b20 在同一次飞越中先后观测，b19 不需要单独机动，实现全覆盖 | `solutions/q2/p2_claude_30.json`、`docs/q2/v2_第二问_claude.md` | 652ba57 |
| 论文稿 | Claude | — | 两层模型的论文表述、搜索实验、局部最优性证据和配图 | `paper/sections/04_第二问.md`、`paper/figures/p2_*.svg` | ce5d221、ca2fc51 |

`results/q2/single_r1_p2/summary.md` 是任务 14 修复之前的旧输出（0/30），要等任务 14 第四轮重跑后替换。

## 第三问：双星协同

| 版本 | 作者 | 结果 | 改进点 | 文件 | 提交 |
|---|---|---|---|---|---|
| v1 | Claude | 30 颗，55.648 m/s（r1 19.239 + r2 36.409），δa = 50 km | 按经度分工：r1 负责西侧 10 颗，r2 负责东侧 20 颗；r2 与 r1 共面，首脉冲并入初始轨道；扫描了 43 个 r2 设计 | `solutions/q3/`、`docs/q3/v1_第三问_claude.md`、`code/p3/` | 5660719 |
| v2（旧数据） | Claude | 30 颗，34.263 m/s（r1 16.547 + r2 17.716），要求 δa ≥ 15 km | 裕量缩小到只为数值安全保留，候选观测机会增多后重新排日程；r2 改为 Δa = +15 km，从东侧组西端出发；给出 J*(δa) 的初步参数分析 | `solutions/q3/v2_p3_*.json`、`docs/q3/v2_第三问_claude.md`、`code/p3/v2/` | 09d2831 |
| **v3（新数据）** | Claude | **30 颗，35.850 m/s**（r1 18.943 + r2 16.907），要求 δa ≥ 7.8 km | 换成高精度新数据（b9 的修正使 r1 多花 2.4 m/s）；δa 扫描加密到 0–25 km，J*(δa) 从 δa = 0 时的 36.38 降到 7.8 km 时的 35.85，之后饱和 | `solutions/q3/v3_p3_*.json`、`docs/q3/v3_第三问_claude.md`、`code/p3/v3/` | 本次 |

## 建模文档

| 版本 | 内容 | 文件 |
|---|---|---|
| v1 | 对照题面逐条核对约束，列出自行引入的裕量和搜索限制 | `docs/00_题目/v1_约束核对_claude.md` |
| v1 | 三问统一的数学模型（基本单元 LEG、时间相关定向越野、设计加分配），可作论文第 2 节底稿 | `docs/模型/v1_模型抽象_claude.md` |
