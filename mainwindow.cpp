#include "mainwindow.h"
#include "ui_mainwindow.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++CONSTRUCTOR+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //cargarmos el driver para trabajar con bases de datos sqlite
    baseDatos = QSqlDatabase::addDatabase("QSQLITE");

    //le damos la ubicacion del archivo al programa
    QDir path;
    baseDatos.setDatabaseName(QString::fromStdString(path.filePath("beer_para_creer.db").toStdString()));

    //validamos que la base se pueda abrir
    if(!baseDatos.open()){
        QMessageBox::critical(this, "Error", baseDatos.lastError().text());
        return;
    }

    //inicializamos y cargamos los arreglos
    inicializarListaMesas();
    cargarMenuProductos();
    cargarListaFacturas();

    //inicializamos la primera vista de la interfaz
    ui->pagesSW->setCurrentIndex(0);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++FUNCIONES PUBLICAS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
        //hacemos push del objeto 'p' a la lista
        menuProductos.push_back(p);
    }
}

void MainWindow::cargarListaFacturas()
{
    //se realiza la consulta sql para extraer todos los datos del ticket
    QSqlQuery qry("SELECT * FROM ticket");

    //ciclo que se repetira hasta que ya no haya una siguiente fila
    while (qry.next()) {
        //creamos objeto de tipo factura y le damos los valores correspondientes
        Factura f;
        f.setId(qry.value(0).toInt());
        f.setNombre(qry.value(1).toString());
        f.setTelefono(qry.value(2).toString());
        f.setDireccion(qry.value(3).toString());
        f.setFecha_hora(qry.value(4).toString());
        f.setIva(qry.value(5).toFloat());
        f.setSubtotal(qry.value(6).toFloat());
        f.setTotal(qry.value(7).toFloat());

        //se realiza la consulta sql para extraer los datos de los productos de la factura correspondiente
        QSqlQuery qry2(QString("SELECT Producto_Id, Cantidad FROM ticket_producto WHERE Ticket_Id = %1").arg(f.getId()));

        //ciclo que se repetira hasta que ya no haya una siguiente fila
        while (qry2.next()) {
            //creamos objeto de tipo producto y le damos los valores correspondientes
            Producto p;
            p.setId(qry2.value(0).toInt());
            p.setCantidad(qry2.value(1).toInt());
            //realizamos una busqueda en la lista del menu para conseguir los datos faltantes del producto
            for (int i(0); i<menuProductos.size(); ++i) {
                //si el id del producto que estamos buscando coincide con el de la lista, extraemos los datos y rompemos el ciclo
                if(p.getId()==menuProductos.at(i).getId()){
                    p.setNombre(menuProductos.at(i).getNombre());
                    p.setPrecio(menuProductos.at(i).getPrecio());
                    break;
                }
            }
            //hacemos push del producto a la lista de pedidos de la factura
            f.Pedidos.push_back(p);
        }
        //hacemos push del objeto 'f' a la lista
        listaFacturas.push_back(f);
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++FUNCIONES PROTEGIDAS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++SLOTS PRIVADOS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

