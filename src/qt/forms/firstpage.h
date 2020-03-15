#ifndef FIRSTPAGE_H
#define FIRSTPAGE_H

#include <QDialog>
#include "clientmodel.h"

namespace Ui {
class firstpage;
}

class firstpage : public QDialog
{
    Q_OBJECT

public:
    explicit firstpage(QWidget *parent = 0);
    ~firstpage();

    void setClientModel(ClientModel *model);

private slots:
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void updateAlerts(const QString &warnings);

private:
    Ui::firstpage *ui;
    ClientModel *clientModel;

};

#endif // FIRSTPAGE_H
