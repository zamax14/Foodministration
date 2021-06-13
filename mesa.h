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

    QList<Producto> pedidosPendientes;
    QList<Producto> pedidosEntregados;

private:
    QString encargado;
    bool estado;
};

#endif // MESA_H
