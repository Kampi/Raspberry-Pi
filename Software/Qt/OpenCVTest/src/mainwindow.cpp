#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          _mUi(new Ui::MainWindow)
{
    _mUi->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _mUi;
}

void MainWindow::on_action_AboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}
