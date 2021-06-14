#ifndef FACTURA_H
#define FACTURA_H
#include <QString>
#include <QList>
#include "producto.h"

class Factura
{
public:
    Factura();

    int getId() const;
    void setId(int newId);

    const QString &getNombre() const;
    void setNombre(const QString &newNombre);

    const QString &getFecha_hora() const;
    void setFecha_hora(const QString &newFecha_hora);

    float getIva() const;
    void setIva(float newIva);

    float getSubtotal() const;
    void setSubtotal(float newSubtotal);

    float getTotal() const;
    void setTotal(float newTotal);

    QList<Producto> Pedidos;

private:
    int id;
    QString nombre;
    QString fecha_hora;
    float iva;
    float subtotal;
    float total;
};

#endif // FACTURA_H
