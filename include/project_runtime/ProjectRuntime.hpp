#ifndef PROJECT_RUNTIME_PROJECT_RUNTIME_HPP_
#define PROJECT_RUNTIME_PROJECT_RUNTIME_HPP_

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace project_runtime {

enum class Severity {
  Info,
  Warning,
  Critical
};

std::string toString(Severity severity);

struct MetricSpec {
  std::string name;
  std::string units;
  double warningLimit{};
  bool lowerIsBetter{true};
};

struct RuntimeSample {
  std::uint32_t sequence{};
  double primaryValue{};
  double secondaryValue{};
  bool online{true};
  std::string source;
};

struct ProjectProfile {
  std::string id;
  std::string title;
  std::string summary;
  std::string evidenceTarget;
  std::vector<std::string> capabilities;
  MetricSpec primaryMetric;
  MetricSpec secondaryMetric;
  bool connectivityRequired{true};
};

struct ValidationIssue {
  Severity severity{Severity::Info};
  std::string code;
  std::string message;
};

struct RuntimeReport {
  bool accepted{};
  RuntimeSample sample;
  std::vector<ValidationIssue> issues;
  std::vector<std::string> trace;
  std::string telemetryPayload;
};

class ISampleSource {
 public:
  virtual ~ISampleSource() = default;
  virtual RuntimeSample read() = 0;
};

class IValidationRule {
 public:
  virtual ~IValidationRule() = default;
  virtual std::optional<ValidationIssue> evaluate(
      const ProjectProfile& profile,
      const RuntimeSample& sample) const = 0;
  virtual std::string name() const = 0;
};

class ITelemetryEncoder {
 public:
  virtual ~ITelemetryEncoder() = default;
  virtual std::string encode(
      const ProjectProfile& profile,
      const RuntimeSample& sample,
      const std::vector<ValidationIssue>& issues) const = 0;
};

class FixedSampleSource final : public ISampleSource {
 public:
  explicit FixedSampleSource(RuntimeSample sample);

  RuntimeSample read() override;

 private:
  RuntimeSample sample_;
};

class RequiredCapabilitiesRule final : public IValidationRule {
 public:
  std::optional<ValidationIssue> evaluate(
      const ProjectProfile& profile,
      const RuntimeSample& sample) const override;
  std::string name() const override;
};

class ConnectivityRule final : public IValidationRule {
 public:
  std::optional<ValidationIssue> evaluate(
      const ProjectProfile& profile,
      const RuntimeSample& sample) const override;
  std::string name() const override;
};

class MetricThresholdRule final : public IValidationRule {
 public:
  explicit MetricThresholdRule(bool primaryMetric);

  std::optional<ValidationIssue> evaluate(
      const ProjectProfile& profile,
      const RuntimeSample& sample) const override;
  std::string name() const override;

 private:
  bool primaryMetric_{};
};

class CompositeValidator final {
 public:
  void add(std::unique_ptr<IValidationRule> rule);
  std::vector<ValidationIssue> evaluate(
      const ProjectProfile& profile,
      const RuntimeSample& sample) const;
  std::vector<std::string> ruleNames() const;

 private:
  std::vector<std::unique_ptr<IValidationRule>> rules_;
};

class JsonTelemetryEncoder final : public ITelemetryEncoder {
 public:
  std::string encode(
      const ProjectProfile& profile,
      const RuntimeSample& sample,
      const std::vector<ValidationIssue>& issues) const override;
};

class ProjectRuntime final {
 public:
  ProjectRuntime(ProjectProfile profile,
                 ISampleSource& sampleSource,
                 CompositeValidator validator,
                 const ITelemetryEncoder& telemetryEncoder);

  RuntimeReport runOnce();

 private:
  ProjectProfile profile_;
  ISampleSource& sampleSource_;
  CompositeValidator validator_;
  const ITelemetryEncoder& telemetryEncoder_;
};

class TextReportWriter final {
 public:
  explicit TextReportWriter(std::ostream& stream);
  void write(const ProjectProfile& profile, const RuntimeReport& report) const;

 private:
  std::ostream& stream_;
};

ProjectProfile demoProfile();
RuntimeSample nominalSample();
RuntimeSample unsafeSample();
RuntimeSample offlineSample();
CompositeValidator defaultValidator();
RuntimeReport runScenario(const std::string& mode);

}  // namespace project_runtime

#endif  // PROJECT_RUNTIME_PROJECT_RUNTIME_HPP_
