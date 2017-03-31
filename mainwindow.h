#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <management.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadImageAction_triggered();


    void on_saveImageAsAction_triggered();

    void on_reloadAction_triggered();

    void on_normalizeAction_triggered();

    void on_gaussAction_triggered();

    void on_sobelAction_triggered();

private:
    unique_ptr<Management> management;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
