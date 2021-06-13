#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *pe)
{
    QPixmap pixmap;
    pixmap.load(":/Imagenes/fondo.jpg");
    QPainter paint(this);
    int widWidth = this->ui->centralwidget->width();
    int widHeight = this->ui->centralwidget->height();
    pixmap = pixmap.scaled(widWidth, widHeight, Qt::KeepAspectRatioByExpanding);
    paint.drawPixmap(0, 0, pixmap);
    QWidget::paintEvent(pe);
}

void MainWindow::on_mesa_1PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_2PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_3PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_4PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_5PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_6PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_mesa_7PB_clicked()
{
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_volverPB_clicked()
{
    ui->pagesSW->setCurrentIndex(0);
}
