# 任务 5 复验结论

## 环境与命令

- 本机 Windows 11，ATK `4.2.0-rc.2+20261107`（已注册），运行前已关闭 ATK 界面
- `ATK_HOME=I:\建模比赛2\ATK4\ATK-4.2.0-rc.2`
- 命令：
  ```
  python run.py params/example.csv --name verify2_inproc
  python run.py params/example.csv --name verify2_isolate --isolate
  ```

## 1. 空槽 bug 确实修好了

| job | verify2_inproc | verify2_isolate | 窗口数 |
|---|---|---|---|
| 1 | **ok** | **ok** | 16 |
| 2 | **ok** | **ok** | 5 |
| 3 | **ok** | **ok** | 11 |
| 4 | error（预期） | error（预期） | — |

任务 1 从 `AttributeError` 变成 `ok`，`KeyError` 那条也照旧正确记录。

两种模式的 `access_1/2/3.csv` **sha256 全部一致**，结果仍然可复现。

## 2. 但报告文件没有真的生成：ATK 不会自动建 `Output/` 目录

`summary.csv` 里 `report_file` 写的是

```
results/verify2_inproc/raw/Output/J2000 Position Velocity.txt
```

**这个文件不存在，`raw/` 目录是空的。** `OutputDataReport()` 把路径字符串返回来了，但写盘失败，**而且不抛异常、不给任何提示**。

定位过程：

1. `GetSaveFileBasePath()` 返回的路径是对的（`...\results\verify2_inproc\raw`）。
2. 调用后 `raw/` 里 0 个文件，ATK 安装目录也没有新增。
3. 手动建出 `raw\Output\` 再跑同一条命令 —— 文件立刻出现（191,950 字节）。

**结论：`OutputDataReport` 只负责返回路径，不会创建 `Output/` 子目录；目录不存在就静默失败。**

建议在 `code/atkbridge.py` 里补两行（我没改 code）：

```python
# __init__ 里
(self.out_dir / "Output").mkdir(parents=True, exist_ok=True)
```

`--isolate` 模式下每个任务的 `out_dir` 是 `raw/<job_id>`，所以建在 `__init__` 里最合适，两种模式都覆盖。

## 3. 证据：预建目录后整条链路跑通（verify2_fixcheck）

为了确认“只差这一个目录”，我在 `results\verify2_fixcheck\raw\Output` 预先建好目录，用同一条命令又跑了一遍：

| job | status | report_file |
|---|---|---|
| 1 | ok | `results/verify2_fixcheck/raw/Output/J2000 Position Velocity.txt`（**191,950 字节，存在**） |
| 2 | ok | — |
| 3 | ok | — |
| 4 | error（预期） | — |

报告前 10 行（文件是 UTF-8）：

```
Satellite-卫星1

         DateTime(UTCG_MM)               x(m)               y(m)               z(m)         vx(m/s)         vy(m/s)         vz(m/s)
--------------------------    ---------------    ---------------    ---------------    ------------    ------------    ------------
2023-03-11 00:00:00.000000     6678137.000000          -0.000118          -1.403309        0.001148     5462.937643     5462.937253
2023-03-11 00:01:00.000000     6662055.617050      327513.112963      327511.689782     -535.832003     5449.782482     5449.782205
2023-03-11 00:02:00.000000     6613888.780882      653448.874058      653447.437740    -1069.084507     5410.380355     5410.380194
2023-03-11 00:03:00.000000     6533868.470161      976237.527940      976236.085403    -1597.188141     5344.921031     5344.920985
2023-03-11 00:04:00.000000     6422380.074636     1294324.476324     1294323.034513    -2117.599478     5253.719769     5253.719840
```

起点地心距 6678 km、速度 5463 m/s，量级正确。**所以除了这个目录问题，其余全通。**

## 4. ATK 安装目录

任务 5 运行期间（00:00 之后）安装目录**新增/修改 0 个文件**。

## 5. 三个目录各自的用途

- `results/verify2_inproc/`、`results/verify2_isolate/`：复验的原始结果，`raw/` 是空的（就是上面那个 bug）。
- `results/verify2_fixcheck/`：预建 `Output/` 后的对照实验，只有 `summary.csv`，`raw/` 不提交。

控制台输出在 `report/verify2_console.txt`。
