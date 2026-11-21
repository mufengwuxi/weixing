#!/bin/bash
# 用法: remargin.sh <tag> <targets> [init]   在已有日程上用小裕量重解
tag=$1; tg=$2; init=$3
pk=$(ls tmp3/${tag}_*.pkl | tail -1)
export P2_TARGETS=$tg P2_MD=0.05 P2_MA=0.05 P2_D=60.5 P2_CERT_MD=0.01 P2_CERT_MA=0.01 OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1
[ -n "$init" ] && export P2_INIT=$init
python3 opt.py $pk out3m/$tag.json > tmp3/${tag}_m.log 2>&1
