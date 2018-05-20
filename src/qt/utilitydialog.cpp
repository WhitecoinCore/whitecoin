// Copyright (c) 2011-2015 The Bitcoin developers
// Copyright (c) 2015-2018 The Whitecoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "utilitydialog.h"
#include "ui_paperwalletdialog.h"
#include "ui_impprivkeydialog.h"

#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "bitcoingui.h"
#include "clientmodel.h"
#include "guiutil.h"
#include "addresstablemodel.h"

#include "clientversion.h"
#include "init.h"
#include "base58.h"

#ifdef ENABLE_WALLET
#include "sendcoinsdialog.h"
#include "sendcoinsentry.h"
#include "coincontrol.h"
#include "coincontroldialog.h"
#include "walletmodel.h"
#endif

#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QInputDialog>

#ifdef USE_QRCODE
#include <qrencode.h>
#endif


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <qnumeric.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#else
// Use QT5's new modular classes
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QPainter>
#endif



/** "PaperWallet" dialog box */
PaperWalletDialog::PaperWalletDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaperWalletDialog)
{
    ui->setupUi(this);

    ui->buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);

    // Begin with a small bold monospace font for the textual version of the key and address.
    QFont font("Monospace");
    font.setBold(true);
    font.setStyleHint(QFont::TypeWriter);
    font.setPixelSize(1);
    ui->addressText->setFont(font);
    ui->privateKeyText->setFont(font);
    ui->addressText->setAlignment(Qt::AlignJustify);
    ui->addressText->setStyleSheet("background:transparent");
    ui->privateKeyText->setAlignment(Qt::AlignJustify);
    ui->privateKeyText->setStyleSheet("background:transparent");

    if (vNodes.size() > 0) {
			QMessageBox::critical(this, tr("Warning: Network Activity Detected"), tr("It is recommended to disconnect from the internet before printing paper wallets. Even though paper wallets are generated on your local computer, it is still possible to unknowingly have malware that transmits your screen to a remote location. It is also recommended to print to a local printer vs a network printer since that network traffic can be monitored. Some advanced printers also store copies of each printed document. Proceed with caution relative to the amount of value you plan to store on each address."), QMessageBox::Ok, QMessageBox::Ok);
    }

}

void PaperWalletDialog::setModel(WalletModel *model)
{
    RandAddSeed();
    this->model = model;
    this->on_getNewAddress_clicked("","","");
}

PaperWalletDialog::~PaperWalletDialog()
{
    delete ui;
}

void PaperWalletDialog::on_getNewAddress_clicked(std::string strAddress , std::string strPubKey , std::string strPrivKey)
{
    // Create a new private key   
    CKey secret;
    secret.MakeNewKey(true);
    
    // Derive the public key
    CPubKey pubkey = secret.GetPubKey();

    // Derive the public key hash
    CBitcoinAddress pubkeyhash;
    pubkeyhash.Set(secret.GetPubKey().GetID());

    // Create String versions of each
    string myPrivKey = strPrivKey;
    string myPubKey = strPubKey;
    string myAddress = strAddress;
    if (strAddress=="") {   
	    myPrivKey = CBitcoinSecret(secret).ToString();
	    myPubKey = HexStr(secret.GetPubKey()).c_str();
	    myAddress = pubkeyhash.ToString();
		}

#ifdef USE_QRCODE
    // Generate the address QR code
    QRcode *code = QRcode_encodeString(myAddress.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
    if (!code)
    {
        ui->addressQRCode->setText(tr("Error encoding Address into QR Code."));
        return;
    }
    QImage publicKeyImage = QImage(code->width, code->width, QImage::Format_ARGB32);
    publicKeyImage.fill(0x000000);
    unsigned char *p = code->data;
    for (int y = 0; y < code->width; y++)
    {
        for (int x = 0; x < code->width; x++)
        {
            publicKeyImage.setPixel(x, y, ((*p & 1) ? 0xff000000 : 0x0));
            p++;
        }
    }
    QRcode_free(code);


    // Generate the private key QR code
    code = QRcode_encodeString(myPrivKey.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
    if (!code)
    {
        ui->privateKeyQRCode->setText(tr("Error encoding private key into QR Code."));
        return;
    }
    QImage privateKeyImage = QImage(code->width, code->width, QImage::Format_ARGB32);
    privateKeyImage.fill(0x000000);
    p = code->data;
    for (int y = 0; y < code->width; y++)
    {
        for (int x = 0; x < code->width; x++)
        {
            privateKeyImage.setPixel(x, y, ((*p & 1) ? 0xff000000 : 0x0));
            p++;
        }
    }
    QRcode_free(code);

    // Populate the QR Codes
    ui->addressQRCode->setPixmap(QPixmap::fromImage(publicKeyImage).scaled(ui->addressQRCode->width(), ui->addressQRCode->height()));
    ui->privateKeyQRCode->setPixmap(QPixmap::fromImage(privateKeyImage).scaled(ui->privateKeyQRCode->width(), ui->privateKeyQRCode->height()));
#endif

    // Populate the Texts
    ui->addressText->setText(myAddress.c_str());
    ui->privateKeyText->setText(tr(myPrivKey.c_str()));
    ui->publicKey->setHtml(myPubKey.c_str());
    
    // Update the fonts to fit the height of the wallet.
    // This should only really trigger the first time since the font size persists.
    double paperHeight = (double) ui->paperTemplate->height();
    double maxTextWidth = paperHeight * 0.98;   
    double minTextWidth = paperHeight * 0.94;
    int pixelSizeStep = 1;

    int addressTextLength = ui->addressText->fontMetrics().boundingRect(ui->addressText->text()).width();
    QFont font = ui->addressText->font();
    for(int i = 0; i < PAPER_WALLET_READJUST_LIMIT; i++) {
        if ( addressTextLength < minTextWidth) {
            font.setPixelSize(font.pixelSize() + pixelSizeStep);
            ui->addressText->setFont(font);
            addressTextLength = ui->addressText->fontMetrics().boundingRect(ui->addressText->text()).width();
        } else {
            break;
        }

    }
    if ( addressTextLength > maxTextWidth ) {
        font.setPixelSize(font.pixelSize() - pixelSizeStep);
        ui->addressText->setFont(font);
        addressTextLength = ui->addressText->fontMetrics().boundingRect(ui->addressText->text()).width();
    }

    int privateKeyTextLength = ui->privateKeyText->fontMetrics().boundingRect(ui->privateKeyText->text()).width();
    font = ui->privateKeyText->font();
    for(int i = 0; i < PAPER_WALLET_READJUST_LIMIT; i++) {
        if ( privateKeyTextLength < minTextWidth) {
            font.setPixelSize(font.pixelSize() + pixelSizeStep);
            ui->privateKeyText->setFont(font);
            privateKeyTextLength = ui->privateKeyText->fontMetrics().boundingRect(ui->privateKeyText->text()).width();
        } else {
            break;
        }
    }
    if ( privateKeyTextLength > maxTextWidth ) {
        font.setPixelSize(font.pixelSize() - pixelSizeStep);
        ui->privateKeyText->setFont(font);
        privateKeyTextLength = ui->privateKeyText->fontMetrics().boundingRect(ui->privateKeyText->text()).width();
    }
    
    ui->addressText->setAttribute(Qt::WA_TranslucentBackground, false);
    ui->privateKeyText->setAttribute(Qt::WA_TranslucentBackground, false);
}

void PaperWalletDialog::on_printButton_clicked()
{

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *qpd = new QPrintDialog(&printer, this);

    qpd->setPrintRange(QAbstractPrintDialog::AllPages);

    QList<QString> recipientPubKeyHashes;

    if ( qpd->exec() != QDialog::Accepted ) {
        return;
    }

    // Hardcode these values
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(QPrinter::A4);
    printer.setFullPage(true);

    QPainter painter;
    if (! painter.begin(&printer)) { // failed to open file
        QMessageBox::critical(this, "Printing Error", tr("failed to open file, is it writable?"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    int walletCount = ui->walletCount->currentIndex() + 1;
    int walletsPerPage = 4;

    int pageHeight = printer.pageRect().height() - PAPER_WALLET_PAGE_MARGIN;
    int walletHeight = ui->paperTemplate->height();
    double computedWalletHeight = 0.9 * pageHeight / walletsPerPage;
    double scale = computedWalletHeight / walletHeight;
    double walletPadding = pageHeight * 0.05 / (walletsPerPage - 1) / scale;

    QRegion walletRegion = QRegion(ui->paperTemplate->x(), ui->paperTemplate->y(),
    ui->paperTemplate->width(), ui->paperTemplate->height());
        painter.scale(scale, scale);

    for(int i = 0; i < walletCount; i++) {

        QPoint point = QPoint(PAPER_WALLET_PAGE_MARGIN, (PAPER_WALLET_PAGE_MARGIN / 2) + ( i % walletsPerPage ) * (walletHeight + walletPadding));
        this->render(&painter, point, walletRegion);
        recipientPubKeyHashes.append(ui->addressText->text());

        if ( i % walletsPerPage == ( walletsPerPage - 1 ) ) {
            printer.newPage();
        }

        this->on_getNewAddress_clicked("","","");
    }

    painter.end();
    return;
}

void PaperWalletDialog::setPrintData(QString strAddress, QString strPubKey, QString strKey)
{
    string myPrivKey = strKey.toStdString();
    string myPubKey = strPubKey.toStdString();
    string myAddress = strAddress.toStdString();
    
    on_getNewAddress_clicked(myAddress, myPubKey, myPrivKey);
    return;
}







/** "Import Private Key" dialog box */

ImpPrivKeyDialog::ImpPrivKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpPrivKeyDialog)
{
    ui->setupUi(this);
}

void ImpPrivKeyDialog::setModel(WalletModel *model)
{
    this->model = model;    
}

ImpPrivKeyDialog::~ImpPrivKeyDialog()
{
    delete ui;
}

void ImpPrivKeyDialog::on_exitButton_clicked()
{
    close();
}

void ImpPrivKeyDialog::on_importButton_clicked()
{
    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(ui->addrEdit->text().toStdString());
    
    if (!fGood)
		{
				QMessageBox::warning(this, windowTitle(), tr("Invalid private key"), QMessageBox::Ok, QMessageBox::Ok);
        return;
		}
    if (fWalletUnlockStakingOnly)
    {
				QMessageBox::warning(this, windowTitle(), tr("Wallet is unlocked for staking only"), QMessageBox::Ok, QMessageBox::Ok);
        return;
		}
    
    string strLabel = "";
    bool fRescan = true;
    
    CKey key = vchSecret.GetKey();
    CPubKey pubkey = key.GetPubKey();
    CKeyID vchAddress = pubkey.GetID();
    {
				LOCK2(cs_main, pwalletMain->cs_wallet);
				
				pwalletMain->MarkDirty();
        pwalletMain->SetAddressBookName(vchAddress, strLabel);

        // Don't throw error in case a key is already there
        if (pwalletMain->HaveKey(vchAddress))
		    {
						QMessageBox::warning(this, windowTitle(), tr("This key has already existed"), QMessageBox::Ok, QMessageBox::Ok);
		        return;
				}
            	
        pwalletMain->mapKeyMetadata[vchAddress].nCreateTime = 1;

        if (!pwalletMain->AddKeyPubKey(key, pubkey))
        {
						QMessageBox::warning(this, windowTitle(), tr("Error adding key to wallet"), QMessageBox::Ok, QMessageBox::Ok);
		        return;
				}

        // whenever a key is imported, we need to scan the whole chain
        pwalletMain->nTimeFirstKey = 1; // 0 would be considered 'no value'

        if (fRescan) {
            pwalletMain->ScanForWalletTransactions(pindexGenesisBlock, true);
            pwalletMain->ReacceptWalletTransactions();
            
            QMessageBox::warning(this, windowTitle(), tr("The import has completed"), QMessageBox::Ok, QMessageBox::Ok);
           	return;
        } //导入完成后，需重启钱包
    }
}
