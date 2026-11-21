# weixing：建模比赛 A 题（高轨卫星抵近观测与机动规划）

三名队员各自在 Windows 电脑上用 ATK 运行和复核，远端 Claude 负责写求解代码。所有代码、方案、复核结果和论文素材都在这个仓库里协作完成。

## 当前最优结果

| 问题 | 当前采用版本 | 结果 | 方案文件 | 说明文档 | ATK 复核 |
|---|---|---|---|---|---|
| 第一问 | v4（Claude，新数据） | 5.212815 m/s | `solutions/q1/p1_claude_v4.json` | `docs/00_题目/v1_约束核对_claude.md` | 任务 17 |
| 第二问 | v2 30 颗版（Claude，旧数据） | 30 颗，177.2571 m/s | `solutions/q2/p2_claude_30.json` | `docs/q2/v2_第二问_claude.md` | 新数据待重算（简化流程目前 28 颗） |
| 第三问 | v3 双星分工（Claude，新数据） | 30 颗，35.850 m/s（δa ≥ 7.8 km） | `solutions/q3/v3_p3_claude.json` | `docs/q3/v3_第三问_claude.md` | 任务 17 |

每一问的版本是怎么演进的，见 [CHANGELOG.md](CHANGELOG.md)。高精度新数据已于任务 16 合并；第一问 v4、第三问 v3 用的是新数据，第二问还在用旧数据，待重算。三问统一的数学模型见 `docs/模型/v1_模型抽象_claude.md`。

## 目录

| 目录 | 放什么 | 谁来改 |
|---|---|---|
| `data/` | 组委会数据 `数据.xlsx` | 只读，数据更新时由组长替换 |
| `docs/` | 建模与分析文档，按问分子目录，文件名带版本和作者（见 `docs/README.md`） | 作者本人 |
| `paper/` | 论文撰写区：章节稿 `sections/`、配图 `figures/`（见 `paper/README.md`） | 按章节分工 |
| `solutions/` | 机动方案 JSON，按问分子目录（格式见 `solutions/README.md`） | 求解代码输出，不手改 |
| `code/` | 求解代码：`atkbridge.py` 封装 ATK；`p1/`、`p2/`、`p3/` 分别是三问的求解器 | 远端 Claude |
| `tools/` | 复核、出图、数据检查脚本 | 远端 Claude |
| `results/` | 复核与运行结果，按问分子目录；`env/` 是早期 ATK 环境测试的输出 | 本机 agent 提交 |
| `report/` | `tasks/` 存各任务的控制台日志和截图；`env/` 存 ATK 环境探测报告和官方接口定义 | 本机 agent 提交 |
| `params/`、`scenes/`、`run.py` | 通用的 ATK 批量运行环境（参数表、场景、入口） | 按需 |
| `TASKS.md` | 当前分配给本机 agent 的任务 | 远端 Claude 发布 |
| `AGENTS.md` | 本机 agent 的工作约定 | — |

## 三人协作约定

1. **分支**：不直接推 `main`。分支名用"内容-人名"，比如 `p3-xiehuiyu`、`paper-q2-zzx`。完成后提 PR，由组长合并。
2. **文档迭代**：新版本另存为新文件，不覆盖旧版。文件名写成 `v<版本号>_<主题>_<作者>.md`，例如 `docs/q2/v3_第二问_zzx.md`；旧版保留下来，作为论文"模型改进"部分的依据。
3. **方案迭代**：方案文件由求解程序直接输出，不手改数值。新版本用新文件名，并在 `solutions/README.md` 的版本表和 `CHANGELOG.md` 里各登记一行。
4. **论文**：每人负责若干章节文件，只改自己的章节；引用别人的数字时，写明出处的方案文件或文档。
5. **大文件**：ATK 场景（`*.atk`）、`raw/` 目录和中间结果不提交；论文要用的图和汇总表才提交。

## ATK 环境（第一次使用）

1. 本机装好 ATK 并完成扫码注册，装好 Python 3.12，再安装依赖：`pip install numpy scipy pyerfa openpyxl`。
2. 克隆仓库：`git clone https://github.com/mufengwuxi/weixing.git`
3. 设置 ATK 安装目录（改成自己的路径），设完后重开命令行窗口：
   ```
   setx ATK_HOME "D:\ATK\ATK4\ATK-4.2.0-rc.2"
   ```
4. 跑一遍示例，确认环境正常：`python run.py params/example.csv`。

如果提示 "Python was not found"，说明 PATH 里的 `python` 是 Microsoft Store 的占位符，把它换成 Python 的完整路径。批量运行前要关掉 ATK 界面。ATK 官方示例脚本（`report/env/vendor_samples/examples/`）是 GBK 编码，本仓库导出的报告和 CSV 都是 UTF-8 编码。

在代码里直接调用 ATK：

```python
import sys; sys.path.insert(0, "code")
from atkbridge import ATK

atk = ATK(out_dir="results/my_test/raw")
atk.load("scenes/AccessExample.atk")
print(atk.objects())
```

参数表的写法见 `params/README.md`。查 ATK 的方法名，看 `report/env/vendor_samples/component/Python/ATKComponentPythonModule.py`。
