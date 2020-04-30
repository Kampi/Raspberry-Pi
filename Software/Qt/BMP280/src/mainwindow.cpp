#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow),
                                          _mTimer(new QTimer(this)),
                                          _mTemperatureSeries(new QLineSeries()),
                                          _mPressureSeries(new QLineSeries()),
                                          _mChart(new QChart()),
                                          _mXAxis(new QDateTimeAxis(this)),
                                          _mTemperatureAxis(new QValueAxis(this)),
                                          _mPressureAxis(new QValueAxis(this)),
                                          _mSensor(new BMP280())
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    connect(_mTimer, &QTimer::timeout, this, &MainWindow::TimerTick);

    _mChartView = new QChartView(_mChart);

    _mChart->addAxis(_mXAxis, Qt::AlignBottom);
    _mChart->addAxis(_mTemperatureAxis, Qt::AlignLeft);
    _mChart->addAxis(_mPressureAxis, Qt::AlignRight);
    _mChart->addSeries(_mTemperatureSeries);
    _mChart->addSeries(_mPressureSeries);

    _mTemperatureSeries->attachAxis(_mXAxis);
    _mTemperatureSeries->attachAxis(_mTemperatureAxis);
    _mTemperatureSeries->setName("Temperature");

    _mPressureSeries->attachAxis(_mXAxis);
    _mPressureSeries->attachAxis(_mPressureAxis);
    _mPressureSeries->setName("Pressure");

    _mXAxis->setFormat("hh:mm:ss");

    _mTemperatureAxis->setRange(-20, 60);
    _mTemperatureAxis->setTitleText("Temperature [°C]");

    _mPressureAxis->setRange(900, 1100);
    _mPressureAxis->setTitleText("Pressure [hPa]");

    this->setCentralWidget(_mChartView);

    _mTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::TimerTick(void)
{
    double Pressure = 0.0;
    double Temperature = 0.0;

    _mXAxis->setMin(QDateTime::currentDateTime().addSecs(-100));
    _mXAxis->setMax(QDateTime::currentDateTime());

    if(!(_mSensor->OpenDevice("/dev/i2c-1") || _mSensor->SetMode(BMP280::MODE_FORCED) || _mSensor->MeasureTemperature(BMP280::OSS_X1, &Temperature) || _mSensor->MeasurePressure(BMP280::OSS_X1, BMP280::OSS_X1, &Pressure)))
    {
        QDateTime Now = QDateTime::currentDateTime();
        _mTemperatureSeries->append(Now.toMSecsSinceEpoch(), Temperature);
        _mPressureSeries->append(Now.toMSecsSinceEpoch(), Pressure);
    }
}

void MainWindow::on_action_AboutQt_triggered(void)
{
    QMessageBox::aboutQt(this, tr("About Qt..."));
}

void MainWindow::on_action_ExportPNG_triggered(void)
{
    QString Path = QFileDialog::getSaveFileName(this, tr("Export chart"), "", ("Portable Network Graphics (*.png)"));
    if(!Path.isEmpty())
    {
        QPixmap PixMap = _mChartView->grab();
        PixMap.save(Path + ".png", "PNG");

        this->statusBar()->showMessage(tr("Save file as") + " " + Path + ".png");
    }
}
