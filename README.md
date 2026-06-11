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

<!-- cpp17-solid-implementation:start -->
## C++17, Design Patterns, and SOLID Implementation

This repository includes a host-buildable C++17 implementation, not only documentation. The implementation applies:

- Strategy pattern for validation rules.
- Adapter interfaces for input samples and telemetry/reporting.
- Composite validation for combining safety and readiness checks.
- Facade orchestration through the project runtime class.
- SOLID boundaries between profile data, input acquisition, validation, telemetry encoding, and tests.
<!-- cpp17-solid-implementation:end -->

<!-- deep-architecture-links:start -->
## Deep Architecture and UML

- [Deep architecture](docs/deep-architecture.md)
- [Full UML Draw.io source](docs/diagrams/full-system-uml.drawio)
- [Full UML PNG export](docs/diagrams/full-system-uml.png)
<!-- deep-architecture-links:end -->

<!-- DESIGN_PACKAGE_START -->
## Detailed Design Package

This repository includes a structured design package for **TinyML Sensor Anomaly Detector**. The package captures the system boundary, runtime flow, hardware/software interfaces, validation strategy, and implementation roadmap.

| Artifact | Link |
| --- | --- |
| Design Index | [docs/design/README.md](docs/design/README.md) |
| System Design | [docs/design/system-design.md](docs/design/system-design.md) |
| Requirements | [docs/design/requirements.md](docs/design/requirements.md) |
| Interface Control | [docs/design/interface-control.md](docs/design/interface-control.md) |
| Runtime Design | [docs/design/runtime-design.md](docs/design/runtime-design.md) |
| Validation Plan | [docs/design/validation-plan.md](docs/design/validation-plan.md) |
| Implementation Roadmap | [docs/design/implementation-roadmap.md](docs/design/implementation-roadmap.md) |
| Draw.io UML | [docs/design/diagrams/system-design.drawio](docs/design/diagrams/system-design.drawio) |
| PNG UML | [docs/design/diagrams/system-design.png](docs/design/diagrams/system-design.png) |
<!-- DESIGN_PACKAGE_END -->
