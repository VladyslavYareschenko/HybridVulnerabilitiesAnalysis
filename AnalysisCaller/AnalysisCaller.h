#pragma once

#include <string>

class AnalysisCaller {
public:
    std::string callStaticSourcesAnalysis(std::string forPath);
    std::string callStaticBinaryAnalysis(std::string forExecutable);
    std::string callDynamicBinaryAnalysis(std::string forPath);
};

