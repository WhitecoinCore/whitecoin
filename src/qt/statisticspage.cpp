#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "util.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "base58.h"
#include "clientmodel.h"
#include <sstream>
#include <string>
#include "json/json_spirit_utils.h"
#include "json/json_spirit_value.h"
#include "rpcserver.h"

using namespace json_spirit;

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    
    setFixedSize(400, 420);
    
    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(updateStatistics()));
}

int heightPrevious = -1;
int connectionPrevious = -1;
uint volumePrevious = -1;
double rewardPrevious = -1;
double netPawratePrevious = -1;
double pawratePrevious = -1;
double hardnessPrevious = -1;
double hardnessPrevious2 = -1;
int stakeminPrevious = -1;
int stakemaxPrevious = -1;
QString stakecPrevious = "";
uint64_t destroyedStakec =  65217844.265830 * COIN;  //https://www.richlist.eu/whitecoin/stats/destroyed

int getBlockHashrate(int);
void StatisticsPage::updateStatistics()
{
		double nMint = ((pindexBest->nMint) / COIN);
    double pHardness = GetDifficulty();
    double pHardness2 = GetDifficulty(GetLastBlockIndex(pindexBest, true));

    double pPawrate2 = 0.000;
    int nHeight = pindexBest->nHeight;
    int pPawrate =getBlockHashrate(nHeight);
    int lPawrate = 0;
    double lPawrate2 = 0.000;
    double nSubsidy = 0;
    
    uint64_t nMinWeight = 0;
    nMinWeight = pwalletMain->GetStakeWeight();
    nMinWeight =nMinWeight / COIN;
    
    uint64_t nNetworkWeight = GetPoSKernelPS();
    nNetworkWeight = nNetworkWeight / COIN;
    
    uint volume = ((pindexBest->nMoneySupply) / COIN);
    int peers = this->model->getNumConnections();
    lPawrate2 = 0; //((double)lPawrate / 1000);
    pPawrate2 = (((double)pPawrate / 1000)/1000);
    QString height = QString::number(nHeight);
    QString stakemax = QString::number(nNetworkWeight);
   	QString stakemin = QString::number(nMinWeight);
    QString phase = "";
    if (pindexBest->nHeight < 10000)
    {
        phase = "POW - POS";
    }
    else if (pindexBest->nHeight > 10000)
    {
        phase = "POS";
    }
    uint64_t aliveVolume = (volume * COIN - destroyedStakec) / COIN;

    QString subsidy = QString::number(nSubsidy, 'f', 6);
    QString hardness = QString::number(pHardness, 'f', 6);
    QString hardness2 = QString::number(pHardness2, 'f', 6);
    QString pawrate = QString::number(pPawrate2, 'f', 3);
    QString Qlpawrate = QString::number(lPawrate2, 'f', 3);
    QString QPeers = QString::number(peers);
    QString qVolume = QLocale(QLocale::English).toString(volume);

    if(nHeight > heightPrevious)
    {
        ui->heightBox->setText("<b><font color=\"green\">" + height + "</font></b>");
    } else {
    ui->heightBox->setText(height);
    }

    if(nNetworkWeight > stakemaxPrevious)
    {
        ui->maxBox->setText("<b><font color=\"green\">" + stakemax + "</font></b>");
    } else {
    ui->maxBox->setText(stakemax);
    }

    if(phase != stakecPrevious)
    {
        ui->cBox->setText("<b><font color=\"green\">" + phase + "</font></b>");
    } else {
    ui->cBox->setText(phase);
    }
    
    ui->minBox->setText(stakemin);

    if(pHardness2 > hardnessPrevious2)
    {
        ui->diffBox2->setText("<b><font color=\"green\">" + hardness2 + "</font></b>");
    } else if(pHardness2 < hardnessPrevious2) {
        ui->diffBox2->setText("<b><font color=\"red\">" + hardness2 + "</font></b>");
    } else {
        ui->diffBox2->setText(hardness2);
    }
    
    if(peers > connectionPrevious)
    {
        ui->connectionBox->setText("<b><font color=\"green\">" + QPeers + "</font></b>");             
    } else if(peers < connectionPrevious) {
        ui->connectionBox->setText("<b><font color=\"red\">" + QPeers + "</font></b>");        
    } else {
        ui->connectionBox->setText(QPeers);  
    }

    if(volume > volumePrevious)
    {
        ui->volumeBox->setText("<b><font color=\"green\">" + qVolume + " XWC" + "</font></b>");
    } else if(volume < volumePrevious) {
        ui->volumeBox->setText("<b><font color=\"red\">" + qVolume + " XWC" + "</font></b>");
    } else {
        ui->volumeBox->setText(qVolume + " XWC");
    }
    
    ui->rewardBox->setText(QString::number(nMint, 'f', 6) + " XWC");
    //ui->circBox->setText(QLocale(QLocale::English).toString(aliveVolume) + " XWC"); 
    
    updatePrevious(nHeight, nMinWeight, nNetworkWeight, phase, nSubsidy, pHardness, pHardness2, pPawrate2, lPawrate, peers, volume);
}

void StatisticsPage::updatePrevious(int nHeight, int nMinWeight, int nNetworkWeight, QString phase, double nSubsidy, double pHardness, double pHardness2, double pPawrate2, double lPawrate, int peers, int volume)
{
    heightPrevious = nHeight;
    stakeminPrevious = nMinWeight;
    stakemaxPrevious = nNetworkWeight;
    stakecPrevious = phase;
    rewardPrevious = nSubsidy;
    hardnessPrevious = pHardness;
    hardnessPrevious2 = pHardness2;
    netPawratePrevious = pPawrate2;
    pawratePrevious = lPawrate;
    connectionPrevious = peers;
    volumePrevious = volume;
}

void StatisticsPage::setModel(ClientModel *model)
{
    updateStatistics();
    this->model = model;
}


StatisticsPage::~StatisticsPage()
{
    delete ui;
}
