#include "addressbookpage.h"
#include "ui_addressbookpage.h"

#include "addresstablemodel.h"
#include "optionsmodel.h"
#include "bitcoingui.h"
#include "editaddressdialog.h"
#include "csvmodelwriter.h"
#include "guiutil.h"

 #ifdef USE_QRCODE
#include "qrcodedialog.h"
#endif

#ifdef USE_ZXING
#include "snapwidget.h"
#endif
#include "utilitydialog.h"

#include "wallet.h"
#include "walletmodel.h"
#include "base58.h"
#include "init.h" // for pwalletMain

#include <QSortFilterProxyModel>
#include <QClipboard>
#include <QMessageBox>
#include <QMenu>

AddressBookPage::AddressBookPage(Mode mode, Tabs tab, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressBookPage),
    model(0),
    optionsModel(0),
    mode(mode),
    tab(tab)
{
    ui->setupUi(this);

#ifdef Q_OS_MAC // Icons on push buttons are very uncommon on Mac
    ui->newAddressButton->setIcon(QIcon());
    ui->copyToClipboard->setIcon(QIcon());
    ui->deleteButton->setIcon(QIcon());
 #ifdef USE_QRCODE
    ui->importQRCodeButton->setIcon(QIcon());
#endif
    ui->ImpPrivKeyButton->setIcon(QIcon());
#endif

#ifdef USE_QRCODE
    ui->showQRCode->setVisible(true);
    ui->importQRCodeButton->setVisible(false);
#else
    ui->showQRCode->setVisible(false);
    ui->importQRCodeButton->setVisible(false);
#endif
    ui->ImpPrivKeyButton->setVisible(false);

    switch(mode)
    {
    case ForSending:
        connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accept()));
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->setFocus();
        break;
    case ForEditing:
        ui->buttonBox->setVisible(false);
        break;
    }
    switch(tab)
    {
    case SendingTab:
        ui->labelExplanation->setVisible(false);
        ui->deleteButton->setVisible(true);
        ui->signMessage->setVisible(false);
 #ifdef USE_QRCODE
        ui->importQRCodeButton->setVisible(true);
#else
        ui->importQRCodeButton->setVisible(false);
#endif
        break;
    case ReceivingTab:
        ui->deleteButton->setVisible(false);
        ui->signMessage->setVisible(true);
        ui->ImpPrivKeyButton->setVisible(true);
        break;
    }

    // Context menu actions
    QAction *copyAddressAction = new QAction(ui->copyToClipboard->text(), this);
    QAction *copyLabelAction = new QAction(tr("Copy &Label"), this);
    QAction *editAction = new QAction(tr("&Edit"), this);
    QAction *printAction = new QAction(tr("&Print"), this);
#ifdef USE_QRCODE
    QAction *showQRCodeAction = new QAction(ui->showQRCode->text(), this);
#endif
    QAction *signMessageAction = new QAction(ui->signMessage->text(), this);
    QAction *verifyMessageAction = new QAction(ui->verifyMessage->text(), this);
    QAction *copyPriKeyAction = new QAction(tr("Copy Private Key"), this);
    QAction *copyPubKeyAction = new QAction(tr("Copy Public Key"), this);
    QAction *copySecretAction = new QAction(tr("Copy Address KeyID"), this);
    deleteAction = new QAction(ui->deleteButton->text(), this);

    // Build context menu
    contextMenu = new QMenu();
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(editAction);
    if(tab == ReceivingTab)
        contextMenu->addAction(printAction);
    if(tab == SendingTab)
        contextMenu->addAction(deleteAction);

    
    contextMenu->addSeparator();
#ifdef USE_QRCODE
    contextMenu->addAction(showQRCodeAction);
#endif
    if(tab == ReceivingTab)  	{
        contextMenu->addAction(signMessageAction);
        contextMenu->addAction(copyPriKeyAction);
        contextMenu->addAction(copyPubKeyAction);
        contextMenu->addAction(copySecretAction);
    }
    else if(tab == SendingTab)
        contextMenu->addAction(verifyMessageAction);

    // Connect signals for context menu actions
    connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(on_copyToClipboard_clicked()));
    connect(copyLabelAction, SIGNAL(triggered()), this, SLOT(onCopyLabelAction()));
    connect(editAction, SIGNAL(triggered()), this, SLOT(onEditAction()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(on_deleteButton_clicked()));
 #ifdef USE_QRCODE
    connect(showQRCodeAction, SIGNAL(triggered()), this, SLOT(on_showQRCode_clicked()));
#endif
    connect(signMessageAction, SIGNAL(triggered()), this, SLOT(on_signMessage_clicked()));
    connect(verifyMessageAction, SIGNAL(triggered()), this, SLOT(on_verifyMessage_clicked()));
    connect(copyPriKeyAction, SIGNAL(triggered()), this, SLOT(on_copyPriKey_clicked()));
    connect(copyPubKeyAction, SIGNAL(triggered()), this, SLOT(on_copyPubKey_clicked()));
    connect(copySecretAction, SIGNAL(triggered()), this, SLOT(on_copySecKey_clicked()));
    connect(printAction, SIGNAL(triggered()), this, SLOT(onPrintAction()));

    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextualMenu(QPoint)));

    // Pass through accept action from button box
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

AddressBookPage::~AddressBookPage()
{
    delete ui;
}

void AddressBookPage::setModel(AddressTableModel *model)
{
    this->model = model;
    if(!model)
        return;

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    switch(tab)
    {
    case ReceivingTab:
        // Receive filter
        proxyModel->setFilterRole(AddressTableModel::TypeRole);
        proxyModel->setFilterFixedString(AddressTableModel::Receive);
        break;
    case SendingTab:
        // Send filter
        proxyModel->setFilterRole(AddressTableModel::TypeRole);
        proxyModel->setFilterFixedString(AddressTableModel::Send);
        break;
    }
    ui->tableView->setModel(proxyModel);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);

    // Set column widths
    ui->tableView->horizontalHeader()->resizeSection(
            AddressTableModel::Address, 320);
    ui->tableView->horizontalHeader()->setResizeMode(
            AddressTableModel::Label, QHeaderView::Stretch);

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChanged()));

    // Select row for newly created address
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(selectNewAddress(QModelIndex,int,int)));

    selectionChanged();
}

void AddressBookPage::setOptionsModel(OptionsModel *optionsModel)
{
    this->optionsModel = optionsModel;
}

void AddressBookPage::on_copyToClipboard_clicked()
{
    GUIUtil::copyEntryData(ui->tableView, AddressTableModel::Address);
}

void AddressBookPage::onCopyLabelAction()
{
    GUIUtil::copyEntryData(ui->tableView, AddressTableModel::Label);
}

void AddressBookPage::onEditAction()
{
    if(!ui->tableView->selectionModel())
        return;
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
    if(indexes.isEmpty())
        return;

    EditAddressDialog dlg(
            tab == SendingTab ?
            EditAddressDialog::EditSendingAddress :
            EditAddressDialog::EditReceivingAddress);
    dlg.setModel(model);
    QModelIndex origIndex = proxyModel->mapToSource(indexes.at(0));
    dlg.loadRow(origIndex.row());
    dlg.exec();
}

void AddressBookPage::on_signMessage_clicked()
{
    QTableView *table = ui->tableView;
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QString address = index.data().toString();
        emit signMessage(address);
    }
}

void AddressBookPage::on_verifyMessage_clicked()
{
    QTableView *table = ui->tableView;
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QString address = index.data().toString();
        emit verifyMessage(address);
    }
}

void AddressBookPage::on_newAddressButton_clicked()
{
    if(!model)
        return;

    EditAddressDialog dlg(
            tab == SendingTab ?
            EditAddressDialog::NewSendingAddress :
            EditAddressDialog::NewReceivingAddress, this);
    dlg.setModel(model);
    if(dlg.exec())
    {
        newAddressToSelect = dlg.getAddress();
    }
}

void AddressBookPage::on_deleteButton_clicked()
{
    QTableView *table = ui->tableView;
    if(!table->selectionModel())
        return;

    QModelIndexList indexes = table->selectionModel()->selectedRows();
    if(!indexes.isEmpty())
    {
        table->model()->removeRow(indexes.at(0).row());
    }
}

void AddressBookPage::selectionChanged()
{
    // Set button states based on selected tab and selection
    QTableView *table = ui->tableView;
    if(!table->selectionModel())
        return;

    if(table->selectionModel()->hasSelection())
    {
        switch(tab)
        {
        case SendingTab:
            // In sending tab, allow deletion of selection
            ui->deleteButton->setEnabled(true);
            ui->deleteButton->setVisible(true);
            deleteAction->setEnabled(true);
            ui->signMessage->setEnabled(false);
            ui->signMessage->setVisible(false);
            ui->verifyMessage->setEnabled(true);
            ui->verifyMessage->setVisible(true);
            break;
        case ReceivingTab:
            // Deleting receiving addresses, however, is not allowed
            ui->deleteButton->setEnabled(false);
            ui->deleteButton->setVisible(false);
            deleteAction->setEnabled(false);
            ui->signMessage->setEnabled(true);
            ui->signMessage->setVisible(true);
            ui->verifyMessage->setEnabled(false);
            ui->verifyMessage->setVisible(false);
            ui->ImpPrivKeyButton->setVisible(true);
            break;
        }
#ifdef USE_QRCODE
        ui->copyToClipboard->setEnabled(true);
        ui->showQRCode->setEnabled(true);
        ui->importQRCodeButton->setEnabled(true);
#endif
    }
    else
    {
        ui->deleteButton->setEnabled(false);
#ifdef USE_QRCODE
        ui->showQRCode->setEnabled(false);
#endif
        ui->copyToClipboard->setEnabled(false);
        ui->signMessage->setEnabled(false);
        ui->verifyMessage->setEnabled(false);
    }
}

void AddressBookPage::done(int retval)
{
    QTableView *table = ui->tableView;
    if(!table->selectionModel() || !table->model())
        return;
    // When this is a tab/widget and not a model dialog, ignore "done"
    if(mode == ForEditing)
        return;

    // Figure out which address was selected, and return it
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QVariant address = table->model()->data(index);
        returnValue = address.toString();
    }

    if(returnValue.isEmpty())
    {
        // If no address entry selected, return rejected
        retval = Rejected;
    }

    QDialog::done(retval);
}

void AddressBookPage::exportClicked()
{
    // CSV is currently the only supported format
    QString filename = GUIUtil::getSaveFileName(
            this,
            tr("Export Address Book Data"), QString(),
            tr("Comma separated file (*.csv)"));

    if (filename.isNull()) return;

    CSVModelWriter writer(filename);

    // name, column, role
    writer.setModel(proxyModel);
    writer.addColumn("Label", AddressTableModel::Label, Qt::EditRole);
    writer.addColumn("Address", AddressTableModel::Address, Qt::EditRole);

    if(!writer.write())
    {
        QMessageBox::critical(this, tr("Error exporting"), tr("Could not write to file %1.").arg(filename),
                              QMessageBox::Abort, QMessageBox::Abort);
    }
}
#ifdef USE_QRCODE
void AddressBookPage::on_showQRCode_clicked()
{

    QTableView *table = ui->tableView;
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QString address = index.data().toString();
        QString label = index.sibling(index.row(), 0).data(Qt::EditRole).toString();

        QRCodeDialog *dialog = new QRCodeDialog(address, label, tab == ReceivingTab, this);
        dialog->setModel(optionsModel);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }

}


void AddressBookPage::on_importQRCodeButton_clicked()
{
#ifdef USE_ZXING
        SnapWidget* snap = new SnapWidget(this);
        connect(snap, SIGNAL(finished(QString)), this, SLOT(onSnapClosed(QString))); 
#endif
}
 
#endif
void AddressBookPage::onSnapClosed(QString strAddressURL)
{
    if (strAddressURL.size() > 0)
    {
        //to do : some more parsing and validation is needed here
        //todo: prompt for a label
        //todo: display a dialog if it doesn't work
        //todo: 新增收款地址
        //emit importWallet(strAddressURL);
        
        //新增付款地址
		    if (strAddressURL.size() > 34) {
		        QString _address;
		        QString _label;
		        QString _amount;     
		        int x = strAddressURL.indexOf(":", 0, Qt::CaseInsensitive);
		        if (x) {
		            _address = strAddressURL.mid(x+1, 34);
		        }
		        int y = strAddressURL.indexOf("label=", 0, Qt::CaseInsensitive);
		        if (y) {
		        		QString tmpURL= strAddressURL.mid(y+6, strAddressURL.size()-y-6);
		        		int z = tmpURL.indexOf("&", 0, Qt::CaseInsensitive);
		        		
		        		//QMessageBox::warning(this, tr("GR-Snap"), tr("The Address is %1.").arg(tmpURL),  QMessageBox::Ok, QMessageBox::Ok);
		        		if (z) {
		            		_label = tmpURL.mid(0, z);
		          	} else {
		          			_label = tmpURL;
		          	}
		        }
		        
		        //Todo: parse out label and amount from incoming string		        
				    if(!model)
				        return;
				
				    EditAddressDialog dlg(EditAddressDialog::NewSendingAddress, this);
				    dlg.setModel(model);
				    dlg.setAddress(_address);
				    dlg.setLabel(_label);
				    if(dlg.exec())
				    {
				        newAddressToSelect = dlg.getAddress();
				    }		        
		    }
     }
}

void AddressBookPage::contextualMenu(const QPoint &point)
{
    QModelIndex index = ui->tableView->indexAt(point);
    if(index.isValid())
    {
        contextMenu->exec(QCursor::pos());
    }
}

void AddressBookPage::selectNewAddress(const QModelIndex &parent, int begin, int end)
{
    QModelIndex idx = proxyModel->mapFromSource(model->index(begin, AddressTableModel::Address, parent));
    if(idx.isValid() && (idx.data(Qt::EditRole).toString() == newAddressToSelect))
    {
        // Select row of newly created address, once
        ui->tableView->setFocus();
        ui->tableView->selectRow(idx.row());
        newAddressToSelect.clear();
    }
}

void AddressBookPage::on_copyPriKey_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(AddressTableModel::Address);
    if(!selection.isEmpty())
    {
    		if (fWalletUnlockStakingOnly)
    		{
    				QMessageBox::warning(this, windowTitle(), tr("Wallet is unlocked for staking only."), QMessageBox::Ok, QMessageBox::Ok);
            return;
				}
				if (pwalletMain && pwalletMain->IsLocked()) 
				{
    				QMessageBox::warning(this, windowTitle(), tr("Not copying because wallet is locked."), QMessageBox::Ok, QMessageBox::Ok);
            return;
    		}
    		
        QString addrStr = selection.at(0).data(Qt::EditRole).toString();
        CBitcoinAddress address(addrStr.toStdString());
        
        CKeyID keyID;
        if ( !address.GetKeyID(keyID) )
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have keyID ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        CKey vchSecret;
	      if (!pwalletMain->GetKey(keyID, vchSecret))
	      {
	          QMessageBox::warning(this, windowTitle(),
	              tr("Address \"%1\" doesn't have private key ").arg(addrStr),
	              QMessageBox::Ok, QMessageBox::Ok);
	          return;
	      }
        QApplication::clipboard()->setText(CBitcoinSecret(vchSecret).ToString().c_str());
    }
}

void AddressBookPage::on_copyPubKey_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(AddressTableModel::Address);
    if(!selection.isEmpty())
    {
    	
    		if (fWalletUnlockStakingOnly)
    		{
    				QMessageBox::warning(this, windowTitle(), tr("Wallet is unlocked for staking only."), QMessageBox::Ok, QMessageBox::Ok);
            return;
				}
				if (pwalletMain && pwalletMain->IsLocked()) 
				{
    				QMessageBox::warning(this, windowTitle(), tr("Not copying because wallet is locked."), QMessageBox::Ok, QMessageBox::Ok);
            return;
    		}
    		
        QString addrStr = selection.at(0).data(Qt::EditRole).toString();       
        CBitcoinAddress address(addrStr.toStdString());
        CKeyID keyID;
        if ( !address.GetKeyID(keyID) )
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have keyID ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        
        CKey vchSecret;
	      if (!pwalletMain->GetKey(keyID, vchSecret))
	      {
	          QMessageBox::warning(this, windowTitle(),
	              tr("Address \"%1\" doesn't have private key ").arg(addrStr),
	              QMessageBox::Ok, QMessageBox::Ok);
	          return;
	      }	      

				CPubKey vchPubKey;
        if ( !pwalletMain->GetPubKey(keyID, vchPubKey))
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have public key ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        QApplication::clipboard()->setText(HexStr(vchPubKey).c_str());
    }
}

void AddressBookPage::on_copySecKey_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(AddressTableModel::Address);
    if(!selection.isEmpty())
    {
    		if (fWalletUnlockStakingOnly)
    		{
    				QMessageBox::warning(this, windowTitle(), tr("Wallet is unlocked for staking only."), QMessageBox::Ok, QMessageBox::Ok);
            return;
				}
				if (pwalletMain && pwalletMain->IsLocked()) 
				{
    				QMessageBox::warning(this, windowTitle(), tr("Not copying because wallet is locked."), QMessageBox::Ok, QMessageBox::Ok);
            return;
    		}
    		
        QString addrStr = selection.at(0).data(Qt::EditRole).toString();
        CBitcoinAddress address(addrStr.toStdString());
        CKeyID keyID;
        if ( !address.GetKeyID(keyID) )
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have keyID ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        
        CKey vchSecret;
	      if (!pwalletMain->GetKey(keyID, vchSecret))
	      {
	          QMessageBox::warning(this, windowTitle(),
	              tr("Address \"%1\" doesn't have private key ").arg(addrStr),
	              QMessageBox::Ok, QMessageBox::Ok);
	          return;
	      }	      

				CPubKey vchPubKey;
        if ( !pwalletMain->GetPubKey(keyID, vchPubKey))
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have public key ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
       
				CBitcoinSecret vchSecret2;
				bool fGood = vchSecret2.SetString(CBitcoinSecret(vchSecret).ToString());
				if (fGood)  {
		        CKey key = vchSecret2.GetKey();
		        CPubKey pubkey = key.GetPubKey();
		        CKeyID vchAddress = pubkey.GetID();
		        QApplication::clipboard()->setText(QString::fromStdString(vchAddress.ToString()));
      	}
      	else {
      			QMessageBox::warning(this, windowTitle(), tr("Have a mistake at GetPrivKey."), QMessageBox::Ok, QMessageBox::Ok);
            return;
      	}
    }
}

void AddressBookPage::onPrintAction()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows(AddressTableModel::Address);
    if(!selection.isEmpty())
    {
				if (fWalletUnlockStakingOnly)
				{
						QMessageBox::warning(this, windowTitle(), tr("Wallet is unlocked for staking only."), QMessageBox::Ok, QMessageBox::Ok);
		        return;
				}
				if (pwalletMain && pwalletMain->IsLocked()) 
				{
						QMessageBox::warning(this, windowTitle(), tr("Not copying because wallet is locked."), QMessageBox::Ok, QMessageBox::Ok);
		        return;
				}

        QString addrStr = selection.at(0).data(Qt::EditRole).toString();
        CBitcoinAddress address(addrStr.toStdString());

        CKeyID keyID;
        if ( !address.GetKeyID(keyID) )
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have keyID ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        
				CPubKey vchPubKey;
        if ( !pwalletMain->GetPubKey(keyID, vchPubKey))
        {
            QMessageBox::warning(this, windowTitle(),
                tr("Address \"%1\" doesn't have public key ").arg(addrStr),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        
        CKey vchSecret;
	      if (!pwalletMain->GetKey(keyID, vchSecret))
	      {
	          QMessageBox::warning(this, windowTitle(),
	              tr("Address \"%1\" doesn't have private key ").arg(addrStr),
	              QMessageBox::Ok, QMessageBox::Ok);
	          return;
	      }	      

				QString strPubKey = QString::fromStdString(HexStr(vchPubKey));
				QString strSecret = QString::fromStdString(CBitcoinSecret(vchSecret).ToString());
        
        PaperWalletDialog dlg(this);
        dlg.setPrintData(addrStr, strPubKey, strSecret);
        dlg.exec();
    }
}

void AddressBookPage::on_ImpPrivKeyButton_clicked()
{
        ImpPrivKeyDialog dlg(this);
        dlg.exec();
}
