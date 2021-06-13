#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <QString>

class Producto
{
public:
    Producto();

    int getId() const;
    void setId(int newId);

    const QString &getNombre() const;
    void setNombre(const QString &newNombre);

    float getPrecio() const;
    void setPrecio(float newPrecio);

    const QString &getImagen() const;
    void setImagen(const QString &newImagen);

    const QString &getCantidad() const;
    void setCantidad(const QString &newCantidad);

private:
    int id;
    QString nombre;
    float precio;
    QString imagen;
    QString cantidad;

};

#endif // PRODUCTO_H
