#include "AnalysisCaller.h"
#include "Private/Utils.h"

std::string AnalysisCaller::callStaticSourcesAnalysis(std::string forPath)
{
    return callCppCheckProcess(forPath);
}

std::string AnalysisCaller::callStaticBinaryAnalysis(std::string forExecutable)
{
    return callCWECheckerProcess(forExecutable);
}

std::string AnalysisCaller::callDynamicBinaryAnalysis(std::string forPath)
{
    return callValgrindCheckProcess(forPath);
}