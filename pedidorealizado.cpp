#include "pedidorealizado.h"
#include "ui_pedidorealizado.h"

PedidoRealizado::PedidoRealizado(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PedidoRealizado)
{
    ui->setupUi(this);
}

PedidoRealizado::~PedidoRealizado()
{
    delete ui;
}

void PedidoRealizado::insertarPedido(const Producto p, const bool estado)
{
    ui->nombreLB->setText(p.getNombre());
    ui->cantidadLB->setText("Cantidad: " + QString::number(p.getCantidad()));
    ui->precioLB->setText("Precio Unitario: $" + QString::number(p.getPrecio()));

    if(estado){
        ui->entregadoPB->setEnabled(false);
        ui->eliminarPB->setEnabled(false);
    }

    prod = p;
}

void PedidoRealizado::on_entregadoPB_clicked()
{
    emit sglEntregar(prod);
}


void PedidoRealizado::on_eliminarPB_clicked()
{
    emit sglEliminar(prod);
}

