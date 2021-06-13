#include "producto.h"

Producto::Producto()
{

}

int Producto::getId() const
{
    return id;
}

void Producto::setId(int newId)
{
    id = newId;
}

const QString &Producto::getNombre() const
{
    return nombre;
}

void Producto::setNombre(const QString &newNombre)
{
    nombre = newNombre;
}

float Producto::getPrecio() const
{
    return precio;
}

void Producto::setPrecio(float newPrecio)
{
    precio = newPrecio;
}

const QString &Producto::getImagen() const
{
    return imagen;
}

void Producto::setImagen(const QString &newImagen)
{
    imagen = newImagen;
}

int Producto::getCantidad() const
{
    return cantidad;
}

void Producto::setCantidad(int newCantidad)
{
    cantidad = newCantidad;
}

