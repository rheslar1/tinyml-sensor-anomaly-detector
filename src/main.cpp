#include "project_runtime/ProjectRuntime.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void printUsage(const char* programName) {
  std::cout << "Usage: " << programName << " [--nominal|--unsafe|--offline|--help]\n";
}

}  // namespace

int main(int argc, char** argv) {
  const std::string option = argc > 1 ? argv[1] : "--nominal";
  if (option == "--help") {
    printUsage(argv[0]);
    return 0;
  }

  const std::string mode =
      option == "--nominal" ? "nominal" :
      option == "--unsafe" ? "unsafe" :
      option == "--offline" ? "offline" : "";

  if (mode.empty()) {
    printUsage(argv[0]);
    return 1;
  }

  try {
    const auto profile = project_runtime::demoProfile();
    const auto report = project_runtime::runScenario(mode);
    project_runtime::TextReportWriter writer(std::cout);
    writer.write(profile, report);
    return report.accepted ? 0 : 2;
  } catch (const std::exception& exception) {
    std::cerr << "runtime error: " << exception.what() << '\n';
    return 1;
  }
}
