#include "facturaimpresa.h"
#include "ui_facturaimpresa.h"

facturaImpresa::facturaImpresa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::facturaImpresa)
{
    ui->setupUi(this);
}

facturaImpresa::~facturaImpresa()
{
    delete ui;
}

void facturaImpresa::insertarFactura(const Factura f)
{
    ui->nombreLB->setText("Encargado: " + f.getNombre());
    ui->fecha_horaLB->setText(f.getFecha_hora());
    ui->idLB->setText("No. Factura: " + QString::number(f.getId()));
    ui->ivaLB->setText("Iva: " + QString::number(f.getIva()));
    ui->subtotalLB->setText("Subtotal: " + QString::number(f.getSubtotal()));
    ui->totalLB->setText("Total: " + QString::number(f.getTotal()));

    for (int i(0); i<f.Pedidos.size(); ++i) {
        QString formatoPedido = f.Pedidos.at(i).getNombre() + " - " + QString::number(f.Pedidos.at(i).getCantidad()) + " - $" + QString::number(f.Pedidos.at(i).getPrecio()) + " - $" + QString::number(f.Pedidos.at(i).getCantidad()*f.Pedidos.at(i).getPrecio()) + "\n";
        ui->pedidosLB->setText(ui->pedidosLB->text() + formatoPedido);
    }
}
