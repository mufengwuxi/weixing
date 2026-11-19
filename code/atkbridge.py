"""ATK Component 模式的 Python 封装。

所有和 ATK 打交道的代码都走这里，上层脚本（run.py、以后的算法代码）只调用下面这些方法：

    atk = ATK(out_dir="results/xxx/raw")    # 启动引擎
    atk.load("scenes/AccessExample.atk")     # 载入场景
    atk.objects()                            # 列出场景里的对象
    atk.set_orbit("卫星1", epoch=..., sma_m=..., ecc=..., inc_deg=..., raan_deg=..., argp_deg=..., ta_deg=...)
    atk.access("卫星1", "卫星2", step_s=60)  # 计算可见窗口，返回 [{start, stop, duration_s}, ...]
    atk.report("卫星1", "J2000 Position Velocity", start, stop)  # 导出 ATK 数据报告，返回文件路径
    atk.save(path) / atk.close()

接口依据 report/vendor_samples 里的官方示例和 SWIG 接口定义。
"""

import math
import os
import sys
from datetime import datetime
from pathlib import Path

TIME_FMT = "%d %b %Y %H:%M:%S.%f"  # ATK 时间字符串格式，例如 "5 Nov 2022 00:00:00.000"


def _import_atk(atk_home):
    comp = Path(atk_home) / "IntegratingWithATK" / "component" / "Python"
    if not comp.exists():
        raise RuntimeError(f"找不到 Component 目录：{comp}，请检查 ATK_HOME")
    for d in (atk_home, comp):
        os.add_dll_directory(str(d))
    os.environ["PATH"] = f"{comp}{os.pathsep}{atk_home}{os.pathsep}{os.environ.get('PATH', '')}"
    if str(comp) not in sys.path:
        sys.path.insert(0, str(comp))
    import ATKComponentPythonModule as m  # noqa: E402
    return m


def parse_time(s):
    return datetime.strptime(" ".join(s.split()), TIME_FMT)


def true_to_mean_anomaly(ta_deg, ecc):
    ta = math.radians(ta_deg)
    E = 2 * math.atan2(math.sqrt(1 - ecc) * math.sin(ta / 2), math.sqrt(1 + ecc) * math.cos(ta / 2))
    return math.degrees(E - ecc * math.sin(E)) % 360


class ATK:
    """一个进程里只建一个 ATK 实例。"""

    def __init__(self, atk_home=None, out_dir="results/raw"):
        atk_home = atk_home or os.environ.get("ATK_HOME")
        if not atk_home:
            raise RuntimeError("请设置环境变量 ATK_HOME")
        self.home = Path(atk_home).resolve()
        self.out_dir = Path(out_dir).resolve()
        self.out_dir.mkdir(parents=True, exist_ok=True)
        cwd = os.getcwd()
        os.chdir(self.home)  # 引擎按工作目录找 AstroData
        self.m = _import_atk(self.home)
        self.m.SetSaveFileBasePath(str(self.out_dir))
        self.m.SetCallCodeType("python")
        self.root = self.m.IAtkObjectRoot()
        self._cwd = cwd
        self.scenario_loaded = False

    # ---------- 路径 ----------
    def _abs(self, p):
        """相对路径按调用方的工作目录解析（ATK 内部会把工作目录切到安装目录）。"""
        p = Path(os.path.expandvars(str(p)))
        return p if p.is_absolute() else (Path(self._cwd) / p).resolve()

    # ---------- 场景 ----------
    def load(self, path):
        if self.scenario_loaded:
            self.close()
        p = self._abs(path)
        if not p.exists():
            raise FileNotFoundError(p)
        self.root.LoadScenario(str(p))
        self.scenario_loaded = True
        return self.scenario()

    def new(self, name, start, stop):
        if self.scenario_loaded:
            self.close()
        sc = self.root.GetChildren().New(self.m.eScenario, name)
        sc.SetTimePeriod(start, stop)
        self.scenario_loaded = True
        return sc

    def scenario(self):
        return self.root.GetCurrentScenario()

    def time_period(self):
        sc = self.scenario()
        return sc.GetStartTime(), sc.GetStopTime()

    def save(self, path=None):
        if path:
            self.root.SaveScenario(str(self._abs(path)))
        else:
            self.root.SaveScenario()

    def close(self):
        if self.scenario_loaded:
            self.root.CloseScenario()
            self.scenario_loaded = False

    # ---------- 对象 ----------
    def objects(self):
        """返回场景内全部对象 [(名字, 类型, 路径)]。"""
        res = []

        def walk(o):
            kids = o.GetChildren()
            for i in range(kids.GetCount()):
                c = kids.Item(i)
                res.append((c.GetInstanceName(), type(c).__name__, c.GetPath()))
                if c.GetHasChildren():
                    walk(c)

        walk(self.scenario())
        return res

    def find(self, name):
        """按实例名找对象（卫星名可以是中文）。"""
        found = []

        def walk(o):
            kids = o.GetChildren()
            for i in range(kids.GetCount()):
                c = kids.Item(i)
                if c.GetInstanceName() == name:
                    found.append(c)
                if c.GetHasChildren():
                    walk(c)

        walk(self.scenario())
        if not found:
            names = ", ".join(n for n, _, _ in self.objects())
            raise KeyError(f"场景里没有名为 {name} 的对象。现有对象：{names}")
        return found[0]

    # ---------- 轨道 ----------
    def propagator_type(self, sat_name):
        sat = self.find(sat_name)
        t = sat.GetPropagatorType()
        for k in dir(self.m):
            if k.startswith("ePropagator") and getattr(self.m, k) == t:
                return k
        return str(t)

    def set_orbit(self, sat_name, epoch, sma_m, ecc, inc_deg, raan_deg, argp_deg, ta_deg):
        """设置卫星初始轨道根数（J2000，单位 m 和度）并重新计算星历。

        支持两类预报器：机动规划（Astromaster/MCS）改初始段；二体/J2/J4 改初始状态。
        """
        sat = self.find(sat_name)
        ptype = self.propagator_type(sat_name)
        prop = sat.GetPropagator()
        if ptype == "ePropagatorAstromaster":
            seg = prop.GetMainSequence().Item(0)
            if seg.GetType() != self.m.eVASegmentTypeInitialState:
                raise RuntimeError("机动规划第一段不是初始段，无法设置初始轨道")
            seg.SetOrbitEpoch(epoch)
            seg.SetElementType(self.m.eVAElementTypeKeplerian)
            el = seg.GetElement()
            el.SetSemiMajorAxis(sma_m)
            el.SetEccentricity(ecc)
            el.SetInclination(inc_deg)
            el.SetRAAN(raan_deg)
            el.SetArgOfPeriapsis(argp_deg)
            el.SetTrueAnomaly(ta_deg)
            prop.RunMCS()
        elif ptype in ("ePropagatorTwoBody", "ePropagatorJ2Perturbation", "ePropagatorJ4Perturbation"):
            st = prop.GetInitialState().GetRepresentation()
            st.SetEpoch(epoch)
            st.AssignClassical(self.m.eCoordinateSystemJ2000, sma_m, ecc, inc_deg, argp_deg, raan_deg,
                               true_to_mean_anomaly(ta_deg, ecc))
            self.root.GetAnimation().Reset()
        else:
            raise NotImplementedError(f"{sat_name} 的预报器是 {ptype}，暂不支持修改初始轨道")
        return ptype

    # ---------- 可见性 ----------
    def access(self, from_name, to_name, start=None, stop=None, step_s=60.0, use_ltd=False):
        """计算两个对象之间的可见窗口，返回 [{start, stop, duration_s, raw}]。"""
        a, b = self.find(from_name), self.find(to_name)
        self.root.GetAnimation().Reset()  # 先计算星历
        acc = a.GetAccessByObject(b)
        if start is None or stop is None:
            start, stop = self.time_period()
        acc.SetAccessTimePeriod(start, stop)
        acc.SetTimeStep(float(step_s))
        acc.SetUseLTD(bool(use_ltd))
        acc.ComputeAccess()
        rows = []
        for r in acc.ComputedAccessIntervalTimes():
            r = list(r)
            item = {"start": r[0] if r else "", "stop": r[1] if len(r) > 1 else "", "duration_s": "", "raw": "|".join(r)}
            try:
                item["duration_s"] = round((parse_time(item["stop"]) - parse_time(item["start"])).total_seconds(), 3)
            except (ValueError, IndexError):
                pass
            rows.append(item)
        return rows

    # ---------- 报告 ----------
    def report(self, obj_name, report_name, start=None, stop=None):
        """导出 ATK 内置数据报告（如 "J2000 Position Velocity"），返回文件路径。"""
        if start is None or stop is None:
            start, stop = self.time_period()
        self.root.GetAnimation().Reset()
        return self.root.OutputDataReport(self.find(obj_name), report_name, start, stop)

    def position_j2000(self, obj_name, time_str):
        """返回某时刻 J2000 位置速度 (pos[3], vel[3])，单位 m、m/s。"""
        pos, vel = self.m.vector_double(), self.m.vector_double()
        self.root.OutputPosVelJ2000(self.find(obj_name), time_str, pos, vel)
        return list(pos), list(vel)
