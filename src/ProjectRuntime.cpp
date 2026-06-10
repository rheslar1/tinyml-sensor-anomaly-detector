#include "project_runtime/ProjectRuntime.hpp"

#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace project_runtime {
namespace {

std::string jsonEscape(const std::string& value) {
  std::ostringstream escaped;
  for (const char item : value) {
    switch (item) {
      case '\\':
        escaped << "\\\\";
        break;
      case '"':
        escaped << "\\\"";
        break;
      case '\n':
        escaped << "\\n";
        break;
      case '\r':
        escaped << "\\r";
        break;
      case '\t':
        escaped << "\\t";
        break;
      default:
        escaped << item;
        break;
    }
  }
  return escaped.str();
}

std::string fixed2(const double value) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << value;
  return stream.str();
}

bool exceeds(const MetricSpec& spec, const double value) {
  return spec.lowerIsBetter ? value > spec.warningLimit : value < spec.warningLimit;
}

}  // namespace

std::string toString(const Severity severity) {
  switch (severity) {
    case Severity::Info:
      return "info";
    case Severity::Warning:
      return "warning";
    case Severity::Critical:
      return "critical";
  }
  return "unknown";
}

FixedSampleSource::FixedSampleSource(RuntimeSample sample) : sample_(std::move(sample)) {}

RuntimeSample FixedSampleSource::read() {
  return sample_;
}

std::optional<ValidationIssue> RequiredCapabilitiesRule::evaluate(
    const ProjectProfile& profile,
    const RuntimeSample&) const {
  if (profile.id.empty() || profile.title.empty() || profile.capabilities.empty()) {
    return ValidationIssue{Severity::Critical, "PROFILE_INCOMPLETE", "project profile is missing identity or capabilities"};
  }
  return std::nullopt;
}

std::string RequiredCapabilitiesRule::name() const {
  return "RequiredCapabilitiesRule";
}

std::optional<ValidationIssue> ConnectivityRule::evaluate(
    const ProjectProfile& profile,
    const RuntimeSample& sample) const {
  if (profile.connectivityRequired && !sample.online) {
    return ValidationIssue{Severity::Critical, "CONNECTIVITY_OFFLINE", "required telemetry or control path is offline"};
  }
  return std::nullopt;
}

std::string ConnectivityRule::name() const {
  return "ConnectivityRule";
}

MetricThresholdRule::MetricThresholdRule(const bool primaryMetric)
    : primaryMetric_(primaryMetric) {}

std::optional<ValidationIssue> MetricThresholdRule::evaluate(
    const ProjectProfile& profile,
    const RuntimeSample& sample) const {
  const auto& metric = primaryMetric_ ? profile.primaryMetric : profile.secondaryMetric;
  const double value = primaryMetric_ ? sample.primaryValue : sample.secondaryValue;
  if (exceeds(metric, value)) {
    return ValidationIssue{
        primaryMetric_ ? Severity::Critical : Severity::Warning,
        primaryMetric_ ? "PRIMARY_METRIC_LIMIT" : "SECONDARY_METRIC_LIMIT",
        metric.name + "=" + fixed2(value) + " " + metric.units +
            " outside limit " + fixed2(metric.warningLimit) + " " + metric.units};
  }
  return std::nullopt;
}

std::string MetricThresholdRule::name() const {
  return primaryMetric_ ? "PrimaryMetricThresholdRule" : "SecondaryMetricThresholdRule";
}

void CompositeValidator::add(std::unique_ptr<IValidationRule> rule) {
  if (!rule) {
    throw std::invalid_argument("validator rule cannot be null");
  }
  rules_.push_back(std::move(rule));
}

std::vector<ValidationIssue> CompositeValidator::evaluate(
    const ProjectProfile& profile,
    const RuntimeSample& sample) const {
  std::vector<ValidationIssue> issues;
  for (const auto& rule : rules_) {
    if (auto issue = rule->evaluate(profile, sample)) {
      issues.push_back(*issue);
    }
  }
  return issues;
}

std::vector<std::string> CompositeValidator::ruleNames() const {
  std::vector<std::string> names;
  for (const auto& rule : rules_) {
    names.push_back(rule->name());
  }
  return names;
}

std::string JsonTelemetryEncoder::encode(
    const ProjectProfile& profile,
    const RuntimeSample& sample,
    const std::vector<ValidationIssue>& issues) const {
  std::ostringstream payload;
  payload << "{"
          << "\"project_id\":\"" << jsonEscape(profile.id) << "\","
          << "\"title\":\"" << jsonEscape(profile.title) << "\","
          << "\"sequence\":" << sample.sequence << ","
          << "\"source\":\"" << jsonEscape(sample.source) << "\","
          << "\"online\":" << (sample.online ? "true" : "false") << ","
          << "\"" << jsonEscape(profile.primaryMetric.name) << "\":" << fixed2(sample.primaryValue) << ","
          << "\"" << jsonEscape(profile.secondaryMetric.name) << "\":" << fixed2(sample.secondaryValue) << ","
          << "\"issue_count\":" << issues.size() << ","
          << "\"accepted\":" << (issues.empty() ? "true" : "false") << "}";
  return payload.str();
}

ProjectRuntime::ProjectRuntime(ProjectProfile profile,
                               ISampleSource& sampleSource,
                               CompositeValidator validator,
                               const ITelemetryEncoder& telemetryEncoder)
    : profile_(std::move(profile)),
      sampleSource_(sampleSource),
      validator_(std::move(validator)),
      telemetryEncoder_(telemetryEncoder) {}

RuntimeReport ProjectRuntime::runOnce() {
  RuntimeReport report;
  report.sample = sampleSource_.read();
  report.trace = validator_.ruleNames();
  report.issues = validator_.evaluate(profile_, report.sample);
  report.accepted = report.issues.empty();
  report.telemetryPayload = telemetryEncoder_.encode(profile_, report.sample, report.issues);
  return report;
}

TextReportWriter::TextReportWriter(std::ostream& stream) : stream_(stream) {}

void TextReportWriter::write(const ProjectProfile& profile, const RuntimeReport& report) const {
  stream_ << profile.title << '\n'
          << "status=" << (report.accepted ? "PASS" : "FAIL") << '\n'
          << "evidence=" << profile.evidenceTarget << '\n'
          << "sample_source=" << report.sample.source << '\n'
          << profile.primaryMetric.name << '=' << fixed2(report.sample.primaryValue)
          << ' ' << profile.primaryMetric.units << '\n'
          << profile.secondaryMetric.name << '=' << fixed2(report.sample.secondaryValue)
          << ' ' << profile.secondaryMetric.units << '\n';

  for (const auto& step : report.trace) {
    stream_ << "validator=" << step << '\n';
  }

  for (const auto& issue : report.issues) {
    stream_ << "issue=" << toString(issue.severity) << ':'
            << issue.code << ':' << issue.message << '\n';
  }

  stream_ << "telemetry=" << report.telemetryPayload << '\n';
}

ProjectProfile demoProfile() {
  return ProjectProfile{
      "tinyml-sensor-anomaly-detector",
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
      "Ring buffers"
      },
      MetricSpec{"sample_value", "units", 75.0, true},
      MetricSpec{"sample_jitter_ms", "ms", 20.0, true},
      false};
}

RuntimeSample nominalSample() {
  return RuntimeSample{1U, 33.75, 9.0, true, "host-ci-nominal"};
}

RuntimeSample unsafeSample() {
  return RuntimeSample{2U, 101.25, 27.0, true, "host-ci-limit-test"};
}

RuntimeSample offlineSample() {
  return RuntimeSample{3U, 33.75, 9.0, false, "host-ci-offline"};
}

CompositeValidator defaultValidator() {
  CompositeValidator validator;
  validator.add(std::make_unique<RequiredCapabilitiesRule>());
  validator.add(std::make_unique<ConnectivityRule>());
  validator.add(std::make_unique<MetricThresholdRule>(true));
  validator.add(std::make_unique<MetricThresholdRule>(false));
  return validator;
}

RuntimeReport runScenario(const std::string& mode) {
  RuntimeSample sample = nominalSample();
  if (mode == "unsafe") {
    sample = unsafeSample();
  } else if (mode == "offline") {
    sample = offlineSample();
  } else if (mode != "nominal") {
    throw std::invalid_argument("unknown scenario: " + mode);
  }

  FixedSampleSource source(sample);
  JsonTelemetryEncoder encoder;
  ProjectRuntime runtime(demoProfile(), source, defaultValidator(), encoder);
  return runtime.runOnce();
}

}  // namespace project_runtime
