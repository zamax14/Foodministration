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

    int getCantidad() const;
    void setCantidad(int newCantidad);

private:
    int id;
    QString nombre;
    float precio;
    QString imagen;
    int cantidad;

};

#endif // PRODUCTO_H
