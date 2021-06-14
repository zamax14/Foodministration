#ifndef PEDIDOREALIZADO_H
#define PEDIDOREALIZADO_H

#include <QWidget>
#include "producto.h"

namespace Ui {
class PedidoRealizado;
}

class PedidoRealizado : public QWidget
{
    Q_OBJECT

public:
    explicit PedidoRealizado(QWidget *parent = nullptr);
    ~PedidoRealizado();

    void insertarPedido(const Producto p, const bool estado);

private slots:
    void on_entregadoPB_clicked();

    void on_eliminarPB_clicked();

signals:
    void sglEntregar(const Producto p);
    void sglEliminar(const Producto p);

private:
    Ui::PedidoRealizado *ui;
    Producto prod;
};

#endif // PEDIDOREALIZADO_H
