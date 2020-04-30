#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QtCharts>
#include <QMainWindow>

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

    private:
        Ui::MainWindow* _mUi;

        QLineSeries* _mSeries;
        QChartView*_mChartView;
        QChart* _mChart;
        QTimer* _mTimer;
        QDateTimeAxis* _mXAxis;
        QValueAxis* _mYAxis;
};
#endif // MAINWINDOW_H
