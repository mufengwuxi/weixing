# 模型抽象（v1，对应当前各问的求解版本）

> 这份文档把三问的求解统一写成一套数学模型，可以作为论文第 2 节"模型建立"的底稿。三问共用一套记号：第一问是基本单元，第二问是基本单元的序列化，第三问再加上第二颗星的设计和任务分配。实现细节（网格步长、裕量、束宽）不在这里，见各问的求解文档。

## 1. 记号与动力学

观测星记为 $r_1, r_2$，目标集合记为 $\mathcal B = \{b_1,\dots,b_{30}\}$。初始历元 $t=0$ 对应 2027-01-01 00:00:00 UTC，所有矢量都在地心惯性系 ICRF 中表示。卫星状态记为 $\boldsymbol x = (\boldsymbol r, \boldsymbol v) \in \mathbb R^6$。在二体模型下，无机动段的演化由开普勒流 $\Phi$ 给出：

$$\boldsymbol x(t) = \Phi\big(\boldsymbol x(t_0),\, t - t_0\big), \qquad \ddot{\boldsymbol r} = -\mu \boldsymbol r / \|\boldsymbol r\|^3, \quad \mu = 398600.4418\ \mathrm{km^3/s^2}.$$

观测星的机动方案是一列脉冲 $\{(t_k, \Delta\boldsymbol v_k)\}_{k=1}^{K}$，满足 $t_1 < t_2 < \dots < t_K$。脉冲时刻位置连续，速度跳变：$\boldsymbol r(t_k^+) = \boldsymbol r(t_k^-)$，$\boldsymbol v(t_k^+) = \boldsymbol v(t_k^-) + \Delta\boldsymbol v_k$。于是观测星的轨迹 $\boldsymbol r_O(t)$ 由初始状态和脉冲序列完全确定。目标星不机动，轨迹为 $\boldsymbol r_b(t) = \Phi(\boldsymbol x_b(0), t)$ 的位置分量。太阳位置 $\boldsymbol r_\odot(t)$ 取 ERFA 几何星历，只用于几何判断，不产生加速度。

## 2. 观测可行性

对观测星 $O$ 和目标 $b$，在时刻 $t$ 定义相对矢量 $\boldsymbol\rho(t) = \boldsymbol r_O(t) - \boldsymbol r_b(t)$，以及三个几何量：

$$d(t) = \|\boldsymbol\rho\|, \qquad \alpha(t) = \angle\big(\boldsymbol r_\odot - \boldsymbol r_b,\ \boldsymbol\rho\big), \qquad \theta(t) = \angle\big(-\boldsymbol r_b,\ \boldsymbol\rho\big),$$

其中 $\angle(\boldsymbol a, \boldsymbol c) = \arccos\dfrac{\boldsymbol a\cdot\boldsymbol c}{\|\boldsymbol a\|\|\boldsymbol c\|} \in [0, \pi]$ 是有向夹角，不对点积取绝对值。瞬时可观测条件为

$$\mathcal G_b(t):\quad 10 \le d(t) \le 50\ \mathrm{km},\quad \alpha(t) \le 45^\circ,\quad \theta(t) \le 45^\circ .$$

一次有效观测是一个起始时刻 $s$，使得 $\mathcal G_b(t)$ 在整个区间 $[s, s+\tau]$ 上成立，其中 $\tau = 60$ s。这是一组半无限约束，求解时在时间网格上离散化，再用更细的网格认证。

由 $\alpha$ 和 $\theta$ 可以推出一条必要条件。两个角都以 $\boldsymbol\rho$ 为一边，由球面三角不等式，

$$\angle(\boldsymbol r_\odot - \boldsymbol r_b,\ -\boldsymbol r_b) \le \alpha + \theta \le 90^\circ .$$

也就是说，只有当目标处的"太阳方向"和"地心方向"夹角不超过 90° 时，目标才可能被观测。对地球同步轨道上的目标，这把可观测时段限制在每天目标位于地球背阳一侧的大约半天里。这个必要条件只依赖目标和太阳，与观测星无关，可以在求解之前预先筛掉不可能观测的时段。

## 3. 机动约束与目标函数

对每颗观测星 $j$，题面的机动约束为

$$\|\Delta\boldsymbol v_k\| \le 30\ \mathrm{m/s},\qquad t_{k+1} - t_k \ge 3600\ \mathrm s,\qquad J_j = \sum_k \|\Delta\boldsymbol v_k\| \le 300\ \mathrm{m/s},$$

并且每次有效观测之前（从上一次观测结束算起）最多安排 2 次脉冲，所有观测在任务时限 $T$ 内结束（第一问 $T = 48$ h，第二、三问 $T = 30$ d）。

设方案 $\mathcal P$ 成功观测的不同目标集合为 $S(\mathcal P)$。三问统一采用字典序目标：

$$\operatorname{lexmax}_{\mathcal P}\ \big(\,|S(\mathcal P)|,\ -\textstyle\sum_j J_j(\mathcal P)\,\big).$$

它等价于两级问题：先求 $N^* = \max_{\mathcal P} |S(\mathcal P)|$，再在 $|S(\mathcal P)| = N^*$ 的方案中最小化总 Δv。整个过程没有引入任何权重。

## 4. 第一问：单次转移观测（基本单元）

决策变量为 $\boldsymbol z = (t_1, t_2, \Delta\boldsymbol v_1, \Delta\boldsymbol v_2, s) \in \mathbb R^{9}$，模型为

$$\min_{\boldsymbol z}\ \|\Delta\boldsymbol v_1\| + \|\Delta\boldsymbol v_2\| \quad \text{s.t.}\quad 0 \le t_1,\ t_2 - t_1 \ge 3600,\ t_2 \le s,\ s + \tau \le T,\ \|\Delta\boldsymbol v_i\| \le 30,\ \mathcal G_{b_1}(t)\ \forall t \in [s, s+\tau].$$

它是一个带半无限约束的非线性规划。目标函数在 $\Delta\boldsymbol v_i = 0$ 处不可微，所以单脉冲和零脉冲的方案都包含在这个模型里。求解分两步：先用两脉冲瞄准（让观测中点的相对位置落到可行区内的一个点）生成多起点初值，再在完整的连续观测约束下用 SLSQP 精修。我们把这个基本单元记作 $\mathrm{LEG}(\boldsymbol x_0, t_0, t_{\min}, b, \hat s)$：观测星在 $t_0$ 的状态为 $\boldsymbol x_0$，第一次脉冲不早于 $t_{\min}$，在 $\hat s$ 附近观测目标 $b$，返回最小 Δv 和对应的转移。

## 5. 第二问：单星巡游（序列化）

方案由一个访问序列 $\pi = (b_{\pi_1}, \dots, b_{\pi_m})$ 和每段的 LEG 解组成。第 $k$ 段从第 $k-1$ 次观测结束时的状态出发，第一次脉冲时刻必须同时晚于上一次观测结束、并且与上一次脉冲相隔至少 1 h，即

$$t_{\min}^{(k)} = \max\big(s_{k-1} + \tau,\ t_2^{(k-1)} + 3600\big).$$

这是一个时间相关的定向越野问题（time-dependent orienteering）：先选目标、再定顺序，每条"边"的代价由一个非线性规划给出，并且依赖到达时刻和到达状态。直接枚举不可行，我们分两层求解。

**上层（排日程）** 只用解析模型估计每段代价。第一，观测机会离散化：结合第 2 节的必要条件，目标通常在穿过观测星轨道面的时刻附近、并且处在背阳时段时才可观测，所以每颗目标大约每天有一次机会。第二，漂移相位模型：观测星与 GEO 的半长轴差为 $\Delta a$ 时，相对经度漂移率为 $\dot\lambda = -\tfrac32 n\,\Delta a / a$，而改变 $\Delta a$ 需要的切向速度增量为 $\Delta v = \tfrac{v}{2a}\Delta a$。合起来得到

$$\Delta v = \frac{v}{3n}\,\big|\Delta\dot\lambda\big| \approx 2.84\ \mathrm{m/s}\ \text{（每 1°/天）}.$$

于是每段代价可以近似为"把漂移率调到恰好在机会时刻赶到目标"所需的 $k\,|\Delta\dot\lambda|$，其中 $k \approx 2.85$ m/s/(°/天)。在这个代价下，用束搜索按时间顺序扩展序列，按字典序（已访问数、累计估计 Δv）保留前 $W$ 个部分方案。

**下层（精确实现）** 沿上层给出的序列，逐段调用 LEG，在真实二体动力学和完整观测约束下求精确脉冲。如果某颗目标在相邻观测的同一次飞越里也满足观测条件，就把两次观测合进同一段转移，第二颗目标不再需要单独机动（例如 b19 与 b20）。

## 6. 第三问：双星协同（设计 + 分配）

决策变量分三部分：r2 的初始轨道根数 $\boldsymbol\sigma_2 = (a_2, e_2, i_2, \Omega_2, \omega_2, M_2)$，目标的分配 $\mathcal B = \mathcal A_1 \cup \mathcal A_2$，以及两颗星各自的第二问式方案。约束是

$$|a_2 - a_{\mathrm{GEO}}| \le \delta a,\qquad e_2 < 0.001,\qquad J_1 \le 300,\quad J_2 \le 300\ \mathrm{m/s}.$$

目标仍为 $\operatorname{lexmax}(|S_1 \cup S_2|,\ -(J_1 + J_2))$。给定 $\boldsymbol\sigma_2$ 和分配之后，问题分解成两个互相独立的第二问子问题，因为题面对两颗星之间没有耦合约束。

**初始轨道与首脉冲等价。** 如果 r2 的第一次脉冲发生在 $t=0$，那么"初始轨道 $\boldsymbol\sigma_2$ 加上 $t=0$ 的脉冲"与"初始轨道 $\boldsymbol\sigma_2'$、没有这次脉冲"完全等价，只要 $\boldsymbol\sigma_2'$ 仍满足 δa 和偏心率约束。所以可以把首脉冲吸收进初始轨道设计，它不计入 Δv。

**δa 是参数而不是常数。** 记 $J^*(\delta a)$ 为给定 δa 时的最优总 Δv。δa 增大时，r2 的可行初始轨道集合只会扩大，所以 $J^*(\delta a)$ 关于 δa 单调不增。δa 的取值通过分析这条曲线来确定，有两条依据。

- **漂移需求（下界）。** r2 要在 $T$ 内凭自然漂移跨过它负责的经度范围 $\Delta\Lambda$，需要 $|\Delta a| \gtrsim \frac{2a}{3n}\frac{\Delta\Lambda}{T}$。东侧目标 $\Delta\Lambda \approx 15^\circ$，算出约 40 km。
- **观测几何（上界）。** 共面近圆轨道之间的径向间距在 $|\Delta a|$ 附近振荡，而观测距离不能超过 50 km。目标的半长轴都在 GEO 附近几千米以内，所以初始 $|\Delta a|$ 超过约 55 km 时，r2 必须先机动才可能观测，再增大 δa 已不能扩大观测能力。

曲线在这两个量之间进入饱和段，推荐值取在饱和段的起点。

**分配。** 当前版本按 r1 的经度把目标分成东西两组，各自单向扫描。这样两颗星都不用掉头，扫描跨度减半，所需漂移率和 Δv 也就大致减半。

## 7. 求解框架小结

三问的求解可以概括为一句话：第一问的 LEG 是精确求解单元；第二问在它上面加了一层"机会离散化 + 漂移代价 + 束搜索"的排程；第三问在第二问外面再套一层"r2 轨道设计 + 目标分配"的外层搜索，并对 δa 做参数分析。每一层都只负责提出候选，最终方案都经过精确动力学、0.05 s 网格认证，以及 ATK 独立积分复核。
