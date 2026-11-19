# weixing：ATK 联动环境

几个人各自在 Windows 电脑上运行 ATK，代码和参数通过这个仓库共享。所有和 ATK 打交道的代码都在 `code/atkbridge.py` 里。平时只需要改参数表，然后运行 `run.py`。

## 第一次使用

1. 本机装好 ATK 并完成扫码注册，装好 Python 3.12。
2. 克隆仓库：
   ```
   git clone https://github.com/mufengwuxi/weixing.git
   ```
3. 设置 ATK 安装目录（改成自己的路径）。想让设置长期生效，就用 `setx`，设完重开命令行窗口：
   ```
   setx ATK_HOME "D:\ATK\ATK4\ATK-4.2.0-rc.2"
   ```
4. 跑一遍示例，确认环境正常：
   ```
   python run.py params/example.csv
   ```
   如果提示 "Python was not found"，就把 `python` 换成 Python 的完整路径。

## 日常使用

1. 复制 `params/example.csv`，按 `params/README.md` 的说明填参数。每行是一个任务。
2. 先关掉 ATK 界面，再运行 `python run.py params/你的参数表.csv`。开着界面时，ATK 会往安装目录写三维地图缓存。任务很多、或者 ATK 偶尔崩溃时，加上 `--isolate`，让每个任务单独开一个进程运行。
3. 结果在 `results/<参数表名_时间>/` 下：
   - `summary.csv`：每个任务一行
   - `access_<id>.csv`：可见窗口明细
   - `raw/`：ATK 原始输出，不提交
4. 在自己的分支上提交参数表和 `results/` 下的汇总结果，然后提 PR。

## 注意事项

- 批量运行前关掉 ATK 界面。
- 提示 "Python was not found" 时，说明 PATH 里的 `python` 是 Microsoft Store 占位符，换成 Python 的完整路径。
- ATK 官方示例脚本（`report/vendor_samples/examples/`）是 GBK 编码；本环境导出的报告和 CSV 是 UTF-8 编码。

## 在代码里直接调用

```python
import sys; sys.path.insert(0, "code")
from atkbridge import ATK

atk = ATK(out_dir="results/my_test/raw")
atk.load("scenes/AccessExample.atk")
print(atk.objects())
wins = atk.access("卫星1", "卫星2", step_s=60)
```

## 目录

| 目录 | 内容 |
|---|---|
| `code/` | `atkbridge.py`：ATK 封装 |
| `run.py` | 按参数表批量运行 |
| `params/` | 参数表（CSV） |
| `scenes/` | 场景文件 |
| `results/` | 运行结果 |
| `tools/` | 环境探测脚本 |
| `report/` | 探测报告和 ATK 接口定义，查方法名看 `report/vendor_samples/component/Python/ATKComponentPythonModule.py` |

本机 agent 的工作约定见 `AGENTS.md`，当前任务见 `TASKS.md`。
