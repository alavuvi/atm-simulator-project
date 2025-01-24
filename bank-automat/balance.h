#ifndef BALANCE_H
#define BALANCE_H

#include <QDialog>

namespace Ui {
class Balance;
}

class Balance : public QDialog
{
    Q_OBJECT

public:
    explicit Balance(QWidget *parent = nullptr);
    ~Balance();

    void setCardnumber(const QString &newCardnumber);

    void setMyToken(const QByteArray &newMyToken);

private:
    Ui::Balance *ui;
    QString cardnumber;
    QByteArray myToken;
};

#endif // BALANCE_H
