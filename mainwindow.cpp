#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //realizamos la conexion a la base de datos
    baseDatos = QSqlDatabase::addDatabase("QSQLITE");
    baseDatos.setDatabaseName("D:/Escritorio/Project Foodministration/Foodministration/Base de Datos/beer_para_creer.db");
    //validamos que la base se pueda abrir
    if(!baseDatos.open()){
        QMessageBox::critical(this, "Error", baseDatos.lastError().text());
        return;
    }

    //inicializamos y cargamos los arreglos
    inicializarListaMesas();
    cargarMenuProductos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inicializarListaMesas()
{
    //se realiza un ciclo en el que se llena el array de mesas con mesas en estado apagado
    for (int i(0); i<7; ++i) {
        Mesa m;
        m.setEstado(false);
        listaMesas.push_back(m);
    }
}

void MainWindow::cargarMenuProductos()
{
    //se realiza la consulta sql para extarer todos los datos de la tabla producto
    QSqlQuery qry("SELECT * FROM producto");

    //ciclo que se repetira hasta que ya no haya una siguiente fila
    while (qry.next()) {
        //creamos objeto de tipo producto y la damos los valores correspondientes
        Producto p;
        p.setId(qry.value(0).toInt());
        p.setNombre(qry.value(1).toString());
        p.setPrecio(qry.value(2).toFloat());
        p.setImagen(qry.value(3).toString());
        //hacemos push del objeto p a la lista
        menuProductos.push_back(p);
    }
}
