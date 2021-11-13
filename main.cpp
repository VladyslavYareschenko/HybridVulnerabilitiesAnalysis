#include <iostream>
#include <QApplication>

extern "C" void run_for_path(const char*);

int main(int argc, char **argv)
{
    // run_for_path("/home/osboxes/CLionProjects/TestProjectWithVulnerabilities/executable");

    QApplication app(argc, argv);

    return 0;
}
