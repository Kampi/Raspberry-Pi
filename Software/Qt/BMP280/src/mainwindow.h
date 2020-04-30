#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QTimer>
#include <QtCharts>
#include <QMainWindow>

#include "bmp280.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public slots:
        void TimerTick(void);

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void on_action_AboutQt_triggered();
        void on_action_ExportPNG_triggered();

    private:
        Ui::MainWindow* _mUi;

        QTimer* _mTimer;
        QLineSeries* _mTemperatureSeries;
        QLineSeries* _mPressureSeries;
        QChartView*_mChartView;
        QChart* _mChart;
        QDateTimeAxis* _mXAxis;
        QValueAxis* _mTemperatureAxis;
        QValueAxis* _mPressureAxis;

        BMP280* _mSensor;
};
#endif // MAINWINDOW_H
