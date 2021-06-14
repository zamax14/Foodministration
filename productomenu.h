#ifndef PRODUCTOMENU_H
#define PRODUCTOMENU_H

#include <QWidget>
#include <QPainter>
#include "producto.h"

namespace Ui {
class ProductoMenu;
}

class ProductoMenu : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoMenu(QWidget *parent = nullptr);
    ~ProductoMenu();
    void insertarProducto(const Producto &p);

protected:
    void paintEvent(QPaintEvent *pe);

private:
    Ui::ProductoMenu *ui;
    int id;

signals:
    void sglAgregar(const int id, const int cantidad);

private slots:
    void on_cantidadSB_valueChanged(int arg1);
    void on_agregarPB_clicked();
};

#endif // PRODUCTOMENU_H
