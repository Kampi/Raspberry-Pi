#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow),
                                          _mClient(new QMqttClient(this))
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    connect(_mClient, &QMqttClient::messageReceived, this, &MainWindow::MQTT_Received);
    connect(_mClient, &QMqttClient::connected, this, &MainWindow::MQTT_Connected);

    // Default settings
    _mIp = "127.0.0.1";
    _mPort = 1883;

    _mUi->statusBar->showMessage(tr("Disconnected"));
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::Settings_finished(int result)
{
    if(result == 1)
    {
        SettingsDialog* Dialog = qobject_cast<SettingsDialog*>(sender());

        _mIp = Dialog->ip();
        _mPort = Dialog->port();
    }
}

void MainWindow::MQTT_Connected(void)
{
    _mUi->statusBar->showMessage(tr("Connected"));
}

void MainWindow::MQTT_Received(const QByteArray& message, const QMqttTopicName& topic)
{
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message
                + QLatin1Char('\n');
    _mUi->textEdit_Messages->insertPlainText(content);
}

void MainWindow::on_pushButton_Publish_clicked()
{
    if(_mClient->publish(_mUi->lineEdit_TopicPublish->text(), _mUi->lineEdit_MessagePublish->text().toUtf8()) == -1)
    {
        _mUi->statusBar->showMessage(tr("Could not publish to topic") + " " + _mUi->lineEdit_TopicPublish->text());

        return;
    }

    _mUi->statusBar->showMessage(tr("Publish to topic") + " " + _mUi->lineEdit_TopicPublish->text());
}

void MainWindow::on_pushButton_Subscribe_clicked()
{
    if(!_mClient->subscribe(_mUi->lineEdit_TopicSubscribe->text()))
    {
        _mUi->statusBar->showMessage(tr("Could not subscripe to topic") + " " + _mUi->lineEdit_TopicSubscribe->text());

        return;
    }

    _mUi->statusBar->showMessage(tr("Subscripe to topic") + " " + _mUi->lineEdit_TopicSubscribe->text());
}

void MainWindow::on_action_Connect_triggered()
{
    if(_mClient->state() == QMqttClient::Disconnected)
    {
        _mUi->action_Connect->setEnabled(false);
        _mUi->action_Disconnect->setEnabled(true);
        _mClient->setHostname(_mIp);
        _mClient->setPort(_mPort);
        _mClient->connectToHost();
    }
}

void MainWindow::on_action_Disconnect_triggered()
{
    this->_disconnect();
}

void MainWindow::on_action_Settings_triggered()
{
    SettingsDialog* Dialog = new SettingsDialog(_mIp, _mPort, this);
    connect(Dialog, &QDialog::finished, this, &MainWindow::Settings_finished);
    Dialog->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    this->_disconnect();

    QMainWindow::closeEvent(event);
}

void MainWindow::_disconnect(void)
{
    if(_mClient->state() == QMqttClient::Connected)
    {
        _mClient->disconnectFromHost();
    }

    _mUi->action_Disconnect->setEnabled(false);
    _mUi->action_Connect->setEnabled(true);

    _mUi->statusBar->showMessage(tr("Disconnected"));
}
