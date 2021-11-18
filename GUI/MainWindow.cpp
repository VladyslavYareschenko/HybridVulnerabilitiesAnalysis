#include "MainWindow.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QFrame(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* centralLayout = new QVBoxLayout(centralWidget);

    centralLayout->addWidget(createSourcesAnalysisWidgetsGroup());
    centralLayout->addWidget(createBinariesAnalysisWidgetsGroup());
    centralLayout->addStretch();
    centralLayout->addWidget(createAnalysisRunningStateWidget());

    setMinimumSize(472, 360);
}

QWidget* MainWindow::createSourcesAnalysisWidgetsGroup()
{
    QWidget* group = new QGroupBox("Static sources analysis", this);
    QHBoxLayout* hLayout = new QHBoxLayout(group);

    QCheckBox* enableStaticCheckbox = new QCheckBox("Run static sources analysis", group);
    hLayout->addWidget(enableStaticCheckbox);

    QLineEdit* pathToSources = new QLineEdit(group);
    hLayout->addWidget(pathToSources);

    QPushButton* browsePathButton = new QPushButton("Browse sources", group);
    hLayout->addWidget(browsePathButton);

    connect(browsePathButton,
            &QPushButton::clicked,
            this,
            [pathToSources, browsePathButton]()
            {
                QString dir = QFileDialog::getExistingDirectory(browsePathButton,
                                                                "Choose directory with cxx sources",
                                                                "/home",
                                                                QFileDialog::ShowDirsOnly |
                                                                QFileDialog::DontResolveSymlinks);

                pathToSources->setText(dir);
            });

    return group;
}

QWidget* MainWindow::createBinariesAnalysisWidgetsGroup()
{
    QWidget* group = new QGroupBox("Binaries (libraries and executables) analysis", this);

    QGridLayout* grid = new QGridLayout(group);

    QCheckBox* enableStatic = new QCheckBox("Run static binaries analysis", group);
    grid->addWidget(enableStatic, 0, 0);

    QCheckBox* enableDynamic = new QCheckBox("Run dynamic binaries analysis", group);
    grid->addWidget(enableDynamic, 1, 0);

    QLineEdit* pathToBinaries = new QLineEdit(group);
    grid->addWidget(pathToBinaries, 0, 1);

    QPushButton* browsePathButton = new QPushButton("Browse binaries", group);
    grid->addWidget(browsePathButton, 0, 2);

    connect(browsePathButton,
            &QPushButton::clicked,
            this,
            [pathToBinaries, browsePathButton]()
            {
                QString dir = QFileDialog::getExistingDirectory(browsePathButton,
                                                                "Choose directory with binaries",
                                                                "/home",
                                                                QFileDialog::ShowDirsOnly |
                                                                QFileDialog::DontResolveSymlinks);

                pathToBinaries->setText(dir);
            });

    return group;
}

QWidget* MainWindow::createAnalysisRunningStateWidget()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(widget);

    QProgressBar* progressBar = new QProgressBar(widget);
    progressBar->setRange(0, 100);
    hLayout->addWidget(progressBar);

    QTimer* autoProgressTimer = new QTimer(progressBar);
    autoProgressTimer->setInterval(500);
    connect(autoProgressTimer,
            &QTimer::timeout,
            progressBar,
            [progressBar, autoProgressTimer]()
            {
                constexpr auto SingleStep = 2;
                const auto progressValue = std::min(progressBar->value() + SingleStep, progressBar->maximum());

                progressBar->setValue(progressValue);

                if (progressValue >= progressBar->maximum())
                {
                    autoProgressTimer->stop();
                    autoProgressTimer->deleteLater();
                }
            });
    autoProgressTimer->start();

    hLayout->addStretch();

    QPushButton* runButton = new QPushButton("Run", this);
    hLayout->addWidget(runButton);

    return widget;
}