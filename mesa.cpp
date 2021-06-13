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
