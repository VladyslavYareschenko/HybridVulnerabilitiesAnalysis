#include "AnalysisCaller.h"
#include "Private/Utils.h"

void AnalysisCaller::runCweCheck(std::string path)
{
    callCweCheckerExternRun(std::move(path));
}

void AnalysisCaller::runCppCheck(std::string path)
{
    callCppCheckProcess(std::move(path));
}

void AnalysisCaller::runValgrindCheck(std::string path)
{
    callValgrindCheckProcess(std::move(path));
}