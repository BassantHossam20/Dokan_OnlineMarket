#include "ProductInSearchGuiWidget.h"
#include "ui_ProductInSearchGuiWidget.h"

ProductInSearchGuiWidget::ProductInSearchGuiWidget(Product *p,  QWidget *parent) :
    QWidget(parent), ui(new Ui::ProductInSearchGuiWidget)
{
    ui->setupUi(this);

    ui->lb_name->setText(p->Name.c_str());
    ui->lb_offerPrice->setText(to_string(p->Price).c_str());
    QPixmap pix(":/img/assets/img/Products/100.jpg");
    if(pix.height()>pix.width())
        ui->lb_photo->setGeometry(20, 10, 40, 60);
    else if (pix.height() == pix.width())
        ui->lb_photo->setGeometry(20, 10, 60, 60);
    else
        ui->lb_photo->setGeometry(20, 10, 60, 40);

    ui->lb_photo->setPixmap(pix);
}

ProductInSearchGuiWidget::~ProductInSearchGuiWidget()
{
    delete ui;
}
