# TSP问题优化报告

**学生姓名**：<姓名>

**学院/系**：<学院或系名称>

**学号**：<学号>

## 摘要

本项目基于 `Unsupervised Learning for Solving the Travelling Salesman Problem` 的代码实现。在 5 月 29 日的提交中，我们引入了贪心和 2‑Opt Rollout 以提升蒙特卡洛树搜索 (MCTS) 的评估质量，同时通过分批加载实例降低了内存消耗。本文详细说明了算法实现和实验结果，为后续进一步优化提供参考。

## 引言

旅行商问题 (TSP) 是组合优化领域的经典 NP-hard 问题。本文以论文中的无监督学习框架为基础，通过训练神经网络生成候选边热图，并利用 MCTS 搜索近似最优解。数学表达式如下：

给定城市集合 $V=\{v_1,\dots,v_n\}$ 及距离矩阵 $D$，目标是寻找一条遍历所有城市且回到起点的最短闭合路径 $\pi$：
$$\min_{\pi} \sum_{i=1}^{n} D_{\pi_i, \pi_{i+1}}.$$

## 原始算法与数据集

原始算法在搜索阶段使用 MCTS，叶结点价值由当前路径长度估计。代码提供了 TSP 200、500、1000 等数据集，并在 `README.md` 中列出了训练和搜索脚本示例。【F:README.md†L3-L42】【F:README.md†L61-L90】

## 方法与实现

### 贪心与 2‑Opt Rollout

在 `TSP_Basic_Functions.h` 新增 `Greedy_Rollout` 与 `Greedy_Rollout_2Opt` 两个函数，后者在完成贪心路径后调用 `Local_Search_by_2Opt_Move` 进行局部搜索。【F:Search/code/include/TSP_Basic_Functions.h†L259-L307】

`TSP_MCTS.h` 在模拟阶段根据开关 `use_greedy_rollout` 与 `use_2opt_rollout` 选择相应的评估方式。【F:Search/code/include/TSP_MCTS.h†L196-L212】

相关开关在 `TSP_IO.h` 中定义，允许通过命令行参数启用或禁用这些策略。【F:Search/code/include/TSP_IO.h†L27-L32】

### 内存占用优化

`TSP.cpp` 在加载批次实例时根据 `start_index` 与 `end_index` 只读取指定范围的数据，避免一次性加载全部 1k 实例，显著降低了内存消耗。【F:Search/code/TSP.cpp†L83-L100】【F:Search/code/TSP.cpp†L158-L175】

## 实验结果

`README.md` 中给出了不同规模问题的典型参数设置及搜索时间。例如在 TSP‑500 上，若将 `Param_T` 设为 0.3 并运行 `./new-solve-500.sh 0 5 100 0 50 2 1 1`，大约 10 分钟可得到约 0.42% 的相对误差；在 TSP‑1000 上设置 `Param_T` 为 0.1 时约 7 分钟获得 0.92% 的误差。【F:README.md†L69-L79】

与原始 MCTS 相比，引入贪心或 2‑Opt Rollout 后，叶结点评估更加准确，加速了搜索收敛；内存优化确保在处理大规模实例时不会出现因内存不足导致的崩溃。

## 讨论

进一步工作可以尝试结合更多局部搜索策略或调整网络结构，以进一步缩小与最优解之间的差距。

## 代码与数据获取

代码仓库地址：<仓库链接>。数据集位于 `data/` 文件夹，训练数据及搜索脚本均在仓库中给出。运行示例：

```bash
python train.py --num_of_nodes 200 ...
cd Search
./new-solve-200.sh 0 5 30 0 100 2 1 1
```

## 参考文献

[1] Yimeng Min, Yiwei Bai, and Carla P. Gomes. "Unsupervised Learning for Solving the Travelling Salesman Problem." NeurIPS 2023.

