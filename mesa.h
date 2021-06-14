#ifndef MESA_H
#define MESA_H
#include <QString>
#include <QList>
#include "producto.h"

class Mesa
{
public:
    Mesa();

    const QString &getEncargado() const;
    void setEncargado(const QString &newEncargado);

    bool getEstado() const;
    void setEstado(bool newEstado);

    void ingresarProductoPendiente(const Producto p);
    void moverProductoAEntregado(const Producto p);
    void eliminarProductoPendiente(const Producto p);
    bool existenEntregados();
    bool existenPendientes();

    const QList<Producto> &getPedidosPendientes() const;

    const QList<Producto> &getPedidosEntregados() const;

private:
    QString encargado;
    bool estado;

    QList<Producto> pedidosPendientes;
    QList<Producto> pedidosEntregados;
};

#endif // MESA_H
