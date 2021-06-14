#ifndef FACTURAIMPRESA_H
#define FACTURAIMPRESA_H

#include <QWidget>
#include "factura.h"

namespace Ui {
class facturaImpresa;
}

class facturaImpresa : public QWidget
{
    Q_OBJECT

public:
    explicit facturaImpresa(QWidget *parent = nullptr);
    ~facturaImpresa();
    void insertarFactura(const Factura f);

private:
    Ui::facturaImpresa *ui;
    Factura fact;
};

#endif // FACTURAIMPRESA_H
