# 第二问批量搜索脚本（多核服务器用）

本目录是 `code/p2` 的精简合并版，可以单独拷到 Linux 服务器上并行跑，依赖 numpy、scipy、pyerfa。
目标星初始状态放在 `states.json`，内容就是 `data/数据.xlsx` 里的数据。

| 文件 | 作用 |
|---|---|
| `p2lib.py` | 动力学、太阳星历、观测机会、线性化瞄准、单段 SLSQP、双目标飞越 |
| `sched.py` | 第一层：相位模型束搜索排日程。参数依次为 mu、束宽、单段最长天数、巡游角度权重、漂移率上限，环境变量 `WSCALE` 用来放宽偏离公共交点方向目标的漂移率限制 |
| `opt.py` | 第二层：按日程逐段精确求解，再对缺失目标尝试同次飞越插入 |
| `job.sh` | 一组参数的完整流程：排日程，然后精确求解 |
| `insert2.py` | 把缺失目标插到某个间隙，用重度多起点求解，之后接力 |
| `retime.py` | 局部改进：第 k 段换观测日期，或用重度多起点重解 |
| `check.py` | 独立重放检查：观测约束、观测期间有没有机动、机动间隔 |

用法示例：
```
export OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1
mkdir -p logs out
for mu in 3 4 5; do for ct in 3 4; do for ws in 2.0 2.5 3.0; do nohup ./job.sh $mu 10000 3 $ct 2.6 $ws & done; done; done
```
