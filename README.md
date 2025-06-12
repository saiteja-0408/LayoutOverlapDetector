# LayoutOverlapDetector

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#build-and-run)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

A Qt 6 / QML demo application that loads rectangle layouts from JSON, runs a multi‐threaded sweep‐line algorithm to detect overlaps in O(n·log n) time, and highlights overlapping shapes in real time. This project mirrors the core geometry‐checking engine in EDA sign‐off tools like **Siemens Calibre DESIGNrev**, distilled into a lightweight, easy‐to‐understand proof of concept.

---

## 🎯 Table of Contents

1. [Why This Project?](#why-this-project)
2. [Key Features](#key-features)
3. [Prerequisites](#prerequisites)
4. [Installation & Build](#installation--build)
5. [Usage](#usage)
6. [Project Structure](#project-structure)
7. [Detailed Architecture](#detailed-architecture)

   * [Data Input (JSON)](#data-input-json)
   * [Sweep‐Line Algorithm](#sweep-line-algorithm)
   * [Threading Model](#threading-model)
   * [Qt Quick UI](#qt-quick-ui)
8. [Example Layouts](#example-layouts)
9. [Extending & Next Steps](#extending--next-steps)
10. [References & Licensing](#references--licensing)

---

## Why This Project?

In advanced integrated‐circuit (IC) design flows, thousands to millions of polygons must be verified for **no illegal overlaps** (and minimum spacing). A brute‐force O(n²) check quickly becomes impractical. The **sweep‐line** technique—sorting “enter”/“exit” events and maintaining an active interval tree—reduces runtime to O(n log n).

This demo:

* **Illustrates** the sweep‐line overlap detection on simple rectangles.
* **Shows** best practices in Qt 6, CMake, C++17, and multi‐threaded UI design.
* **Simulates** a micro‐DRC engine akin to those in Siemens Calibre DESIGNrev.

Even if you’ve never touched EDA or Qt, by the end you’ll understand how to:

1. Load and parse JSON layout data.
2. Run a background computational geometry pass.
3. Surface results in a modern, GPU‐accelerated GUI.

---

## Key Features

* **High‐performance overlap detection** using a sweep‐line (O(n log n)).
* **Background processing** with `QThread` to keep the UI fluid.
* **Data‐driven UI** via `QAbstractListModel` and QML `Repeater`.
* **One‐step setup** script for macOS/Linux.
* **Scriptable test cases**: generate random or hand‐crafted JSON scenarios.

---

## Prerequisites

Ensure you have the following installed:

| Component                 | macOS (Homebrew)                    | Ubuntu / Debian                                     |
| ------------------------- | ----------------------------------- | --------------------------------------------------- |
| C++ toolchain             | Xcode CLI tools (clang, make, etc.) | `build-essential`                                   |
| CMake (≥ 3.16)            | `brew install cmake`                | `sudo apt install cmake`                            |
| Qt 6 Base & Quick modules | `brew install qt6`                  | `sudo apt install qt6-base-dev qt6-declarative-dev` |
| nlohmann‐json             | `brew install nlohmann-json`        | `sudo apt install nlohmann-json3-dev`               |
| GoogleTest (for tests)    | `brew install googletest`           | `sudo apt install libgtest-dev`                     |
| Python 3 (for scripts)    | `brew install python3`              | `sudo apt install python3`                          |

---

## Installation & Build

**Clone the repo**:

```bash
git clone https://github.com/your‐org/LayoutOverlapDetector.git
cd LayoutOverlapDetector
```

**One‐step setup (macOS/Linux)**:

```bash
./setup.sh
```

This script will:

1. Install missing dependencies (if possible).
2. Generate a sample layout (`scripts/generate_layout.py`).
3. Configure and build under `build/`.
4. Launch the app.

---

**Manual build**:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./LayoutOverlapDetector   # runs sample_layout.json by default
```

---

## Usage

Once the window appears:

1. **View** the gray rectangles on the canvas.

2. Click **Detect Overlaps** → any overlapping rectangles turn **red**.

3. To try a different scenario, press **Esc** to close, then from the `build/` directory run:

   ```bash
   # load a custom JSON scenario
   ./LayoutOverlapDetector cluster.json
   ./LayoutOverlapDetector edge_touch.json
   ```

4. To generate a fresh random layout:

   ```bash
   scripts/generate_layout.py
   cd build
   cmake --build .
   ./LayoutOverlapDetector
   ```

---

## Project Structure

```
LayoutOverlapDetector/
├── CMakeLists.txt          # Build configuration
├── setup.sh                # One‐step install & run
├── include/
│   ├── Rectangle.h         # POD for rectangle data
│   ├── OverlapEngine.h     # Sweep‐line algorithm interface
│   ├── LayoutModel.h       # Qt model exposing rectangles to QML
│   └── WorkerThread.h      # Offloads compute to background thread
├── src/
│   ├── Rectangle.cpp
│   ├── OverlapEngine.cpp
│   ├── LayoutModel.cpp
│   ├── WorkerThread.cpp
│   └── main.cpp            # App startup, JSON loading, QML wiring
├── resources/
│   ├── qml/
│   │   └── MainView.qml    # QML UI: Repeater + Button
│   ├── layouts/            # Pre‐built test scenarios
│   │   ├── sample_layout.json
│   │   ├── no_overlap.json
│   │   ├── single_overlap.json
│   │   ├── nested_overlap.json
│   │   ├── edge_touch.json
│   │   └── cluster.json
│   └── resources.qrc       # Embeds `MainView.qml` into the binary
├── scripts/
│   └── generate_layout.py  # Generates random JSON layouts
├── build/                  # Out‐of‐source build (ignored in Git)
└── LICENSE
```

---

## Detailed Architecture

### Data Input (JSON)

* **Why JSON?** Human‐readable, easy to generate via scripts or export from other tools.
* **Format**:

  ```json
  [
    { "id": 0, "x": 10, "y": 10, "w": 80, "h": 80 },
    { "id": 1, "x":200, "y": 20, "w":100, "h": 60 }
  ]
  ```
* **Parser**: [nlohmann/json](https://github.com/nlohmann/json) (header‐only, C++17).

### Sweep‐Line Algorithm

1. **Event creation**:

   * Each rectangle generates two events:

     * **Enter event** at `x`
     * **Exit event** at `x + w`
2. **Sort events** by `x` (if tie, entries before exits) → O(n log n).
3. **Active set**: a `std::set<int>` keyed by rectangle Y‐coordinate for fast neighbor queries.
4. **Scan**:

   * On **enter**, check the new rectangle’s Y‐range against all active rectangles; mark both as overlaps when ranges intersect.
   * Insert the new rectangle into the active set.
   * On **exit**, remove it from the active set.
5. **Result**: Each rectangle’s `.overlaps` flag is `true` if it ever intersected a neighbor.

This approach is the backbone of high‐capacity, high‐performance geometry checks in industrial DRC tools.

### Threading Model

* **UI responsiveness** is critical.
* We wrap the sweep‐line call in **`WorkerThread`** (subclass of `QThread`).
* When “Detect Overlaps” is clicked:

  1. Instantiate `WorkerThread` with references to the data and model.
  2. Call `start()`.
  3. In `run()`, compute overlaps, then use `QMetaObject::invokeMethod(...)` to update the `LayoutModel` on the UI thread.
* This guarantees no data races and a smooth UI.

### Qt Quick UI

* **`MainView.qml`**:

  ```qml
  ApplicationWindow {
    width: 800; height: 600
    Repeater {
      model: layoutModel
      Rectangle {
        x:      rectX
        y:      rectY
        width:  rectW
        height: rectH
        color:  overlaps ? "red" : "#dddddd"
        border.color: "black"
      }
    }
    Button {
      text: "Detect Overlaps"
      onClicked: controller.onDetectClicked()
    }
  }
  ```
* **Model‐View**: `LayoutModel` exposes each rectangle’s properties (`rectX`, `rectY`, `rectW`, `rectH`, `overlaps`) as roles.
* **Styling**: Overlaps are instantly visible via a color change.

---

## Example Layouts

| Filename              | Description                           |
| --------------------- | ------------------------------------- |
| `no_overlap.json`     | All rectangles disjoint → no red.     |
| `single_overlap.json` | Exactly one pair overlaps → two reds. |
| `nested_overlap.json` | One shape entirely inside another.    |
| `edge_touch.json`     | Shared edge only → no red.            |
| `cluster.json`        | Small cluster with multiple overlaps. |

Switch scenarios at runtime:

```bash
cd build
./LayoutOverlapDetector cluster.json
```

---

## Extending & Next Steps

* **Real EDA data**: Import GDSII/LEF/DEF → generate JSON → visualize.
* **Pan & zoom**: Add `WheelHandler`/`PinchArea` in QML.
* **Benchmarking**: Plot runtime vs. rectangle count in CI badges.
* **Unit tests**: Add Googletest cases under `tests/`.
* **Packaging**:

  * macOS: `macdeployqt LayoutOverlapDetector.app`
  * Linux: `cpack -G DEB`

---

## References & Licensing

* **Sweep‐line algorithm**: Agarwal & Erickson, “Geometric Range Searching and its Relatives”
* **Qt 6**: [https://doc.qt.io/qt-6](https://doc.qt.io/qt-6)
* **nlohmann/json**: [https://github.com/nlohmann/json](https://github.com/nlohmann/json)
