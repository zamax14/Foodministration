#include "factura.h"

Factura::Factura()
{

}

int Factura::getId() const
{
    return id;
}

void Factura::setId(int newId)
{
    id = newId;
}

const QString &Factura::getNombre() const
{
    return nombre;
}

void Factura::setNombre(const QString &newNombre)
{
    nombre = newNombre;
}

const QString &Factura::getFecha_hora() const
{
    return fecha_hora;
}

void Factura::setFecha_hora(const QString &newFecha_hora)
{
    fecha_hora = newFecha_hora;
}

float Factura::getIva() const
{
    return iva;
}

void Factura::setIva(float newIva)
{
    iva = newIva;
}

float Factura::getSubtotal() const
{
    return subtotal;
}

void Factura::setSubtotal(float newSubtotal)
{
    subtotal = newSubtotal;
}

float Factura::getTotal() const
{
    return total;
}

void Factura::setTotal(float newTotal)
{
    total = newTotal;
}
