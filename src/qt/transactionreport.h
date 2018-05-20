// Copyright (c) 2013-2015 The Feathercoin developers
// Copyright (c) 2014-2017 The Whitecoin developers

#ifndef TRANSACTIONREPORT_H
#define TRANSACTIONREPORT_H

#include "guiutil.h"

#include <QWidget>

class WalletModel;
class TransactionFilterProxy;

QT_BEGIN_NAMESPACE
class QTableView;
class QStandardItemModel;
class QComboBox;
class QLineEdit;
class QModelIndex;
class QMenu;
class QFrame;
class QDateTimeEdit;
class QLabel;
QT_END_NAMESPACE

/** Widget showing the transaction list for a wallet, including a filter row.
    Using the filter row, the user can view or export a subset of the transactions.
  */
class TransactionReport : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionReport(QWidget *parent = 0);

    void setModel(WalletModel *model);

    // Date ranges for filter
    enum DateEnum
    {
        All,
        Today,
        Yesterday,
        ThisWeek,
        LastWeek,
        ThisMonth,
        LastMonth,
        ThisYear,
        Range
    };

private:
    WalletModel *model;
    TransactionFilterProxy *transactionProxyModel;
    QTableView *transactionReport;
    QTableView *transactionReportT;
    QTableView *transactionRreportT2;
    QStandardItemModel *reportModel;
    QStandardItemModel *reportModelT;
    QStandardItemModel *reportModelT2;

    QComboBox *dateWidget;
    QComboBox *typeWidget;
    QLineEdit *addressWidget;
    QLineEdit *amountWidget;
    QLabel *totalnumslWidget;
    QLabel *totaltimesWidget;
    QLabel *rptTotal;  
    QMenu *contextMenu;

    QFrame *dateRangeWidget;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;

    QWidget *createDateRangeWidget();


public Q_SLOTS:	
    void chooseDate(int idx);
    void chooseType(int idx);
    void changedPrefix(const QString &prefix);
    void changedAmount(const QString &amount);
    void exportClicked();
    void focusTransaction(const QModelIndex&);


private Q_SLOTS:
    void dateRangeChanged();
    void showTotal();

Q_SIGNALS:
    /**  Fired when a message should be reported to the user */
    void message(const QString &title, const QString &message, unsigned int style);

};

#endif // TRANSACTIONREPORT_H
