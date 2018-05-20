#ifndef SENDRAWDIALOG_H
#define SENDRAWDIALOG_H

#include <QDialog>
#include <QObject>
#include "walletmodel.h"

namespace Ui {
    class SendRawDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE


/** Dialog send hex code of transaction */
class SendRawDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendRawDialog(const QString &txID, const QString &txHEX, QWidget *parent = 0);
    ~SendRawDialog();
    
    void setModel(WalletModel *model);

private:
    Ui::SendRawDialog *ui;
    
    WalletModel *model;
    	
private Q_SLOTS:
    void on_sendButton_clicked();
    void on_exitButton_clicked();
};

#endif // SENDRAWDIALOG_H
