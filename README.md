# 🌳 Optimal Binary Search Tree (OBST)

This project implements the **Optimal Binary Search Tree** algorithm using **Dynamic Programming** in C++. OBST is used to build a binary search tree that minimizes the expected search cost, based on known access frequencies for keys and gaps (unsuccessful searches).

---

## 📖 What is OBST?

In scenarios where each key has a known probability of being searched, a naive BST might not be optimal. OBST builds a **statistically optimized tree** that:
- Minimizes the **average number of comparisons**
- Accounts for both **successful** and **unsuccessful** search probabilities

---

## 📌 Features

- Input: Sorted keys with probabilities `p[i]` (success) and `q[i]` (failure)
- Computes:
  - `e[i][j]`: Expected search cost
  - `w[i][j]`: Cumulative probability
  - `root[i][j]`: Root table for tree reconstruction
- Outputs cost and root matrices

---

## 🧪 Sample Input

```text
Keys:        A  B  C
p:          0.15  0.10  0.05
q:          0.05  0.10  0.05  0.05
