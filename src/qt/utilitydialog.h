// Copyright (c) 2011-2015 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UTILITYDIALOG_H
#define UTILITYDIALOG_H

#include <QDialog>
#include <QObject>
#include "walletmodel.h"

class BitcoinGUI;
class ClientModel;
class SendCoinsRecipient;

namespace Ui {
    class PaperWalletDialog;
    class ImpPrivKeyDialog;
}


/** "Paper Wallet" dialog box */
class PaperWalletDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaperWalletDialog(QWidget *parent);
    ~PaperWalletDialog();

    void setModel(WalletModel *model);
    void setPrintData(QString strAddress, QString strPubKey, QString strKey);

private:
    Ui::PaperWalletDialog *ui;
    WalletModel *model;
    static const int PAPER_WALLET_READJUST_LIMIT = 20;
    static const int PAPER_WALLET_PAGE_MARGIN = 50;

private slots:
    void on_getNewAddress_clicked(std::string strAddress , std::string strPubKey , std::string strPrivKey);
    void on_printButton_clicked();
};



/** "Import Private Key" dialog box */
class ImpPrivKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImpPrivKeyDialog(QWidget *parent);
    ~ImpPrivKeyDialog();

    void setModel(WalletModel *model);

private:
    Ui::ImpPrivKeyDialog *ui;
    WalletModel *model;


private Q_SLOTS:
    void on_importButton_clicked();
    void on_exitButton_clicked();

};



#endif // UTILITYDIALOG_H