#include "project_runtime/ProjectRuntime.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

bool contains(const std::string& value, const std::string& needle) {
  return value.find(needle) != std::string::npos;
}

void nominalScenarioPasses() {
  const auto profile = project_runtime::demoProfile();
  const auto report = project_runtime::runScenario("nominal");

  assert(report.accepted);
  assert(report.issues.empty());
  assert(!report.telemetryPayload.empty());
  assert(contains(report.telemetryPayload, profile.id));
  assert(report.trace.size() == 4U);
}

void unsafeScenarioFailsSafetyGates() {
  const auto report = project_runtime::runScenario("unsafe");

  assert(!report.accepted);
  assert(!report.issues.empty());
  assert(contains(report.telemetryPayload, "\"accepted\":false"));
}

void offlineScenarioIsDeterministic() {
  const auto profile = project_runtime::demoProfile();
  const auto report = project_runtime::runScenario("offline");

  if (profile.connectivityRequired) {
    assert(!report.accepted);
    assert(!report.issues.empty());
  } else {
    assert(report.accepted);
  }
}

void reportWriterIncludesSolidEvidence() {
  const auto profile = project_runtime::demoProfile();
  const auto report = project_runtime::runScenario("nominal");
  std::ostringstream output;
  project_runtime::TextReportWriter writer(output);

  writer.write(profile, report);

  assert(contains(output.str(), "validator=RequiredCapabilitiesRule"));
  assert(contains(output.str(), "telemetry="));
}

}  // namespace

int main() {
  nominalScenarioPasses();
  unsafeScenarioFailsSafetyGates();
  offlineScenarioIsDeterministic();
  reportWriterIncludesSolidEvidence();
  return 0;
}
