#include "balance.h"
#include "ui_balance.h"

Balance::Balance(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Balance)
{
    ui->setupUi(this);
}

Balance::~Balance()
{
    delete ui;
}

void Balance::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
}

void Balance::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}
