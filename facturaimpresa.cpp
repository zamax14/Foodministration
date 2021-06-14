#include "facturaimpresa.h"
#include "ui_facturaimpresa.h"

facturaImpresa::facturaImpresa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::facturaImpresa)
{
    ui->setupUi(this);
}

facturaImpresa::~facturaImpresa()
{
    delete ui;
}
