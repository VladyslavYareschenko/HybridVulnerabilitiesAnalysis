#include "Utils.h"
#include "Config.h"

#include <iostream>

#include <QProcess>

namespace
{
extern "C" void run_for_path(const char*);
} // anonymous;

void callCweCheckerExternRun(const std::string forPath)
{
    run_for_path(forPath.c_str());
}

void callCppCheckProcess(const std::string forPath)
{
    QProcess cppCheckProc;

    QString executable = CppCheckExecutable;

    QStringList arguments;
    arguments << QString::fromStdString(forPath) << "--force";

    cppCheckProc.start(executable, arguments);
    std::cout << "Proc started" << std::endl;
    cppCheckProc.waitForFinished();
    std::cout << "Proc ended" << int(cppCheckProc.error()) << std::endl;

    std::cout << cppCheckProc.readAllStandardError().constData() << std::endl;
    std::cout << cppCheckProc.readAll().constData() << std::endl;
}

void callValgrindCheckProcess(const std::string forPath)
{
    QProcess valgrindCheckProc;

    QString executable = ValgrindExecutable;

    QStringList arguments;
    arguments << QString::fromStdString(forPath);

    valgrindCheckProc.start(executable, arguments);
    std::cout << "Proc started" << std::endl;
    valgrindCheckProc.waitForFinished();
    std::cout << "Proc ended" << int(valgrindCheckProc.error()) << std::endl;

    std::cout << valgrindCheckProc.readAllStandardError().constData() << std::endl;
    std::cout << valgrindCheckProc.readAll().constData() << std::endl;
}