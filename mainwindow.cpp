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
    baseDatos.setDatabaseName("beer_para_creer.db");

    //validamos que la base se pueda abrir
    if(!baseDatos.open()){
        QMessageBox::critical(this, "Error", baseDatos.lastError().text());
        return;
    }

    //inicializamos y cargamos los arreglos
    inicializarListaMesas();
    cargarMenuProductos();
    cargarListaFacturas();

    //imprimimos el menu
    imprimirMenu();

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
        m.setEncargado("Mesero");
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
        f.setFecha_hora(qry.value(2).toString());
        f.setIva(qry.value(3).toFloat());
        f.setSubtotal(qry.value(4).toFloat());
        f.setTotal(qry.value(5).toFloat());

        //se realiza la consulta sql para extraer los datos de los productos de la factura correspondiente
        QSqlQuery qry2(QString("SELECT Producto_Id, Cantidad FROM ticket_producto WHERE Ticket_Id = %1").arg(f.getId()));

        //ciclo que se repetira hasta que ya no haya una siguiente fila
        while (qry2.next()) {
            //creamos objeto de tipo producto y le damos los valores correspondientes
            Producto p;
            p.setId(qry2.value(0).toInt());
            p.setCantidad(qry2.value(1).toInt());
            p.setNombre(menuProductos.at(p.getId()-1).getNombre());
            p.setPrecio(menuProductos.at(p.getId()-1).getPrecio());

            //hacemos push del producto a la lista de pedidos de la factura
            f.Pedidos.push_back(p);

            //aumentamos la variable que guarda referencia del id de la tabla ticket_producto
            ++id_Ticket_Producto;
        }
        //hacemos push del objeto 'f' a la lista
        listaFacturas.push_back(f);
    }
}

void MainWindow::almacenarFacturaEnBD(const Factura f)
{
    QSqlQuery qry(QString("INSERT INTO ticket(Id, Nombre, Fecha_hora, Iva, Subtotal, Total) VALUES (%1, '%2', '%3', %4, %5, %6)")
                  .arg(f.getId()).arg(f.getNombre()).arg(f.getFecha_hora()).arg(f.getIva()).arg(f.getSubtotal()).arg(f.getTotal()));

    for (int i(0); i<f.Pedidos.size(); ++i) {
        QSqlQuery qry2(QString("INSERT INTO ticket_producto (Id, Ticket_Id, Producto_Id, Cantidad) VALUES (%1, %2, %3, %4)")
                       .arg(id_Ticket_Producto).arg(f.getId()).arg(f.Pedidos.at(i).getId()).arg(f.Pedidos.at(i).getCantidad()));
        ++id_Ticket_Producto;
    }
}

void MainWindow::imprimirMenu()
{
    //recorremos toda la lista del menu productos
    for (int i(0); i<menuProductos.size(); ++i) {
        //creamos un objeto de tipo ProductosMenu
        ProductoMenu* pm = new ProductoMenu;
        pm->insertarProducto(menuProductos.at(i));
        connect(pm, SIGNAL(sglAgregar(int, int)), this, SLOT(on_agregarPedidoSignal(int, int)));
        ui->menuGL->addWidget(pm, i/5, i%5, Qt::AlignCenter);
    }
}

void MainWindow::imprimirPedidos()
{
    QLayoutItem* child;
    while((child = ui->pedidosGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    int contF = 0;
    QList<Producto> pedidos = listaMesas.at(mesaActual).getPedidosPendientes();
    for (int i(0); i<pedidos.size(); ++i) {
        PedidoRealizado* pr = new PedidoRealizado();
        pr->insertarPedido(pedidos.at(i), false);
        connect(pr, SIGNAL(sglEntregar(Producto)), this, SLOT(on_entregarPedidoSignal(Producto)));
        connect(pr, SIGNAL(sglEliminar(Producto)), this, SLOT(on_eliminarPedidoSignal(Producto)));
        ui->pedidosGL->addWidget(pr,contF,1);

        ++contF;
    }

    pedidos = listaMesas.at(mesaActual).getPedidosEntregados();
    for (int i(0); i<pedidos.size(); ++i) {
        PedidoRealizado* pr = new PedidoRealizado();
        pr->insertarPedido(pedidos.at(i), true);
        ui->pedidosGL->addWidget(pr,contF,1);
        ++contF;
    }
}

void MainWindow::validarGeneracionFactura()
{
    if(listaMesas[mesaActual].listaEntregadosVacia()){
        QMessageBox msg;
        msg.setText("No hay pedidos entregados.");
        msg.exec();
    }
    else if(!listaMesas[mesaActual].listaPendientesVacia()){
        QMessageBox msg;
        msg.setText("Aun hay pedidos pendientes por entregar.");
        msg.exec();
    }
    else if(listaMesas.at(mesaActual).getEncargado() == "Mesero"){
        QMessageBox msg;
        msg.setText("No se selecciono mesero.");
        msg.exec();
    }
    else{
        float total, iva, subtotal=0;
        Factura f;
        f.setId(listaFacturas.size()+1);
        f.setNombre(listaMesas.at(mesaActual).getEncargado());
        f.setFecha_hora(QDateTime::currentDateTime().toString());
        f.Pedidos = listaMesas.at(mesaActual).getPedidosEntregados();

        for (int i(0); i<f.Pedidos.size(); ++i)
            subtotal += f.Pedidos.at(i).getCantidad() * f.Pedidos.at(i).getPrecio();

        iva = subtotal * 0.16;
        total = subtotal + iva;

        f.setSubtotal(subtotal);
        f.setIva(iva);
        f.setTotal(total);

        listaFacturas.push_back(f);
        almacenarFacturaEnBD(f);

        imprimirFactura();

        Mesa m;
        m.setEstado(false);
        m.setEncargado("Mesero");
        listaMesas.replace(mesaActual,m);
    }
}

void MainWindow::imprimirFactura()
{
    QLayoutItem* child;
    while((child = ui->facturaGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    facturaImpresa* fi = new facturaImpresa();
    fi->insertarFactura(listaFacturas.back());
    ui->facturaGL->addWidget(fi);

    ui->pagesSW->setCurrentIndex(3);
}

void MainWindow::imprimirHistorialDeFacturas()
{
    QLayoutItem* child;
    while((child = ui->facturaGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    for (int i(0); i<listaFacturas.size(); ++i) {
        facturaImpresa* fi = new facturaImpresa();
        fi->insertarFactura(listaFacturas.at(i));
        ui->facturaGL->addWidget(fi);
    }

    ui->pagesSW->setCurrentIndex(3);

}

void MainWindow::actualizarSemaforos(const int pos)
{
    switch (pos) {

    case 0:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 1:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 2:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 3:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 4:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 5:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    case 6:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM7->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM7->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM7->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
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
    //se coloca el index de la mesa segun la seleccionada
    mesaActual = 0;

    //se revisa en la lista de mesas si es que la mesa seleccionada esta habilitada
    //en caso de que lo este, se cambia a la vista del menu, se coloca el numero de mesa en la etiqueta y el mesero correspondiente
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        //imprimimos los pedidos de la mesa
        imprimirPedidos();
    }
    //de no ser el caso, se pasa a la vista de la mesa en estado apagado
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_2PB_clicked()
{
    mesaActual = 1;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_3PB_clicked()
{
    mesaActual = 2;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_4PB_clicked()
{
    mesaActual = 3;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_5PB_clicked()
{
    mesaActual = 4;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_6PB_clicked()
{
    mesaActual = 5;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_mesa_7PB_clicked()
{
    mesaActual = 6;
    if(listaMesas.at(mesaActual).getEstado()){
        ui->pagesSW->setCurrentIndex(1);
        ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
        ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
        imprimirPedidos();
    }
    else{
        ui->pagesSW->setCurrentIndex(2);
    }
}


void MainWindow::on_volverPB_clicked()
{
    //se regresa a la vista del menu principal
    ui->pagesSW->setCurrentIndex(0);
}


void MainWindow::on_activarMesaPB_clicked()
{
    //cambia el estado de la mesa correspondiente a true
    listaMesas[mesaActual].setEstado(true);
    //cambia la vista de la interfaz al menu y coloca en la etiqueta el numero de mesa correspondiente
    ui->pagesSW->setCurrentIndex(1);
    ui->mesaLB->setText("MESA: "+QString::number(mesaActual+1));
    ui->meserosCB->setCurrentText(listaMesas.at(mesaActual).getEncargado());
    //imprimimos los pedidos de la mesa
    imprimirPedidos();
}


void MainWindow::on_regresarPB_clicked()
{
    //se regresa a la vista del menu principal
    ui->pagesSW->setCurrentIndex(0);
}


void MainWindow::on_agregarPedidoSignal(int id, int cantidad)
{
    //se crea objeto producto y se colocan sus datos correspondientes
    Producto p;
    p.setId(id);
    p.setCantidad(cantidad);
    p.setNombre(menuProductos.at(id-1).getNombre());
    p.setPrecio(menuProductos.at(id-1).getPrecio());
    //se llama a la funcion de la mesa que añade producto a pendiente
    listaMesas[mesaActual].ingresarProductoPendiente(p);

    //imprimimos los pedidos
    imprimirPedidos();
}


void MainWindow::on_entregarPedidoSignal(Producto p)
{
    listaMesas[mesaActual].moverProductoAEntregado(p);
    imprimirPedidos();
}


void MainWindow::on_eliminarPedidoSignal(Producto p)
{
    listaMesas[mesaActual].eliminarProductoPendiente(p);
    imprimirPedidos();
}


void MainWindow::on_meserosCB_currentTextChanged(const QString &arg1)
{
    listaMesas[mesaActual].setEncargado(arg1);
}


void MainWindow::on_facturaPB_clicked()
{
    validarGeneracionFactura();
}


void MainWindow::on_okPB_clicked()
{
    ui->pagesSW->setCurrentIndex(0);
}


void MainWindow::on_historialFacturasPB_clicked()
{
    imprimirHistorialDeFacturas();
}


void MainWindow::on_apagarPB_clicked()
{
    Mesa m;
    m.setEstado(false);
    m.setEncargado("Mesero");
    listaMesas.replace(mesaActual,m);
    ui->pagesSW->setCurrentIndex(0);
}

void MainWindow::on_pagesSW_currentChanged(int arg1)
{
    if(arg1 == 0)
        //llamamos a la funcion que actualiza el estado grafico de la mesa
        actualizarSemaforos(mesaActual);
}

void MainWindow::on_anteriorPB_clicked()
{
    if(ayudaCont == 1)
        ui->anteriorPB->setEnabled(false);

    ui->siguientePB->setEnabled(true);
    --ayudaCont;
    ui->ayudaSW->setCurrentIndex(ayudaCont);

}

void MainWindow::on_siguientePB_clicked()
{
    if (ayudaCont == 10)
        ui->siguientePB->setEnabled(false);

    ui->anteriorPB->setEnabled(true);
    ++ayudaCont;
    ui->ayudaSW->setCurrentIndex(ayudaCont);

}

void MainWindow::on_regresarAPB_clicked()
{
    ui->pagesSW->setCurrentIndex(0);
    ayudaCont = 0;
}

void MainWindow::on_helpPB_clicked()
{
    ui->pagesSW->setCurrentIndex(4);
    ui->ayudaSW->setCurrentIndex(0);
    ui->anteriorPB->setEnabled(false);

}
