#pragma once

#include <string>

class AnalysisCaller {
public:
    static void runCweCheck(std::string path);
    static void runCppCheck(std::string path);
    static void runValgrindCheck(std::string path);
};

