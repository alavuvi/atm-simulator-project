#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>

namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

    void setCardnumber(const QString &newCardnumber);

    void setMyToken(const QByteArray &newMyToken);

private:
    Ui::Withdraw *ui;
    QString cardnumber;
    QByteArray myToken;
};

#endif // WITHDRAW_H
