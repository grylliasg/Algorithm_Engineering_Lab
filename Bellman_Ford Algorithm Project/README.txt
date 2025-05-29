# Bellman-Ford Implementation and Experimental Evaluation

This repository contains my solution to **Programming Exercise 2** of the course *Algorithm Engineering* taught by Prof. Christos D. Zaroliagis at the University of Patras.

## 🧠 Project Description

The goal of the assignment was to implement the **Bellman-Ford algorithm** for solving the **single-source shortest path problem** on a directed graph with integer edge weights. The implementation and experimental evaluation were done using:

- **Custom implementation using Boost Graph Library**
- **Built-in `bellman_ford_shortest_paths()` function from Boost**
- **LEDA's `BELLMAN_FORD_B_T` implementation**

The assignment focused both on algorithm design and **performance comparison** between the three implementations.

## 🔍 Algorithm Details

The custom implementation follows the iterative version of Bellman-Ford as taught in a Lecture of the course. It maintains a set `U` of active vertices and updates the distances through edge relaxations.

Special attention was given to:
- The representation of **+∞** and **nil**
- Correct handling and **detection of negative-weight cycles**
- Memory-efficient graph data structures

## 🔬 Experimental Evaluation

The implementations were benchmarked on two types of graph families:

1. **Random graphs** with:
   - |V| ∈ {1000, 4000, 8000}
   - |E| = 20·n·log(n)
   - Edge weights in [−100, 10000]
   - Random source node `s`
   - Weak connectivity ensured

2. **Grid-based graphs** with:
   - n × n grids, where n ∈ {100, 200, 300}
   - Complex directional and weight patterns as described in the assignment
   - Special structure with guaranteed negative-weight cycle for validation

For each experiment:
- Execution time was measured (mean of 5 runs)
- Existence of negative cycles was reported
- Vertices were classified into sets V+, V−, and Vf as per the algorithm's outcome

## 📁 Repository Structure

