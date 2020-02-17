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
    ui->textBrowser_info->setText(tr("Whitecoin Introduce"));
    ui->textBrowser_adv->setText(tr("Whitecoin Invitation"));
    ui->label_officelink->setText(tr("Official Link"));
    ui->label_official_media->setText(tr("Official Media"));

    ui->textBrowser_headline->raise();
    ui->textBrowser_info->raise();
    ui->textBrowser_adv->raise();
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
