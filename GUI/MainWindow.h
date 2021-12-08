#pragma once

#include "../AnalysisCaller/AnalysisCaller.h"

#include <QMainWindow>

class QCheckBox;
class QLabel;
class QLineEdit;
class QTextBrowser;
class QProgressBar;

class OperationsPool;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void setupUI();
    QWidget* createSourcesAnalysisWidgetsGroup();
    QWidget* createBinariesAnalysisWidgetsGroup();
    QWidget* createRunAsyncAnalysisPredicateWidget();
    QWidget* createOutputWidgetsBlock();
    QWidget* createAnalysisRunningStateWidget();

    OperationsPool* performAnalysis(bool runAsync);

private:
    AnalysisCaller _caller;

    QCheckBox* _enableStaticSourcesCheckbox = nullptr;
    QLineEdit* _pathToSources = nullptr;

    QCheckBox* _enableStaticBinaryCheckbox = nullptr;
    QCheckBox* _enableDynamicBinaryCheckbox = nullptr;
    QLineEdit* _pathToBinary = nullptr;

    QCheckBox* _runAsyncCheckbox = nullptr;

    QTextBrowser* _sourcesAnalysisOutputBrowser = nullptr;
    QTextBrowser* _binaryAnalysisOutputBrowser = nullptr;

    QLabel* _binaryErrorsSummary = nullptr;
    QLabel* _timeTakenLabel = nullptr;

    QProgressBar* _progressBar = nullptr;
};
