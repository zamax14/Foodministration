#include "mesa.h"

Mesa::Mesa()
{

}

const QString &Mesa::getEncargado() const
{
    return encargado;
}

void Mesa::setEncargado(const QString &newEncargado)
{
    encargado = newEncargado;
}

bool Mesa::getEstado() const
{
    return estado;
}

void Mesa::setEstado(bool newEstado)
{
    estado = newEstado;
}

const QList<Producto> &Mesa::getPedidosPendientes() const
{
    return pedidosPendientes;
}

const QList<Producto> &Mesa::getPedidosEntregados() const
{
    return pedidosEntregados;
}

void Mesa::ingresarProductoPendiente(const Producto p)
{
    int i;
    for (i = 0; i<pedidosPendientes.size(); ++i) {
        if(p.getId() == pedidosPendientes.at(i).getId()){
            pedidosPendientes[i].setCantidad(pedidosPendientes[i].getCantidad() + p.getCantidad());
            break;
        }
    }

    if(i == pedidosPendientes.size())
        pedidosPendientes.push_back(p);

}

void Mesa::moverProductoAEntregado(const Producto p)
{
    for (int j(0); j<pedidosPendientes.size(); ++j) {
        if(p.getId() == pedidosPendientes.at(j).getId()){

            int i;
            for (i = 0; i<pedidosEntregados.size(); ++i) {
                if(p.getId() == pedidosEntregados.at(i).getId()){
                    pedidosEntregados[i].setCantidad(pedidosEntregados[i].getCantidad() + p.getCantidad());
                    break;
                }
            }

            if(i == pedidosEntregados.size())
                pedidosEntregados.push_back(p);

            pedidosPendientes.removeAt(j);
        }
    }
}

void Mesa::eliminarProductoPendiente(const Producto p)
{
    for (int i(0); i<pedidosPendientes.size(); ++i) {
        if(p.getId() == pedidosPendientes.at(i).getId())
            pedidosPendientes.removeAt(i);
    }
}
