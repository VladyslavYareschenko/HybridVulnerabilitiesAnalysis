#include "GUI/MainWindow.h"

#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QWindow>

void* get_ex()
{
    return nullptr;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    if (auto* handle = window.windowHandle(); handle->screen())
    {
        window.setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                               Qt::AlignCenter,
                                               window.size(),
                                               handle->screen()->availableGeometry()));
    }
//    AnalysisCaller::runCweCheck("/home/osboxes/CLionProjects/TestProjectWithVulnerabilities/executable");
//    AnalysisCaller::runCppCheck("/home/osboxes/CLionProjects/TestProjectWithVulnerabilities/main.cpp");
//    AnalysisCaller::runValgrindCheck("/home/osboxes/CLionProjects/TestProjectWithVulnerabilities/executable");

    return app.exec();
}
