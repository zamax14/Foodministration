#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include <QList>
#include <QVarLengthArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <iostream>

#include "producto.h"
#include "factura.h"
#include "mesa.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void inicializarListaMesas();
    void cargarMenuProductos();

private:
    Ui::MainWindow *ui;
    QList<Producto> menuProductos;
    QList<Factura> listaFacturas;
    QVarLengthArray<Mesa, 7> listaMesas;
    QSqlDatabase baseDatos;
};
#endif // MAINWINDOW_H
