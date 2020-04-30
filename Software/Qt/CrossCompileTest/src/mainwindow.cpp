#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    _mUi->label_Version->setText(QString("Version: %1").arg(QT_VERSION_STR));
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::on_pushButton_Click_clicked()
{
    _mUi->label_Output->setText("You clicked the button!");
}

void MainWindow::on_action_AboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_action_Close_triggered()
{
    this->close();
}
