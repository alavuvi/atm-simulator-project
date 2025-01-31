#include "selectaccount.h"
#include "ui_selectaccount.h"

SelectAccount::SelectAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectAccount)
{
    ui->setupUi(this);
}

SelectAccount::~SelectAccount()
{
    delete ui;
}

void SelectAccount::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    qDebug()<<"SelectAccount";
    qDebug()<<myToken;
}

void SelectAccount::SetAccountID(const QJsonArray &newAccountID)
{
    accountID = newAccountID;
    qDebug() << accountID;
}

void SelectAccount::on_btnDebit_clicked()
{

}


void SelectAccount::on_btnCredit_clicked()
{

}

