#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void on_pushButton_Click_clicked();
        void on_action_AboutQt_triggered();
        void on_action_Close_triggered();

    private:
        Ui::MainWindow* _mUi;
};

#endif // MAINWINDOW_H
