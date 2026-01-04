# High-Performance C++ Boids Simulation

### Overview
This project is a high-density agent simulation built in Modern C++ (C++17/20) using the SFML graphics library. It serves as a technical showcase for **spatial partitioning**, **multi-core concurrency**, and **graphics pipeline optimization**. 

![Boids Simulation](https://github.com/anderson-ryan352/cpp-high-performance-boids/releases/download/v1.0.0/boidsanimation.gif)

### The Performance Benchmark
By migrating from a naive $O(n^2)$ proximity check to a specialized **Quadtree Spatial Partitioning** architecture, I achieved a **200x increase in agent throughput**:

*   **Baseline:** 50 Agents (Single-threaded, $O(n^2)$)
*   **Optimized:** **10,000 Agents @ 400+ FPS** (Multithreaded, $O(n \log n)$)

### Technical Deep Dive

#### 1. Spatial Partitioning (Quadtree)
Implemented a custom Quadtree algorithm to manage agent distribution. This reduces the complexity of neighborhood searches from quadratic to logarithmic time. By localizing "awareness" to nearby quadrants, the simulation avoids billions of unnecessary distance calculations per frame.

#### 2. Multi-core Concurrency (Intel TBB)
Utilized **C++17 Parallel Execution Policies** backed by the **Intel TBB** engine. By decoupling the simulation update from the rendering thread, the workload is distributed across all available CPU cores, resulting in a 400% performance increase over single-threaded execution.

#### 3. Graphics & Math Optimization
*   **Vertex Batching:** Eliminated the "Draw Call Bottleneck" by migrating from individual object rendering to a single **sf::VertexArray**. This allows the entire 10,000-agent state to be submitted to the GPU in a single command.
*   **Trigonometry-Free Updates:** Optimized the "hot loop" by replacing expensive transcendental functions (`sin`, `cos`, `atan2`) with **Vector Basis Transformations**, deriving triangle orientations directly from normalized velocity vectors.

#### 4. Modern C++ & Memory Safety
*   **RAII & Smart Pointers:** Implemented `std::unique_ptr` for all recursive tree nodes to ensure zero memory leaks and automated resource management.
*   **Heap Churn Reduction:** Minimized runtime allocations by utilizing persistent vectors and pre-allocating memory for neighborhood queries.

### Technology Stack
*   **Language:** C++17 / C++20
*   **Compiler:** GCC 15.2 (UCRT64)
*   **Build System:** Modern CMake (utilizing `FetchContent` for automated dependency resolution)
*   **Graphics:** SFML (Simple and Fast Multimedia Library)
*   **Concurrency:** Intel TBB (Threading Building Blocks)

### Installation & Build

This project utilizes a portable CMake pipeline. No manual library installation is required.

1.  **Clone the Repo:** `git clone https://github.com/anderson-ryan352/cpp-high-performance-boids.git`
2.  **Configure:** 
    ```bash
    mkdir build && cd build
    cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
    ```
3.  **Build:** 
    ```bash
    cmake --build .
    ```
4.  **Run:** `./Boids.exe`

---

### Reflection & Iteration
The primary challenge of this project was identifying the shift from a **CPU-bound** algorithmic bottleneck (resolved by the Quadtree) to a **GPU-bound** draw-call bottleneck (resolved by Vertex Batching). The final result demonstrates a highly scalable system capable of maintaining near-linear performance even as agent density increases.
