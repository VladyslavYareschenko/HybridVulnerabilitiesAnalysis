#include "MainWindow.h"

#include <iostream>

#include <QCheckBox>
#include <QElapsedTimer>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>
#include <QThread>
#include <QTextBrowser>

namespace
{
class AsyncOperation : public QObject
{
    Q_OBJECT
public:
    AsyncOperation(std::function<QVariant()> operation, const bool runAsync = true)
    : _operation(std::move(operation))
    , _runAsync(runAsync)
    {
    }

    void run()
    {
        if (_runAsync)
        {
            QThread *thread = new QThread();
            moveToThread(thread);

            connect(thread, &QThread::started, this, [this, thread] {
                // execute
                _result = _operation();
                thread->quit();

                // inform
                emit finished(_result);

                // cleanup
                thread->deleteLater();
                deleteLater();
            });


            thread->start();
        }
        else
        {
            _result = _operation();

            emit finished(_result);

            deleteLater();
        }
    }

signals:
    void finished(QVariant);

private:
    bool _runAsync = true;
    QVariant _result;
    std::function<QVariant()> _operation;
};

void extractAnalysisErrorsSummary(const QString& analysisOutput, QLabel* destination)
{
    // Dynamic output example: ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

    auto dynamicCount = -1;

    static const QRegularExpression DynamicErrorsSubstring("\\d+ errors from");
    const auto errosPos = analysisOutput.indexOf(DynamicErrorsSubstring);

    if (errosPos > -1)
    {
        const QRegularExpression CounterSubstring("\\d+");
        dynamicCount = CounterSubstring.match(analysisOutput, errosPos).captured().toInt();
    }

    static const QRegularExpression CWEErrorSubstring("\\[CWE\\d+\\]");
    auto staticCount = analysisOutput.count(CWEErrorSubstring);

    QString outputString;
    QString stylesheet;

    if (dynamicCount >= 0)
    {
        outputString.append(QString("Dynamic errors: %1.").arg(dynamicCount));
        stylesheet = "color: rgb(230, 150, 25);";
    }

    if (staticCount >= 0)
    {
        outputString.append(QString("Static errors: %1.").arg(staticCount));
        stylesheet = "color: rgb(230, 150, 25);";
    }

    if (staticCount >= 0 && dynamicCount >= 0 && staticCount != dynamicCount)
    {
        outputString.append("Possible false-positives. Be careful!");
        stylesheet = "color: red;";
    }

    destination->setText(outputString);
    destination->setStyleSheet(stylesheet);
}

QString getElapsedAnalysisTimeString(const int msec = 0)
{
    return QString("Time spent: %1").arg(msec);
}
} // anonymous


class OperationsPool : public QObject
{
Q_OBJECT

public:
    using QObject::QObject;

    void appendOperation(AsyncOperation* operation)
    {
        if (operation == nullptr)
        {
            return;
        }

        _operations.append(operation);

        connect(operation, &AsyncOperation::finished, this, [this, operation]
        {
            _operations.removeOne(operation);

            if (_operations.isEmpty())
            {
                emit allOperationsFinished();

                deleteLater();
            }
        });
    }

signals:
    void allOperationsFinished();

private:
    QList<AsyncOperation*> _operations;
};

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
    centralLayout->addWidget(createRunAsyncAnalysisPredicateWidget());
    centralLayout->addWidget(createOutputWidgetsBlock());
    centralLayout->addStretch();
    centralLayout->addWidget(createAnalysisRunningStateWidget());

    setMinimumSize(472, 360);
}

QWidget* MainWindow::createSourcesAnalysisWidgetsGroup()
{
    QWidget* group = new QGroupBox("Static sources analysis", this);
    QHBoxLayout* hLayout = new QHBoxLayout(group);

    _enableStaticSourcesCheckbox = new QCheckBox("Run static sources analysis", group);
    hLayout->addWidget(_enableStaticSourcesCheckbox);

    _pathToSources = new QLineEdit(group);
    hLayout->addWidget(_pathToSources);

    QPushButton* browsePathButton = new QPushButton("Browse sources", group);
    hLayout->addWidget(browsePathButton);

    connect(browsePathButton,
            &QPushButton::clicked,
            this,
            [this, browsePathButton]()
            {
                QString dir = QFileDialog::getExistingDirectory(browsePathButton,
                                                                "Choose directory with cxx sources",
                                                                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                                QFileDialog::ShowDirsOnly |
                                                                QFileDialog::DontResolveSymlinks);

                _pathToSources->setText(dir);
            });

    return group;
}

QWidget* MainWindow::createBinariesAnalysisWidgetsGroup()
{
    QWidget* group = new QGroupBox("Binaries (libraries and executables) analysis", this);

    QGridLayout* grid = new QGridLayout(group);

    _enableStaticBinaryCheckbox = new QCheckBox("Run static binaries analysis", group);
    grid->addWidget(_enableStaticBinaryCheckbox, 0, 0);

    _enableDynamicBinaryCheckbox = new QCheckBox("Run dynamic binaries analysis", group);
    grid->addWidget(_enableDynamicBinaryCheckbox, 1, 0);

    _pathToBinary = new QLineEdit(group);
    grid->addWidget(_pathToBinary, 0, 1);

    QPushButton* browsePathButton = new QPushButton("Browse binaries", group);
    grid->addWidget(browsePathButton, 0, 2);

    connect(browsePathButton,
            &QPushButton::clicked,
            this,
            [this, browsePathButton]()
            {
                QString dir = QFileDialog::getOpenFileName(browsePathButton,
                                                           "Choose binary executable",
                                                           QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

                _pathToBinary->setText(dir);
            });

    return group;
}

QWidget* MainWindow::createRunAsyncAnalysisPredicateWidget()
{
    _runAsyncCheckbox = new QCheckBox("Run async analysis", this);

    return _runAsyncCheckbox;
}

QWidget* MainWindow::createOutputWidgetsBlock()
{
    QWidget* blockWidget = new QWidget(this);
    QVBoxLayout* blockLayout = new QVBoxLayout(blockWidget);

    QGroupBox* sourcesOutput = new QGroupBox("Sources analysis result: ",this);
    QVBoxLayout* sourcesOutputLayout = new QVBoxLayout(sourcesOutput);

    _sourcesAnalysisOutputBrowser = new QTextBrowser(sourcesOutput);
    _sourcesAnalysisOutputBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _sourcesAnalysisOutputBrowser->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    sourcesOutputLayout->addWidget(_sourcesAnalysisOutputBrowser);

    blockLayout->addWidget(sourcesOutput, 1);

    QGroupBox* binaryOutput = new QGroupBox("Binary analysis result: ",this);
    QVBoxLayout* binaryOutputLayout = new QVBoxLayout(binaryOutput);

    _binaryAnalysisOutputBrowser = new QTextBrowser(binaryOutput);
    _binaryAnalysisOutputBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _binaryAnalysisOutputBrowser->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    binaryOutputLayout->addWidget(_binaryAnalysisOutputBrowser);

    blockLayout->addWidget(binaryOutput, 1);

    _timeTakenLabel = new QLabel(blockWidget);
    _timeTakenLabel->setText(getElapsedAnalysisTimeString());
    blockLayout->addWidget(_timeTakenLabel);

    _binaryErrorsSummary = new QLabel(blockWidget);
    _binaryErrorsSummary->setText(" ");
    blockLayout->addWidget(_binaryErrorsSummary);

    return blockWidget;
}

QWidget* MainWindow::createAnalysisRunningStateWidget()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(widget);

    _progressBar = new QProgressBar(widget);
    _progressBar->setRange(0, 100);
    constexpr auto AutoIntrementMaxValue = 85;

    hLayout->addWidget(_progressBar);

    QTimer* autoProgressTimer = new QTimer(_progressBar);
    autoProgressTimer->setInterval(500);
    connect(autoProgressTimer,
            &QTimer::timeout,
            _progressBar,
            [this, autoProgressTimer]()
            {
                constexpr auto SingleStep = 2;
                const auto progressValue = std::min(_progressBar->value() + SingleStep, _progressBar->maximum());

                _progressBar->setValue(progressValue);

                if (progressValue >= AutoIntrementMaxValue)
                {
                    autoProgressTimer->stop();
                    autoProgressTimer->deleteLater();
                }
            });

    hLayout->addStretch();

    connect(_progressBar,
            &QProgressBar::valueChanged,
            this,
            [this, autoProgressTimer](const int value)
            {
                if (_progressBar->value() >= AutoIntrementMaxValue)
                {
                    autoProgressTimer->stop();
                }
            });

    QPushButton* runButton = new QPushButton("Run", this);
    connect(runButton,
            &QPushButton::clicked,
            this,
            [this, autoProgressTimer]()
            {
                _sourcesAnalysisOutputBrowser->clear();
                _binaryAnalysisOutputBrowser->clear();

                _progressBar->setValue(0);
                autoProgressTimer->start();

                QSharedPointer<QElapsedTimer> timeTakenTimer(new QElapsedTimer());
                timeTakenTimer->start();
                auto* pool = performAnalysis(_runAsyncCheckbox->isChecked());

                connect(pool, &OperationsPool::allOperationsFinished, this, [this, timeTakenTimer]
                {
                    _timeTakenLabel->setText(getElapsedAnalysisTimeString(timeTakenTimer->elapsed()));

                    _progressBar->setValue(_progressBar->maximum());

                    extractAnalysisErrorsSummary(_binaryAnalysisOutputBrowser->toPlainText(), _binaryErrorsSummary);
                });
            });
    hLayout->addWidget(runButton);

    return widget;
}

OperationsPool* MainWindow::performAnalysis(const bool runAsync)
{
    auto performAnalysisImpl = [this, runAsync](QCheckBox* predicate,
                                                QTextBrowser* outputContainer,
                                                std::function<QString(std::string)> analysisFunc,
                                                const QString& path,
                                                const QString& emptyPathFallbackMessage) -> AsyncOperation*
    {
        assert(predicate);
        assert(outputContainer);

        if (path.isEmpty() || !predicate->isChecked())
        {
            if (predicate->isChecked())
            {
                outputContainer->append(emptyPathFallbackMessage);
            }

            return nullptr;
        }

        auto* op = new AsyncOperation([this, func = std::move(analysisFunc), path = path.toStdString()]()
                                      {
                                          return func(path);
                                      },
                                      runAsync);

        connect(op,
                &AsyncOperation::finished,
                this,
                [this, outputContainer](QVariant result)
                {
                    outputContainer->append(result.toString());
                });

        if (runAsync)
        {
            op->run();
        }

        return op;
    };

    OperationsPool* pool = new OperationsPool(this);

    auto* staticSourcesOp = performAnalysisImpl(

                            _enableStaticSourcesCheckbox,
                            _sourcesAnalysisOutputBrowser,
                            [this](std::string path)
                            {
                                return QString::fromStdString(
                                        _caller.callStaticSourcesAnalysis(std::move(path)));
                            },
                            _pathToSources->text(),
                            "No path specified for sources. Sources analysis will be skipped.");

    auto* dynamicBinaryOp = performAnalysisImpl(
                            _enableDynamicBinaryCheckbox,
                            _binaryAnalysisOutputBrowser,
                            [this](std::string path)
                            {
                                return QString::fromStdString(
                                        _caller.callDynamicBinaryAnalysis(std::move(path)));
                            },
                            _pathToBinary->text(),
                            "No path specified for binary. Binary analysis will be skipped.");


    auto* staticBinaryOp = performAnalysisImpl(
                            _enableStaticBinaryCheckbox,
                            _binaryAnalysisOutputBrowser,
                            [this](std::string path)
                            {
                                return QString::fromStdString(
                                        _caller.callStaticBinaryAnalysis(std::move(path)));
                            },
                            _pathToBinary->text(),
                            "No path specified for binary. Binary analysis will be skipped.");

    if (runAsync)
    {
        pool->appendOperation(staticSourcesOp);
        pool->appendOperation(dynamicBinaryOp);
        pool->appendOperation(staticBinaryOp);

    }
    else
    {
        auto* cumulativeOperation = new AsyncOperation([=]()
                                                       {
                                                           if (staticSourcesOp)
                                                           {
                                                               staticSourcesOp->run();
                                                           }

                                                           if (dynamicBinaryOp)
                                                           {
                                                               dynamicBinaryOp->run();
                                                           }

                                                           if (staticBinaryOp)
                                                           {
                                                               staticBinaryOp->run();
                                                           }

                                                           return QVariant();
                                                       });

        cumulativeOperation->run();

        pool->appendOperation(cumulativeOperation);
    }

    connect(pool, &OperationsPool::allOperationsFinished, pool, &OperationsPool::deleteLater);

    return pool;
}

#include "MainWindow.moc"
