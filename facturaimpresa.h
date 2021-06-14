#ifndef FACTURAIMPRESA_H
#define FACTURAIMPRESA_H

#include <QWidget>
#include <factura.h>


namespace Ui {
class facturaImpresa;
}

class facturaImpresa : public QWidget
{
    Q_OBJECT

public:
    explicit facturaImpresa(QWidget *parent = nullptr);
    ~facturaImpresa();

private:
    Ui::facturaImpresa *ui;
};

#endif // FACTURAIMPRESA_H
