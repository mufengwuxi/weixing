#!/bin/bash
# 把 ../figures 下的 SVG 转成 PDF 后编译。需要 rsvg-convert（librsvg）和 xelatex。
set -e
cd "$(dirname "$0")"
mkdir -p figures
for f in ../figures/*.svg; do
  n=$(basename "$f" .svg)
  rsvg-convert -f pdf -o "figures/$n.pdf" "$f"
done
latexmk -xelatex -interaction=nonstopmode -halt-on-error main.tex
