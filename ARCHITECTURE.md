# TinyML Sensor Anomaly Detector Architecture

## Goal

Embedded ML optimization, memory budgeting, fixed-point inference, and useful intelligence at the device edge.

## Runtime Shape

1. Hardware or simulator input is sampled through a narrow driver boundary.
2. A control profile normalizes state into a deterministic decision surface.
3. Safety checks reject unsafe commands before they reach the actuator, transport, or update path.
4. Telemetry and validation logs are emitted for repeatable review.

## C++17 Design Shape

- `ProjectProfile` owns project identity and evidence text.
- `IReadinessRule` defines a narrow strategy interface for scaffold readiness checks.
- `RequiredEvidenceRule` is a concrete strategy used by the starter executable and tests.
- The scaffold keeps documentation, executable behavior, and validation concerns separated.

## SOLID Notes

- Single Responsibility: profile data and readiness rules are separate.
- Open/Closed: new readiness rules can be added without changing the profile object.
- Liskov Substitution: any `IReadinessRule` can replace the default rule.
- Interface Segregation: the readiness interface exposes only one focused operation.
- Dependency Inversion: the executable consumes the readiness rule abstraction.

## Boundaries

- `src/`: native starter implementation and future device-specific drivers.
- `docs/`: validation plans, timing notes, hardware captures, and acceptance evidence.
- `tests/`: repo-level smoke tests and future simulator or host-side unit tests.
- `.github/workflows/`: CI entry point for build and validation evidence.

## Validation Plan

- Build the host starter with CMake.
- Run the executable and confirm the reported profile matches this repository.
- Run CTest to validate the C++17 readiness scaffold.
- Add hardware-specific logs after the first board, simulator, or bus test.
- Capture CI, terminal, and hardware evidence for the portfolio detail page.

## Expansion Notes

Replace the starter profile with the project-specific implementation slice while preserving the same review boundaries: build, tests, architecture notes, validation logs, and screenshots.

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
