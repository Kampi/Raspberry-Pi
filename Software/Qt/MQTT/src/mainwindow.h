#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QMessageBox>
#include <QtMqtt/QtMqtt>

#include <QDebug>
#include "settingsdialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    protected:
        void closeEvent(QCloseEvent* event);

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void Settings_finished(int result);
        void MQTT_Connected(void);
        void MQTT_Received(const QByteArray& message, const QMqttTopicName& topic);
        void on_pushButton_Publish_clicked();
        void on_pushButton_Subscribe_clicked();
        void on_action_Connect_triggered();
        void on_action_Disconnect_triggered();
        void on_action_Settings_triggered();

    private:
        Ui::MainWindow* _mUi;
        QMqttClient* _mClient;

        QString _mIp;
        int _mPort;

        void _disconnect(void);
};

#endif // MAINWINDOW_H
