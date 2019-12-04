#include "firstpage.h"
#include "ui_firstpage.h"

firstpage::firstpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstpage)
{
    ui->setupUi(this);
}

firstpage::~firstpage()
{
    delete ui;
}
