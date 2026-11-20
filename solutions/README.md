# solutions：机动方案文件

每个方案存成一个 JSON 文件，比如 `p1_best.json`。数值必须是**求解程序直接输出的全精度浮点数**，不要手抄、不要四舍五入。

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
