#include "Utils.h"

#include <iostream>
#include <cassert>

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
    assert(false && "callCppCheckProcess not implemented.");
}

void callValgrindCheckProcess(const std::string forPath)
{
    assert(false && "callValgrindCheckProcess not implemented.");
}