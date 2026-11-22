#!/bin/bash
# 用法：job.sh mu W md ct rt [rho_tol]
# 一组参数的完整流程：排日程（sched.py），然后逐段精确求解并尝试同次飞越插入（opt.py）。
export OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1 MKL_NUM_THREADS=1
cd "$(dirname "$0")"
mkdir -p logs out
export RHO_TOL=${6:-25.0}
tag="$1_$2_$3_$4_$5_rho${RHO_TOL}"
python3 sched.py $1 $2 $3 $4 $5 > logs/sched_$tag.txt 2>&1
pk=$(ls -t sched_*_rho${RHO_TOL}.pkl 2>/dev/null | head -1)
python3 opt.py "$pk" out/opt_$tag.json > logs/opt_$tag.txt 2>&1
