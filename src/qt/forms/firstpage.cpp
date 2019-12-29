#include <QUrl>
#include <QDesktopServices>
#include "firstpage.h"
#include "ui_firstpage.h"

firstpage::firstpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstpage)
{
    ui->setupUi(this);
    ui->textBrowser_headline->setText(tr("Welcome to the new community ecology"));
    ui->textBrowser_info->setText(tr("In order to protect the rights and interests of the whole community and investors, the development team of whitecoin keeps the original intention, adheres to the original concept of block chain, and works with the community regardless of the influence of profits and fame."));
    ui->textBrowser_adv->setText(tr("Join the new development group website whitecoin.in. Development team members are with us."));
    ui->label_officelink->setText(tr("Official Link"));
    ui->label_official_media->setText(tr("Official Media"));
}

firstpage::~firstpage()
{
    delete ui;
}

void firstpage::on_toolButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.facebook.com/whitecoin.core"));
}

void firstpage::on_toolButton_2_clicked()
{
    // out link
    QDesktopServices::openUrl(QUrl("https://t.me/joinchat/OvDtOQ_tPccshwFm71nVyg"));
}

void firstpage::on_toolButton_3_clicked()
{
    QDesktopServices::openUrl(QUrl("https://discord.gg/fsWkUMD"));
}

void firstpage::on_toolButton_4_clicked()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/WhitecoinC"));
}

void firstpage::on_toolButton_5_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/WhitecoinCore/whitecoin"));
}
