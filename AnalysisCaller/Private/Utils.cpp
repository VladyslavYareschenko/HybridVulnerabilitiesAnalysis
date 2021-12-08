#include "Utils.h"
#include "Config.h"

#include <iostream>
#include <sstream>

#include <QProcess>

namespace
{
extern "C" void run_for_path(const char*);

class StandardOutputRedirect
{
public:
    StandardOutputRedirect()
    {
        auto setNew = _buffer.rdbuf();
        Q_ASSERT(setNew);

        _oldStd = std::cout.rdbuf(setNew);
        _oldErr = std::cerr.rdbuf(setNew);
    }

    ~StandardOutputRedirect()
    {
        std::cout.rdbuf(_oldStd);
        std::cout.rdbuf(_oldErr);
    }

    std::string getRedirectedOutput() const
    {
        return _buffer.str();
    }

private:
    std::stringstream _buffer;
    std::streambuf* _oldStd;
    std::streambuf* _oldErr;
};
} // anonymous;

//std::string callCweCheckerExternRun(const std::string forPath)
//{
//    StandardOutputRedirect filter;
//
//    run_for_path(forPath.c_str());
//
//    return filter.getRedirectedOutput();
//}

std::string callCWECheckerProcess(const std::string forPath)
{
    QProcess cweCheckerProc;

    QString executable = CWECheckerExecutable;
    QStringList arguments;
    arguments << QString::fromStdString(forPath);

    cweCheckerProc.start(executable, arguments);
    cweCheckerProc.waitForFinished(-1);

    const auto stdOut = cweCheckerProc.readAll().toStdString();
    const auto stdErr = cweCheckerProc.readAllStandardError().toStdString();

    return stdOut + '\n' + stdErr;
}

std::string callCppCheckProcess(const std::string forPath)
{
    QProcess cppCheckProc;

    QString executable = CppCheckExecutable;
    QStringList arguments;
    arguments << QString::fromStdString(forPath) << "--force" << "--inconclusive";

    cppCheckProc.start(executable, arguments);
    cppCheckProc.waitForFinished(-1);

    const auto stdOut = cppCheckProc.readAll().toStdString();
    const auto stdErr = cppCheckProc.readAllStandardError().toStdString();

    return stdOut + '\n' + stdErr;
}

std::string callValgrindCheckProcess(const std::string forPath)
{
    QProcess valgrindCheckProc;

    QString executable = ValgrindExecutable;
    QStringList arguments;
    arguments << QString::fromStdString(forPath);

    valgrindCheckProc.start(executable, arguments);
    valgrindCheckProc.waitForFinished(-1);

    const auto stdOut = valgrindCheckProc.readAll().toStdString();
    const auto stdErr = valgrindCheckProc.readAllStandardError().toStdString();

    return stdOut + '\n' + stdErr;
}