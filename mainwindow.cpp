#include "mainwindow.h"
#include "management.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QtDebug>
#include <QString>
#include <QTextStream>
#include <QLabel>

QTextStream cout(stdout);
QTextStream cin(stdin);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::management = make_unique<Management>();
    management.get()->setImageLabel(ui->imageLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageAction_triggered()
{
    management.get()->loadImage(QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.jpg *.bmp"));
}


void MainWindow::on_saveImageAsAction_triggered()
{
    management.get()->saveImage(QFileDialog::getSaveFileName(0, "Save Dialog", "", "*.jpg *.bmp"));
}

void MainWindow::on_reloadAction_triggered()
{
    management.get()->reloadImage();
}

void MainWindow::on_normalizeAction_triggered()
{
    management.get()->normalize();
}

void MainWindow::on_gaussAction_triggered()
{
    management.get()->gauss(5, 0);
}

void MainWindow::on_sobelAction_triggered()
{
    management.get()->sobel(0);
}
