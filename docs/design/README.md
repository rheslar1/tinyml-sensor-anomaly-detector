# TinyML Sensor Anomaly Detector Design Package

## Purpose

Quantized sensor anomaly model running on an ARM Cortex-M core with measured RAM, flash, latency, and power budget constraints.

This package defines the project as an implementation-ready embedded system. It covers system architecture, requirements, interface boundaries, runtime design, validation evidence, and phased delivery.

## Project Profile

| Field | Value |
| --- | --- |
| Repository | `rheslar1/tinyml-sensor-anomaly-detector` |
| Primary stack | C++17, C++ Design Patterns, SOLID, Cortex-M, TensorFlow Lite Micro, Quantization, CMSIS-NN, Ring buffers, Edge inference |
| Review proof point | Embedded ML optimization, memory budgeting, fixed-point inference, and useful intelligence at the device edge. |

## Artifacts

- [System Design](system-design.md)
- [Requirements](requirements.md)
- [Interface Control](interface-control.md)
- [Runtime Design](runtime-design.md)
- [Validation Plan](validation-plan.md)
- [Implementation Roadmap](implementation-roadmap.md)
- [Draw.io UML](diagrams/system-design.drawio)
- [PNG UML](diagrams/system-design.png)
