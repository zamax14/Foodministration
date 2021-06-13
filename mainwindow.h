#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *pe);
private slots:
    void on_mesa_1PB_clicked();

    void on_mesa_2PB_clicked();

    void on_mesa_3PB_clicked();

    void on_mesa_4PB_clicked();

    void on_mesa_5PB_clicked();

    void on_mesa_6PB_clicked();

    void on_mesa_7PB_clicked();

    void on_volverPB_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
