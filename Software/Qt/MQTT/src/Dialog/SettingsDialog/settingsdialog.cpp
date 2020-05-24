#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QString IP, int Port, QWidget* parent) : QDialog(parent),
                                                                        _mUi(new Ui::SettingsDialog),
                                                                        _mIp(IP),
                                                                        _mPort(Port)
{
    _mUi->setupUi(this);

    _mUi->lineEdit_IP->setText(IP);
    _mUi->spinBox_Port->setValue(Port);
}

SettingsDialog::~SettingsDialog()
{
    delete _mUi;
}

QString SettingsDialog::ip() const
{
    return _mIp;
}

int SettingsDialog::port() const
{
    return _mPort;
}

void SettingsDialog::on_buttonBox_Close_clicked(QAbstractButton* button)
{
    if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Save))
    {
        this->accept();
    }
    else if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}

void SettingsDialog::on_lineEdit_IP_textChanged(const QString& arg1)
{
    _mIp = arg1;
}

void SettingsDialog::on_spinBox_Port_valueChanged(int arg1)
{
    _mPort = arg1;
}
