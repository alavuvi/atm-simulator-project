#include "withdraw.h"
#include "ui_withdraw.h"

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
{
    ui->setupUi(this);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
}

void Withdraw::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}
