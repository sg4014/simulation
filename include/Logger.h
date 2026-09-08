//
// Created by sergey on 9/7/26.
//

#ifndef SIMULATION_LOGGER_H
#define SIMULATION_LOGGER_H
#include <fstream>
#include <iostream>
#include <format>
#include <chrono>

class Logger {
public:
  explicit Logger(const std::string& filename) : m_outf{filename} {
    if (!m_outf) {
      std::cerr << std::format("{} could not be opened for writing, the logger is in invalid state.",
        filename);
      return;
    }
    isValid = true;
  }

  void log(std::string_view sv) {
    const auto now = std::chrono::steady_clock::now();
    const auto timeSinceLastLog = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastLog);
    std::cerr << std::format("{} {}\n", timeSinceLastLog, sv);
    m_outf << std::format("{} {}\n", timeSinceLastLog, sv);
    m_lastLog = now;
  }

private:
  std::ofstream m_outf{};
  std::chrono::time_point<std::chrono::steady_clock> m_lastLog {};
  bool isValid{};
};

#endif //SIMULATION_LOGGER_H