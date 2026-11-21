# 任务 4 验证结论

## 环境

- 本机 Windows 11，ATK `4.2.0-rc.2+20261107`（已注册）
- Python：`C:\Users\xie\AppData\Local\Programs\Python\Python312\python.exe`（3.12.14）
- 运行前已关闭 ATK 界面，`ATK_HOME=I:\建模比赛2\ATK4\ATK-4.2.0-rc.2`
- 命令：
  ```
  python run.py params/example.csv --name verify_inproc
  python run.py params/example.csv --name verify_isolate --isolate
  ```

## 结果总览

| job | inproc | isolate | 窗口数 | r_expected_m | r_actual_m |
|---|---|---|---|---|---|
| 1 | error | error | 16（窗口算出来了） | — | — |
| 2 | ok | ok | 5 | 6993000.0 | 6993000.0 |
| 3 | ok | ok | 11 | 7499250.0 | 7499250.0 |
| 4 | error（预期） | error（预期） | — | — | — |

两种模式的 `access_*.csv` **字节级完全相同**（sha256 一致），窗口数、轨道自检值也一致。**同一进程连续载入是稳定的。**

## 1. 单位：确认是“米 + 度”，没有问题

任务 2：`a=7000000, e=0.001, f=0` → 理论地心距 `a(1-e²)/(1+e·cos f) = 6993000.0`，实测 `6993000.0`。
任务 3：`a=7500000, e=0.01, f=90°` → 理论 `7499250.0`，实测 `7499250.0`。

**误差 0 米，不是 1000 倍的问题**，`sma_m` 用米、角度用度是对的。

## 2. 任务 1 失败：不是预期内的，是个真 bug

预期任务 1 状态 `ok`，实际 `error`：

```
AttributeError: 'NoneType' object has no attribute 'GetInstanceName'
  atkbridge.py line 215 report() -> find() -> walk() line 134
```

**已经定位到根因，可稳定复现：**

```
载入后       : scenario.GetChildren().GetCount() == 2   → Item(0)=卫星1, Item(1)=卫星2
ComputeAccess 后: scenario.GetChildren().GetCount() == 3   → Item(0)=卫星1, Item(1)=卫星2, Item(2)=None
```

也就是说，**算完可见性之后，场景的子对象集合会多出一个空槽，`Item(2)` 返回 `None`**。`walk()` 里直接 `c.GetInstanceName()` 就崩了。

任务 2、3 之所以没崩，是因为它们算完可见性就结束了，没有再调 `find()`；任务 1 在算完窗口后还要 `report()`，一调 `find()` 就撞上这个空槽。

**建议修法**（改 `code/atkbridge.py`，我没动）：

```python
# objects() 的 walk 和 find() 的 walk，两处都加
for i in range(kids.GetCount()):
    c = kids.Item(i)
    if c is None:          # 算完 access 后场景会多一个空槽
        continue
```

顺带建议：`GetCount()` 之后再 `Item(i)` 最好统一加个 `try/except`，这类“空槽”在 ATK 里可能不止一处。

## 3. 任务 4：符合预期

```
KeyError: 场景里没有名为 不存在的卫星 的对象。现有对象：卫星1, 卫星2
```
报错信息完整、能定位、被正确写进 `summary.csv` 的 `error` 列。

## 4. `raw/` 是空的，原因同 2

`results/env/verify_inproc/raw/`、`results/env/verify_isolate/raw/` 都是 0 文件。因为唯一要导出报告的 job 1 在 `report()` 这一步就崩了，还没走到 `OutputDataReport`。**修掉空槽问题后应该就有输出了。**

## 5. ATK 安装目录：本次运行没有写入

- 任务 4 两次运行期间（23:45 之后），ATK 安装目录**新增/修改文件数为 0**，`SetSaveFileBasePath` 生效了。
- 但要注意：**更早打开 ATK 图形界面时**，`AstroData\Earth\Map\8886e29d\` 下有 osgEarth 三维地图缓存写入（`.osgb` 文件，23:29 左右）。这是 GUI 看三维地球产生的缓存，不是 Component 模式写的，但说明“安装目录零写入”这个承诺只在 headless 运行时成立。

## 6. 控制台日志

完整控制台输出在 `report/env/verify_console.txt`（两种模式追加在一起）。里面有大量 ATK 内部 `Debug -1 ...` 输出，属于正常噪音。
