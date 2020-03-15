#include <QUrl>
#include <QDesktopServices>
#include "firstpage.h"
#include "ui_firstpage.h"

firstpage::firstpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstpage)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    QFont uiFont("PingFangSC-Light");
#else
    QFont uiFont("SimHei");
#endif

    uiFont.setPixelSize(30);
    uiFont.setWeight(QFont::Normal);
    ui->textBrowser_headline->setFont(uiFont);

     int widthHeadLine = ui->textBrowser_headline->fontMetrics().width(tr("社区新生态，欢  "));

    QString testFontStr(tr("Official Website:"));
    if(testFontStr == QString("Official Website:"))
    {
        uiFont.setFamily("Ubuntu");
         widthHeadLine = ui->textBrowser_headline->fontMetrics().width(tr("Welcome to the new    "));
    }
    ui->textBrowser_headline->setFixedWidth(widthHeadLine);
    ui->textBrowser_headline->setText(tr("Welcome to the new community ecology"));
    ui->textBrowser_headline->adjustSize();

#ifdef Q_OS_MAC
    uiFont.setPixelSize(13);
    uiFont.setWeight(QFont::ExtraLight);
#else
    uiFont.setPixelSize(16);
#endif
    ui->textBrowser_info->setFont(uiFont);
    int widthBrowser = ui->textBrowser_info->fontMetrics().width(tr("白币技术团队是由来自世界各地的技术精"));
    if(testFontStr == QString("Official Website:"))
             widthBrowser = ui->textBrowser_info->fontMetrics().width(tr("The whitecoin team is deeply involved in blockchain"));
    ui->textBrowser_info->setFixedWidth(widthBrowser);
    ui->textBrowser_info->setText(tr("Whitecoin Introduce"));

//   ui->textBrowser_adv->setFont(QFont("SimHei", 12, QFont::Normal));
//    ui->textBrowser_adv->setFixedWidth(width);
//    ui->textBrowser_adv->setText(tr("Whitecoin Invitation"));

    ui->label_officelink->setFont(uiFont);
    ui->label_officelink->setText(tr("Official Website:"));
    ui->label_officelink->adjustSize();
    QRect officeLableRect = ui->label_officelink->geometry();

    QRect officelinkRect = ui->label_official_link->geometry();
    officelinkRect.setLeft(officeLableRect.right()+5);
    ui->label_official_link->setGeometry(officelinkRect.left(),officelinkRect.top(),officelinkRect.right(),officelinkRect.bottom());

    ui->label_official_link->setFont(uiFont);
    ui->label_official_link->adjustSize();


    ui->label_official_media->setFont(uiFont);
    ui->label_official_media->setText(tr("Official Media"));
    ui->label_official_media->adjustSize();


    ui->textBrowser_headline->raise();
    ui->textBrowser_info->raise();
//    ui->textBrowser_adv->raise();
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
void firstpage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Show warning if this is a prerelease version
        connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        updateAlerts(model->getStatusBarWarnings());
    }
}

void firstpage::updateAlerts(const QString &warnings)
{
    this->ui->labelAlerts->setVisible(!warnings.isEmpty());
    this->ui->labelAlerts->setText(warnings);
    //this->ui->labelAlerts->adjustSize();
}
