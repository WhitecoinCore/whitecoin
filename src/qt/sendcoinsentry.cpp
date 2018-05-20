#include "sendcoinsentry.h"
#include "ui_sendcoinsentry.h"

#include "guiutil.h"
#include "bitcoinunits.h"
#include "addressbookpage.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "addresstablemodel.h"

#include "script.h"

#include <QApplication>
#include <QClipboard>

SendCoinsEntry::SendCoinsEntry(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SendCoinsEntry),
    model(0)
{
    ui->setupUi(this);

#ifdef Q_OS_MAC
    ui->payToLayout->setSpacing(4);
#endif
#if QT_VERSION >= 0x040700
    /* Do not move this to the XML file, Qt before 4.7 will choke on it */
    ui->addAsLabel->setPlaceholderText(tr("Enter a label for this address to add it to your address book"));
    ui->payTo->setPlaceholderText(tr("Enter a Whitecoin address (e.g. WPT7ufM1tz2uzV7x9sG26z4CuhWs2B18Rw)"));
#endif
    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(ui->payTo);

    GUIUtil::setupAddressWidget(ui->payTo, this);
}

SendCoinsEntry::~SendCoinsEntry()
{
    delete ui;
}

void SendCoinsEntry::on_pasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->payTo->setText(QApplication::clipboard()->text());
}

void SendCoinsEntry::on_addressBookButton_clicked()
{
    if(!model)
        return;
    AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::SendingTab, this);
    dlg.setModel(model->getAddressTableModel());
    if(dlg.exec())
    {
        ui->payTo->setText(dlg.getReturnValue());
        ui->payAmount->setFocus();
    }
}

void SendCoinsEntry::on_payTo_textChanged(const QString &address)
{
    if(!model)
        return;
    // Fill in label from address book, if address has an associated label
    QString associatedLabel = model->getAddressTableModel()->labelForAddress(address);
    if(!associatedLabel.isEmpty())
        ui->addAsLabel->setText(associatedLabel);
}

void SendCoinsEntry::setModel(WalletModel *model)
{
    this->model = model;

    if(model && model->getOptionsModel())
        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

    connect(ui->payAmount, SIGNAL(textChanged()), this, SIGNAL(payAmountChanged()));

    clear();
}

void SendCoinsEntry::setRemoveEnabled(bool enabled)
{
    ui->deleteButton->setEnabled(enabled);
}

void SendCoinsEntry::clear()
{
    ui->payTo->clear();
    ui->addAsLabel->clear();
    ui->payAmount->clear();
    ui->payInfo->clear();
    
    ui->payTo->setFocus();    
    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void SendCoinsEntry::on_deleteButton_clicked()
{
    emit removeEntry(this);
}

bool SendCoinsEntry::validate()
{
    // Check input validity
    bool retval = true;

    if(!ui->payAmount->validate())
    {
        retval = false;
    }
    else
    {
        if(ui->payAmount->value() <= 0)
        {
            // Cannot send 0 coins or less
            ui->payAmount->setValid(false);
            retval = false;
        }
    }

    if(!ui->payTo->hasAcceptableInput() ||
       (model && !model->validateAddress(ui->payTo->text())))
    {
        ui->payTo->setValid(false);
        retval = false;
    }
    
    if (ui->payInfo->text().length() > MAX_OP_RETURN_RELAY)
    {
        ui->payInfo->setValid(false);
        retval = false;
    }

    return retval;
}

SendCoinsRecipient SendCoinsEntry::getValue()
{
    SendCoinsRecipient rv;

    rv.address = ui->payTo->text();
    rv.label = ui->addAsLabel->text();
    rv.amount = ui->payAmount->value();
    rv.remark = ui->payInfo->text();

    return rv;
}

QWidget *SendCoinsEntry::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, ui->payTo);
    QWidget::setTabOrder(ui->payTo, ui->payInfo);
   	QWidget::setTabOrder(ui->payInfo, ui->addressBookButton);
    QWidget::setTabOrder(ui->addressBookButton, ui->pasteButton);
    QWidget::setTabOrder(ui->pasteButton, ui->deleteButton);
    QWidget::setTabOrder(ui->deleteButton, ui->addAsLabel);
    return ui->payAmount->setupTabChain(ui->addAsLabel);
}

void SendCoinsEntry::setValue(const SendCoinsRecipient &value)
{
    ui->payTo->setText(value.address);
    ui->addAsLabel->setText(value.label);
    ui->payAmount->setValue(value.amount);
}

void SendCoinsEntry::setAddress(const QString &address)
{
    //may have been scanned in so it must be parsed first 
    //QMessageBox::warning(this, tr("GR-Snap"), tr("The String Of Address is %1.").arg(address),  QMessageBox::Ok, QMessageBox::Ok);  
    if (address.size() > 34) {
        QString _address;
        QString _label;
        QString _amount;     
        int x = address.indexOf(":", 0, Qt::CaseInsensitive);
        if (x) {
            _address = address.mid(x+1, 34);
        }
        //QMessageBox::warning(this, tr("GR-Snap"), tr("The Address = %1.").arg(_address),  QMessageBox::Ok, QMessageBox::Ok);
        
        int y = address.indexOf("label=", 0, Qt::CaseInsensitive);
        if (y) {
		    		QString tmpURL= address.mid(y+6, address.size()-y-6);
		    		int z = tmpURL.indexOf("&", 0, Qt::CaseInsensitive);
		    		
		    		//QMessageBox::warning(this, tr("GR-Snap"), tr("The label = %1.").arg(tmpURL),  QMessageBox::Ok, QMessageBox::Ok);
		    		if (z) {
		        		_label = tmpURL.mid(0, z);
		      	} else {
		      			_label = tmpURL;
		      	}
		     }
		     int p = address.indexOf("amount=", 0, Qt::CaseInsensitive);
         if (p) {
         		QString tmpURL = address.mid(p+7, address.size()-p-7);
         		int z = tmpURL.indexOf("&", 0, Qt::CaseInsensitive);
         		
         		//QMessageBox::warning(this, tr("GR-Snap"), tr("The amount = %1.").arg(tmpURL),  QMessageBox::Ok, QMessageBox::Ok);
         		if (z) {
		        		_amount = tmpURL.mid(0, z);
		      	} else {
		      			_amount = tmpURL;
		      	}
		      	//QMessageBox::warning(this, tr("GR-Snap"), tr("The amount is %1.").arg(_amount),  QMessageBox::Ok, QMessageBox::Ok);
         }
         
        //Todo: parse out label and amount from incoming string
        ui->payTo->setText(_address);
        ui->addAsLabel->setText(_label);
        ui->payAmount->setValue(_amount.toDouble() * COIN);
    }
    else {
        ui->payTo->setText(address);
    }
    ui->payAmount->setFocus();
}

bool SendCoinsEntry::isClear()
{
    return ui->payTo->text().isEmpty();
}

void SendCoinsEntry::setFocus()
{
    ui->payTo->setFocus();
}

void SendCoinsEntry::updateDisplayUnit()
{
    if(model && model->getOptionsModel())
    {
        // Update payAmount with the current unit
        ui->payAmount->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
    }
}
