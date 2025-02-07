#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "balance.h"
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

void MainMenu::setAccountId(const QString &newAccountId)
{
    accountid = newAccountId;
    ui->labelAccountid->setText(accountid);
}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    if (newMyToken.isEmpty()) {
        qDebug() << "Warning: Tyhjä token saatu";
        return;
    }
    myToken = newMyToken;
    qDebug() << "Token asetettu Main Menu:" << myToken;
}

void MainMenu::on_btnBalance_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Error: Ei tokenia saatavilla balancelle";
        return;
    }
    Balance *objBalance = new Balance(this);
    objBalance->setMyToken(myToken);
     /* tämä ottaa käyttöön, jos accountid:tä tarvitaan balancessa
    objBalance->setAccountId(accountid);
    */
    objBalance->open();
}

void MainMenu::on_btnTransactions_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Error: Ei tokenia saatavilla Transactions";
        return;
    }
    Transactions *objTransactions = new Transactions(this);
    objTransactions->setMyToken(myToken);
    // tämä ottaa käyttöön, jos accountid:tä tarvitaan transactionsissa
    objTransactions->setAccountId(accountid);
    objTransactions->open();
}

void MainMenu::on_btnWithdraw_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Error: Ei tokenia saatavilla Withdraw";
        return;
    }
    Withdraw *objWithdraw = new Withdraw(this);
    objWithdraw->setMyToken(myToken);
     /* tämä ottaa käyttöön, jos accountid:tä tarvitaan withdrawissa
    // objWithdraw->setAccountId(accountid);
    */
    objWithdraw->open();
}

void MainMenu::on_btnLogout_clicked()
{
    myToken.clear();
    qDebug() << "Kirjaudutaan ulos ja tyhjennetään token";
    this->close();
}
