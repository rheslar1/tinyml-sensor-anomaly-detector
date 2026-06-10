# TinyML Sensor Anomaly Detector

Quantized sensor anomaly model running on an ARM Cortex-M core with measured RAM, flash, latency, and power budget constraints.

## Portfolio Purpose

This repository is an Embedded Systems project scaffold for the Rheslar portfolio. It is designed to become a hardware-backed project with build output, validation logs, and reviewable implementation evidence.

All generated Embedded Systems repos are C++17-first and are framed around C++ design patterns and SOLID design principles.

## Stack

- C++17
- C++ Design Patterns
- SOLID
- Cortex-M
- TensorFlow Lite Micro
- Quantization
- CMSIS-NN
- Ring buffers
- Edge inference

## Quick Start

```bash
cmake -S . -B build
cmake --build build
./build/tinyml_sensor_anomaly_detector
ctest --test-dir build --output-on-failure
```

## Implementation Slices

- C++17 starter executable that exposes the project identity, stack, and validation target.
- Small strategy-style readiness check that keeps the scaffold aligned with C++ design patterns.
- Architecture document with control boundaries, data flow, safety assumptions, and evidence plan.
- CTest smoke test that keeps source, docs, and CI files present as the repo grows.
- GitHub Actions workflow for configure, build, executable smoke run, and repository validation.

## Evidence Target

Embedded ML optimization, memory budgeting, fixed-point inference, and useful intelligence at the device edge.

## Remote

Intended public repository: https://github.com/rheslar1/tinyml-sensor-anomaly-detector
