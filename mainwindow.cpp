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
    //consulta de sql para insertar los datos de la factura a la tabla correspondiente a la base de datos
    QSqlQuery qry(QString("INSERT INTO ticket(Id, Nombre, Fecha_hora, Iva, Subtotal, Total) VALUES (%1, '%2', '%3', %4, %5, %6)")
                  .arg(f.getId()).arg(f.getNombre()).arg(f.getFecha_hora()).arg(f.getIva()).arg(f.getSubtotal()).arg(f.getTotal()));

    //ciclo for en el que agregamos a la tabla de ticket_producto cada uno de los productos de la factura
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
        //creamos un objeto de tipo ProductosMenu y lo insertamos en la interfaz
        ProductoMenu* pm = new ProductoMenu;
        pm->insertarProducto(menuProductos.at(i));
        connect(pm, SIGNAL(sglAgregar(int, int)), this, SLOT(on_agregarPedidoSignal(int, int)));
        ui->menuGL->addWidget(pm, i/5, i%5, Qt::AlignCenter);
    }
}

void MainWindow::imprimirPedidos()
{
    //limpiamos el recuadro de los productos
    QLayoutItem* child;
    while((child = ui->pedidosGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    //realizamos un ciclo for en el que imprimimos todos los productos pendientes
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

    //realizamos un ciclo for en el que imprimimos todos los productos entregados
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
    //si la mesa no tiene productos entregados, mostramos mensaje de alerta
    if(listaMesas[mesaActual].listaEntregadosVacia()){
        QMessageBox msg;
        msg.setText("No hay pedidos entregados.");
        msg.exec();
    }
    //si la mesa tiene pedidos pedientes, mostrsamos mensaje de alerta
    else if(!listaMesas[mesaActual].listaPendientesVacia()){
        QMessageBox msg;
        msg.setText("Aun hay pedidos pendientes por entregar.");
        msg.exec();
    }
    //si la mesa no tiene encargado seleccionada, mostramos mensaje de alerta
    else if(listaMesas.at(mesaActual).getEncargado() == "Mesero"){
        QMessageBox msg;
        msg.setText("No se selecciono mesero.");
        msg.exec();
    }
    //si no hubo error, entonces generamos la factura
    else{
        float total, iva, subtotal=0;
        //creamos objeto tipo factura y le pasamos todos los datos correspondientes
        Factura f;
        f.setId(listaFacturas.size()+1);
        f.setNombre(listaMesas.at(mesaActual).getEncargado());
        f.setFecha_hora(QDateTime::currentDateTime().toString());
        f.Pedidos = listaMesas.at(mesaActual).getPedidosEntregados();

        //calculamos el subtotal sumando cada uno de los pedidos
        for (int i(0); i<f.Pedidos.size(); ++i)
            subtotal += f.Pedidos.at(i).getCantidad() * f.Pedidos.at(i).getPrecio();
        //calculamos iva
        iva = subtotal * 0.16;
        //calculamos subtotal
        total = subtotal + iva;

        //añadimos los datos faltantes al objeto factura
        f.setSubtotal(subtotal);
        f.setIva(iva);
        f.setTotal(total);

        //lo agregamos a la lista de facturas
        listaFacturas.push_back(f);
        //llamamos a la funcion que realiza las consultas sql de la factura que se genero
        almacenarFacturaEnBD(f);
        //llamamos a la funcion que imprime la factura
        imprimirFactura();

        //limpiamos la mesa
        Mesa m;
        m.setEstado(false);
        m.setEncargado("Mesero");
        listaMesas.replace(mesaActual,m);
    }
}

void MainWindow::imprimirFactura()
{
    //limpiamos la seccion de facturas en la interfaz
    QLayoutItem* child;
    while((child = ui->facturaGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    //creamos objeto facturaImpresa y lo mandamos a la interfaz
    facturaImpresa* fi = new facturaImpresa();
    fi->insertarFactura(listaFacturas.back());
    ui->facturaGL->addWidget(fi);

    //cambiamos la vista de la interfaz a la parte de las facturas
    ui->pagesSW->setCurrentIndex(3);
}

void MainWindow::imprimirHistorialDeFacturas()
{
    //limpiamos la seccion de facturas en la interfaz
    QLayoutItem* child;
    while((child = ui->facturaGL->takeAt(0))!=0)
    {
        delete child->widget();
    }

    //ciclo que recorre la lista de facturas
    for (int i(0); i<listaFacturas.size(); ++i) {
        //creamos objeto facturaImpresa y lo mandamos a la interfaz
        facturaImpresa* fi = new facturaImpresa();
        fi->insertarFactura(listaFacturas.at(i));
        ui->facturaGL->addWidget(fi);
    }
    //cambiamos la vista de la interfaz a la parte de las facturas
    ui->pagesSW->setCurrentIndex(3);

}

void MainWindow::actualizarSemaforos(const int pos)
{
    //se hace un switch case para saber que mesa se debe de actualizar su semaforo correspondiente
    switch (pos) {
    //mesa 1
    case 0:
        //si la mesa está apagada, se coloca la alerta roja
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        //si tiene pedidos pendientes, se coloca la alerta amarilla
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        //si no paso ningun caso anterior, se coloca la alerta verda
        else
            ui->semaforoM1->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 2
    case 1:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM2->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 3
    case 2:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM3->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 4
    case 3:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM4->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 5
    case 4:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM5->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 6
    case 5:
        if(listaMesas.at(pos).getEstado() == false)
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/apagada.png"));
        else if(!listaMesas[pos].listaPendientesVacia())
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/exclamacion.png"));
        else
            ui->semaforoM6->setPixmap(QPixmap(":/Imagenes/activa.png"));
        break;
    //mesa 7
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

//funcion nativa de Qt para poner un fondo a la interfaz
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

//mismo caso que on_mesa_1PB_clicked()
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

//mismo caso que on_mesa_1PB_clicked()
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

//mismo caso que on_mesa_1PB_clicked()
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

//mismo caso que on_mesa_1PB_clicked()
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

//mismo caso que on_mesa_1PB_clicked()
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

//mismo caso que on_mesa_1PB_clicked()
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
    //se llama a la funcion de la mesa que cambia un producto listo a entregado
    listaMesas[mesaActual].moverProductoAEntregado(p);
    //se reimprime la seccion de pedidos
    imprimirPedidos();
}


void MainWindow::on_eliminarPedidoSignal(Producto p)
{
    //se llama a la funcion de la mesa que elimina un producto pendiente
    listaMesas[mesaActual].eliminarProductoPendiente(p);
    //se reimprime la seccion de pedidos
    imprimirPedidos();
}


void MainWindow::on_meserosCB_currentTextChanged(const QString &arg1)
{
    //asigna a la mesa el mesero que se eligio
    listaMesas[mesaActual].setEncargado(arg1);
}


void MainWindow::on_facturaPB_clicked()
{
    //llama a la funcion de validar la generacion de facturas cuando se presiona el boton correspondiente
    validarGeneracionFactura();
}


void MainWindow::on_okPB_clicked()
{
    //regresa la vista de la interfaz al menu principal
    ui->pagesSW->setCurrentIndex(0);
}


void MainWindow::on_historialFacturasPB_clicked()
{
    //llama a la funcion que imprime todas las facturas
    imprimirHistorialDeFacturas();
}


void MainWindow::on_apagarPB_clicked()
{
    //resetea la mesa que se desea apagar y se regresa al menu principal
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
    //si el contador de la pestaña de ayuda es igual a 1, el boton de regresar se apaga
    if(ayudaCont == 1)
        ui->anteriorPB->setEnabled(false);
    //enciende el boton de siguiente
    ui->siguientePB->setEnabled(true);
    //disminiye el contador de la pestaña de ayuda
    --ayudaCont;
    //cambia la pagina de la pestaña de ayuda
    ui->ayudaSW->setCurrentIndex(ayudaCont);

}

void MainWindow::on_siguientePB_clicked()
{
    //si el contador de la pestaña de ayuda es igual a 10, el boton de siguiente se apaga
    if (ayudaCont == 10)
        ui->siguientePB->setEnabled(false);
    //enciende el boton de anterior
    ui->anteriorPB->setEnabled(true);
    //aumenta el contador de la pestaña de ayuda
    ++ayudaCont;
    //cambia la pagina de la pestaña de ayuda
    ui->ayudaSW->setCurrentIndex(ayudaCont);

}

void MainWindow::on_regresarAPB_clicked()
{
    //regresa al menu principal y regresa el contador de la pestaña de ayuda a 0
    ui->siguientePB->setEnabled(true);
    ui->pagesSW->setCurrentIndex(0);
    ayudaCont = 0;
}

void MainWindow::on_helpPB_clicked()
{
    //cambia la pestaña de la interfaz a la de ayuda
    ui->pagesSW->setCurrentIndex(4);
    ui->ayudaSW->setCurrentIndex(0);
    ui->anteriorPB->setEnabled(false);

}
