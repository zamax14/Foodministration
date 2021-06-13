#include "productomenu.h"
#include "ui_productomenu.h"

ProductoMenu::ProductoMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoMenu)
{
    ui->setupUi(this);
}

ProductoMenu::~ProductoMenu()
{
    delete ui;
}
