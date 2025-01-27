#include "mainmenu.h"
#include "balance.h"
#include "ui_mainmenu.h"
#include "transactions.h"
#include "withdraw.h"

MainMenu::MainMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
    ui->labelCardnumber->setText(cardnumber);
}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    qDebug()<<"Main Menu";
    qDebug()<<myToken;
}

void MainMenu::on_btnBalance_clicked()
{
    Balance *objBalance=new Balance(this);
    objBalance->setMyToken(myToken);
    objBalance->open();
}

void MainMenu::on_btnTransactions_clicked()
{
    Transactions *objTransactions=new Transactions(this);
    objTransactions->setMyToken(myToken);
    objTransactions->open();
}

void MainMenu::on_btnWithdraw_clicked()
{
    Withdraw *objWithdraw=new Withdraw(this);
    objWithdraw->setMyToken(myToken);
    objWithdraw->open();
}


void MainMenu::on_btnLogout_clicked()
{
    delete this;
    setMyToken(nullptr);
}

