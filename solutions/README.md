# solutions：机动方案文件

## 版本表

| 问题 | 文件 | 版本 | 作者 | 结果 | 状态 |
|---|---|---|---|---|---|
| 第一问 | `q1/p1_doc.json` | v1 | zzx | 5.2127 m/s | 已被 v2 取代 |
| 第一问 | `q1/p1_claude_limit.json` | v2 极限版 | Claude | 5.212705 m/s | 贴边界，仅作对照 |
| 第一问 | `q1/p1_claude_robust.json` | v2 稳健版 | Claude | 5.270312 m/s | 稳健性对照 |
| 第一问 | `q1/p1_claude_v3.json` | v3（旧数据） | Claude | 5.212816 m/s | 已被 v4 取代 |
| 第一问 | `q1/p1_claude_v4.json` | v4（新数据） | Claude | 5.212815 m/s | **当前采用** |
| 第二问 | `q2/zzx_Q2_25targets_model_reproducible.zip` | v1 | zzx | 25 颗，188.285 m/s | 已被 v2 取代 |
| 第二问 | `q2/p2_claude_27.json` | v2-27 | Claude | 27 颗，149.92 m/s | 已被取代 |
| 第二问 | `q2/p2_claude_29.json` | v2-29 | Claude | 29 颗，172.09 m/s | 已被取代 |
| 第二问 | `q2/p2_claude_30.json` | v2-30（旧数据） | Claude | 30 颗，177.2571 m/s | 旧数据 |
| 第二问 | `q2/v3a_p2_m2a.json` | v3-A（新数据，M2-A） | Claude | 30 颗，210.387 m/s | M2-B 的初值 |
| 第二问 | `q2/v3b_p2_scp.json` | v3-B（新数据，M2-B 仅 SCP） | Claude | 30 颗，180.761 m/s | 已被 v3-C 取代 |
| 第二问 | `q2/v3c_p2_lns_scp.json` | v3-C（新数据，M2-B 完整流程） | Claude | 30 颗，176.312 m/s | **当前采用** |
| 第三问 | `q3/p3_claude.json`（汇总），`q3/p3_r1.json`、`q3/p3_r2.json`（每颗星各一份） | v1 | Claude | 30 颗，55.648 m/s | 已被 v2 取代 |
| 第三问 | `q3/v2_p3_claude.json`（汇总），`q3/v2_p3_r1.json`、`q3/v2_p3_r2.json` | v2（旧数据） | Claude | 30 颗，34.263 m/s（δa ≥ 15 km） | 已被 v3 取代 |
| 第三问 | `q3/v3_p3_claude.json`（汇总），`q3/v3_p3_r1.json`、`q3/v3_p3_r2.json` | v3（新数据） | Claude | 30 颗，35.850 m/s（δa ≥ 7.8 km） | 已被 v4 取代 |
| 第三问 | `q3/v4_p3_claude.json`（汇总），`q3/v4_p3_r1.json`、`q3/v4_p3_r2.json` | v4（新数据，M3-B） | Claude | 30 颗，34.103 m/s（δa ≥ 7.81 km） | 已被 v5 取代 |
| 第三问 | `q3/v5_p3_claude.json`（汇总），`q3/v5_p3_r1.json`、`q3/v5_p3_r2.json`；δa = 8 km 方案 `q3/v5_p3_*_da8.json` | v5（新数据，M3-C） | Claude | 30 颗，33.274 m/s（δa ≥ 19.01 km） | 灵敏度分析用（δa 饱和点） |
| 第三问 | `q3/v6_p3_claude.json`（汇总），`q3/v6_p3_r1.json`、`q3/v6_p3_r2.json` | v6（新数据，M3-C） | Claude | 30 颗，33.460 m/s（δa = 5.90 km） | **当前采用（主方案）** |

新方案用新文件名，在上表加一行，同时登记到根目录的 `CHANGELOG.md`。

## 格式

第二问和第三问每颗星的文件都用第二问格式（`"problem": "P2"`）：`r1_km` 是观测星的初始状态，`burns` 是全部脉冲（时刻和 ICRF 速度增量），`observations` 是全部观测。第三问的汇总文件（`"problem": "P3"`）另外记录 r2 的轨道设计、δa 和两星分工。下面是第一问的格式。

每个方案存成一个 JSON 文件。数值必须是**求解程序直接输出的全精度浮点数**，不要手抄、不要四舍五入。

```json
{
  "epoch_utc": "2027-01-01T00:00:00",
  "mu_km3_s2": 398600.4418,
  "r1_original_km": [x, y, z, vx, vy, vz],
  "r1_used_km":     [x, y, z, vx, vy, vz],
  "b1_km":          [x, y, z, vx, vy, vz],
  "maneuvers": [
    {"t_s": 53124.673123456, "dv_km_s": [-0.0040240123, 0.0006984456, 0.0003073789]},
    {"t_s": 101939.84012345, "dv_km_s": [-0.0010659123, -0.0003258456, 0.0000745789]}
  ],
  "obs_start_s": 138346.634123,
  "obs_duration_s": 60,
  "claimed": {"total_dv_m_s": 5.2127, "d_min_km": 10.2750, "d_max_km": 14.0474,
              "alpha_max_deg": 44.9999, "theta_max_deg": 44.9999},
  "note": "方案来源、求解代码路径、生成时间"
}
```

| 字段 | 含义 |
|---|---|
| `epoch_utc` | 初始历元（UTC），所有 `t_s` 都是相对它的秒数 |
| `r1_original_km` | `数据.xlsx` 里 r1 的原始值，单位 km、km/s |
| `r1_used_km` | 求解实际使用的 r1 初始状态（如做过修正，就和原始值不同） |
| `b1_km` | b1 初始状态 |
| `maneuvers` | 每次脉冲：时刻（秒）和 ICRF 下的速度增量（km/s） |
| `obs_start_s` | 观测开始时刻（秒） |
| `claimed` | 文档里声称的检查结果，用来对照 |
