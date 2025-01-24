#include "mainmenu.h"
#include "ui_mainmenu.h"

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
