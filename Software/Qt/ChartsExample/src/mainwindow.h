#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QtCharts>
#include <QMainWindow>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void TimerTick(void);
    private:
        Ui::MainWindow* _mUi;

        QChartView* _mView;
        QChart* _mChart;
        QSplineSeries* _mSeries;
        QDateTimeAxis* _mXaxis;
        QValueAxis* _mYaxis;
        QTimer* _mTimer;
};
#endif // MAINWINDOW_H
