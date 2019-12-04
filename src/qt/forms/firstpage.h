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

private:
    Ui::firstpage *ui;
};

#endif // FIRSTPAGE_H
