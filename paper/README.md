# paper：论文撰写区

## 章节与素材

| 文件 | 章节 | 主要素材 | 页数 | 负责人 | 状态 |
|---|---|---|---|---|---|
| `sections/00_摘要.md` | 摘要 | `README.md` 结果表 | 1 | 待定 | 未开始 |
| `sections/01_问题重述.md` | 1 问题重述 | `docs/00_题目/` | 2 | Claude | 已定稿 |
| `sections/02_问题分析.md` | 2 问题分析 | `docs/00_题目/`、`docs/模型/` | 3 | Claude | 正文定稿，配图初稿待审 |
| `sections/03_假设与符号.md` | 3 模型假设与符号说明 | `docs/00_题目/初步建模_zzx.md`、`CHANGELOG.md` 数据版本 | 2.5 | 待定 | 未开始 |
| `sections/04_统一基础模型.md` | 4 统一基础模型 | `docs/模型/v1_模型抽象_claude.md` | 4 | Claude | 已入 LaTeX |
| `sections/05_第一问.md` | 5 问题一 | `docs/q1/` | 4 | 待定 | 未开始 |
| `sections/06_第二问.md` | 6 问题二 | `docs/q2/`，已有初稿 | 9 | 待定 | 初稿（需按新大纲改） |
| `sections/07_第三问.md` | 7 问题三 | `docs/q3/` | 7.5 | 待定 | 未开始 |
| `sections/08_模型检验与灵敏度分析.md` | 8 模型检验与灵敏度分析 | `results/`、`docs/模型/v1_灵敏度分析_claude.md` | 4.5 | 待定 | 未开始 |
| `sections/09_评价与推广.md` | 9 模型评价与推广 | — | 1.5 | 待定 | 未开始 |
| `sections/99_参考文献.md` | 参考文献 | — | — | 全员 | 随写随加 |

正文共 40 页（含图表，不含参考文献和附录）。

**排版用 LaTeX**：`paper/tex/` 是最终排版源，`./build.sh` 一键编译出 `main.pdf`（需要 xelatex 和 rsvg-convert；Windows 上用 xelatex 编译 `main.tex` 即可，图需先用 Inkscape 或 rsvg 转成 PDF 放进 `tex/figures/`）。`sections/*.md` 是讨论稿，定稿后同步到 `tex/sections/*.tex`。格式：题目黑体三号居中；摘要两字黑体四号、内容宋体小四；"关键词"黑体小四、内容宋体小四；一级标题居中黑体三号；二级标题左顶格黑体四号；正文宋体小四；图题表题黑体五号；公式居中编号；"参考文献"黑体四号居中、内容宋体小四。

负责人确定后填进上表。每人只改自己负责的章节，在自己的分支上提交，再提 PR。

## 写作约定

- 正文用连贯的段落，少用编号罗列；每段只讲一件事。
- 引用图表写在句子里，比如"如图 3 所示"。
- 实现细节（网格步长、裕量、求解器参数）放在求解与检验部分，不放进模型部分。
- 正文里的每个数字都要能追溯到 `solutions/` 下的方案文件或 `results/` 下的复核结果。

## 图表

- `figures/` 只放可以在 Visio 里打开编辑的纯 SVG。图由 `tools/` 下的脚本生成，比如 `python tools/make_p2_figs.py solutions/q2/p2_claude_30.json`。数据变了就重新生成，不要手工改图里的数字。
- `tables/` 放论文用的汇总表（CSV 或 Markdown）。
