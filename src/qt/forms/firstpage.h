#ifndef FIRSTPAGE_H
#define FIRSTPAGE_H

#include <QDialog>

namespace Ui {
class firstpage;
}

class firstpage : public QDialog
{
    Q_OBJECT

public:
    explicit firstpage(QWidget *parent = 0);
    ~firstpage();

private slots:
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();

private:
    Ui::firstpage *ui;
};

#endif // FIRSTPAGE_H
