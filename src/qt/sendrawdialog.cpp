//The Whitecoin Developers 2014-2018

#include "sendrawdialog.h"
#include "ui_sendrawdialog.h"

#include "base58.h"
#include "main.h"
#include "init.h"

#include "transactiontablemodel.h"
#include "clientmodel.h"
#include "guiutil.h"

#include <QModelIndex>

using namespace std;
    	
SendRawDialog::SendRawDialog(const QString &txID, const QString &txHEX, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendRawDialog)
{
    ui->setupUi(this);
    
    if (txID.isEmpty() && txHEX.isEmpty())
    	 	return;
    
    if (txHEX.isEmpty())
    {
			    //1. txID
			    QString txIDdesc = txID.left(64);    
			    
			    //2. hex code of transaction(getrawtransaction)
			    uint256 hash;
			    hash.SetHex(txID.toStdString().c_str());
			    
			    CTransaction tx;
			    uint256 hashBlock = 0;
			    if (!GetTransaction(hash, tx, hashBlock))
			    {
							QMessageBox::warning(this, windowTitle(), tr("No information available about transaction"), QMessageBox::Ok, QMessageBox::Ok);
					}
			
			    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
			    ssTx << tx;
			    string strHex = HexStr(ssTx.begin(), ssTx.end());
			    
			    QString txHexCode = QString::fromStdString(strHex);
			    ui->detailText->setText(txHexCode);
		} else  {
					ui->detailText->setText(txHEX);
		}
}

SendRawDialog::~SendRawDialog()
{
    delete ui;
}

void SendRawDialog::setModel(WalletModel *model)
{
    this->model = model;
}

void SendRawDialog::on_exitButton_clicked()
{
    close();
}

void SendRawDialog::on_sendButton_clicked()
{   
    //3. send (sendrawtransaction)
    
    string strHex = ui->detailText->toPlainText().toStdString();
    
    // deserialize binary data stream
    CTransaction tx;
    try {
		    vector<unsigned char> txData(ParseHex(strHex.c_str()));
		    CDataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION);
        ssData >> tx;
    }
    catch (std::exception &e) {
        QMessageBox::warning(this, windowTitle(), tr("TX decode failed"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    uint256 hashTx = tx.GetHash();
    
    
    // See if the transaction is already in a block
    // or in the memory pool:
    CTransaction existingTx;
    uint256 hashBlock = 0;
    if (GetTransaction(hashTx, existingTx, hashBlock))
    {
        if (hashBlock != 0)
        {
            QMessageBox::warning(this, windowTitle(), tr("transaction already in block %1").arg(QString::fromStdString(hashBlock.GetHex())), QMessageBox::Ok, QMessageBox::Ok);
						return;
         }
        // Not in block, but already in the memory pool; will drop through to re-relay it.
    }
    else
    {
        // push to local node
        if (!AcceptToMemoryPool(mempool, tx, true, NULL))
        {
            QMessageBox::warning(this, windowTitle(), tr("TX rejected"), QMessageBox::Ok, QMessageBox::Ok);
        		return;
      	}
    }
    RelayTransaction(tx, hashTx);
		return;
}