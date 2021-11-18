#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void setupUI();
    QWidget* createSourcesAnalysisWidgetsGroup();
    QWidget* createBinariesAnalysisWidgetsGroup();
    QWidget* createAnalysisRunningStateWidget();
};
