#pragma once

#include <string>

std::string callCWECheckerProcess(std::string forPath);
std::string callCppCheckProcess(std::string forPath);
std::string callValgrindCheckProcess(std::string forPath);
