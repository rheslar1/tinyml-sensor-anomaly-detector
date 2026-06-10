#include <array>
#include <cassert>
#include <string_view>

class IReadinessRule {
 public:
  virtual ~IReadinessRule() = default;
  virtual bool passes(std::string_view evidenceTarget) const = 0;
};

class RequiredEvidenceRule final : public IReadinessRule {
 public:
  bool passes(std::string_view evidenceTarget) const override {
    return !evidenceTarget.empty();
  }
};

struct ProjectProfile {
  std::string_view title;
  std::string_view summary;
  std::string_view evidenceTarget;
  std::array<std::string_view, 9> tags;
};

constexpr ProjectProfile profile{
  "TinyML Sensor Anomaly Detector",
  "Quantized sensor anomaly model running on an ARM Cortex-M core with measured RAM, flash, latency, and power budget constraints.",
  "Embedded ML optimization, memory budgeting, fixed-point inference, and useful intelligence at the device edge.",
  {
    "C++17",
    "C++ Design Patterns",
    "SOLID",
    "Cortex-M",
    "TensorFlow Lite Micro",
    "Quantization",
    "CMSIS-NN",
    "Ring buffers",
    "Edge inference"
  }
};

int main() {
  const RequiredEvidenceRule rule;
  assert(!profile.title.empty());
  assert(!profile.summary.empty());
  assert(rule.passes(profile.evidenceTarget));
  assert(profile.tags[0] == "C++17");
  return 0;
}
