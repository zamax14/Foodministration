#include "productomenu.h"
#include "ui_productomenu.h"

ProductoMenu::ProductoMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoMenu)
{
    ui->setupUi(this);      
}

ProductoMenu::~ProductoMenu()
{
    delete ui;
}

void ProductoMenu::insertarProducto(const Producto &p)
{
    QPixmap img(":/Imagenes/"+p.getImagen());
    ui->nombreLB->setText(p.getNombre());
    ui->precioLB->setText("$"+QString::number(p.getPrecio()));
    ui->imagenLB->setPixmap(img);
    id = p.getId();
}

void ProductoMenu::paintEvent(QPaintEvent *pe)
{
    QPixmap pixmap;
    pixmap.load(":/Imagenes/fondo.jpg");
    QPainter paint(this);
    pixmap = pixmap.scaled(150, 220, Qt::KeepAspectRatioByExpanding);
    paint.drawPixmap(0, 0, pixmap);
    QWidget::paintEvent(pe);
}

void ProductoMenu::on_cantidadSB_valueChanged(int arg1)
{
    if(arg1)
        ui->agregarPB->setEnabled(true);
    else
        ui->agregarPB->setEnabled(false);
}

void ProductoMenu::on_agregarPB_clicked()
{
    emit sglAgregar(id, ui->cantidadSB->value());
    ui->cantidadSB->setValue(0);
}

