#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow),
                                          _mChart(new QChart()),
                                          _mSeries(new QSplineSeries(this)),
                                          _mXaxis(new QDateTimeAxis()),
                                          _mYaxis(new QValueAxis()),
                                          _mTimer(new QTimer(this))
{
    _mUi->setupUi(this);

    connect(_mTimer, &QTimer::timeout, this, &MainWindow::TimerTick);

    _mXaxis->setFormat("hh:mm:ss");
    _mXaxis->setTitleText("Time");

    _mYaxis->setTitleText("Random number");
    _mYaxis->setMin(-1.2);
    _mYaxis->setMax(1.2);

    _mChart->addSeries(_mSeries);
    _mChart->addAxis(_mXaxis, Qt::AlignBottom);
    _mChart->addAxis(_mYaxis, Qt::AlignLeft);
    _mChart->setTitle("Spline Series Example");

    _mSeries->setName("Data");
    _mSeries->attachAxis(_mXaxis);
    _mSeries->attachAxis(_mYaxis);

    _mView = new QChartView(_mChart);

    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    this->setCentralWidget(_mView);

    _mTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::TimerTick()
{
    QDateTime Now = QDateTime::currentDateTime();

    _mXaxis->setMin(QDateTime::currentDateTime().addSecs(-10));
    _mXaxis->setMax(Now);
    _mSeries->append(Now.toMSecsSinceEpoch(), QRandomGenerator::global()->bounded(-1, 1));
}
