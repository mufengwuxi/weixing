# 第二问批量搜索脚本（多核服务器用）

本目录是 `code/p2` 的精简合并版，可以单独拷到 Linux 服务器上并行跑，依赖 numpy、scipy、pyerfa。
目标星初始状态放在 `states.json`，内容就是 `data/数据.xlsx` 里的数据。

| 文件 | 作用 |
|---|---|
| `p2lib.py` | 动力学、太阳星历、观测机会、线性化瞄准、单段 SLSQP、双目标飞越 |
| `sched.py` | 第一层：漂移相位模型束搜索排日程。参数依次为 mu、束宽、单段最长天数、巡游角度权重、巡游速率上限。交会时允许的漂移率由模型 ω_max(δ) = min(ω_cap, 540·ρ_tol/(a(1−cos δ))) 计算，δ 是目标交线方向偏离 R1 赤道交线的角度；环境变量 `RHO_TOL`（默认 25 km）、`W_CAP`（默认 4 °/天） |
| `opt.py` | 第二层：按日程逐段精确求解，再对缺失目标尝试同次飞越插入 |
| `job.sh` | 一组参数的完整流程：排日程，然后精确求解 |
| `insert2.py` | 把缺失目标插到某个间隙，用重度多起点求解，之后接力 |
| `retime.py` | 局部改进：第 k 段换观测日期，或用重度多起点重解 |
| `check.py` | 独立重放检查：观测约束、观测期间有没有机动、机动间隔 |

用法示例：
```
export OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1
mkdir -p logs out
for mu in 3 4 5; do for ct in 3 4; do for rho in 15 25 35; do nohup ./job.sh $mu 10000 3 $ct 2.6 $rho & done; done; done
```
