#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow),
                                          _mSeries(new QSplineSeries()),
                                          _mChart(new QChart()),
                                          _mTimer(new QTimer(this)),
                                          _mXAxis(new QDateTimeAxis(this)),
                                          _mYAxis(new QValueAxis(this))
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    connect(_mTimer, &QTimer::timeout, this, &MainWindow::TimerTick);

    _mChartView = new QChartView(_mChart);

    _mChart->legend()->hide();
    _mChart->addAxis(_mXAxis, Qt::AlignBottom);
    _mChart->addAxis(_mYAxis, Qt::AlignLeft);
    _mChart->addSeries(_mSeries);

    _mSeries->attachAxis(_mXAxis);
    _mSeries->attachAxis(_mYAxis);

    _mXAxis->setFormat("hh:mm:ss");

    _mYAxis->setRange(20, 100);
    _mYAxis->setTitleText("Temperature [°C]");

    this->setCentralWidget(_mChartView);

    _mTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::TimerTick(void)
{
    _mXAxis->setMin(QDateTime::currentDateTime().addSecs(-100));
    _mXAxis->setMax(QDateTime::currentDateTime());

    QFile Thermal("/sys/class/thermal/thermal_zone0/temp");
    if(Thermal.open(QIODevice::ReadOnly))
    {
        bool Ok;
        double Temp;

        QString Temperature = Thermal.readLine();
        Temp = Temperature.remove("\n").toDouble(&Ok);

        if(Ok)
        {
            _mSeries->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), Temp / 1000.0);
        }
    }
}
