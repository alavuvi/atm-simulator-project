#include "transactions.h"
#include "ui_transactions.h"

Transactions::Transactions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Transactions)
{
    ui->setupUi(this);
}

Transactions::~Transactions()
{
    delete ui;
}

void Transactions::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
}

void Transactions::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}
