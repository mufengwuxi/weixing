# 参数表说明

每行一个任务，用 Excel 或记事本编辑都行。保存时选 **CSV UTF-8** 格式，否则中文会乱码。

| 列 | 必填 | 含义 |
|---|---|---|
| `job_id` | 是 | 任务编号，同一张表里不能重复。以 `#` 开头的行会被跳过 |
| `scenario` | 是 | 场景文件路径，相对仓库根目录，比如 `scenes/AccessExample.atk` |
| `satellite` | 否 | 要操作的卫星实例名，可以是中文 |
| `access_to` | 否 | 计算 `satellite` 到这个对象的可见窗口 |
| `epoch` | 否 | 轨道历元，格式 `5 Nov 2022 00:00:00.000`；写 `start` 表示用场景开始时刻 |
| `sma_m` `ecc` `inc_deg` `raan_deg` `argp_deg` `ta_deg` | 否 | 半长轴（米）、偏心率、倾角、升交点赤经、近地点幅角、真近点角（度）。这 7 列加上 `epoch` 全填才会改轨道，否则沿用场景里的轨道 |
| `start` `stop` | 否 | 可见性和报告的时间范围，不填就用场景时间 |
| `step_s` | 否 | 可见性计算步长（秒），默认 60 |
| `report` | 否 | 要导出的 ATK 数据报告名，比如 `J2000 Position Velocity` |
| `note` | 否 | 备注，程序不读 |

新增参数表时复制 `example.csv` 改，文件名写清楚用途，比如 `params/r1_inclination_scan.csv`。
