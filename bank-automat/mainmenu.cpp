#include "mainmenu.h"
#include "environment.h"
#include "ui_mainmenu.h"
#include "balance.h"
#include "transactions.h"
#include "withdraw.h"
#include "timermanager.h"

MainMenu::MainMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    labelName = ui->label;
    TimerManager::getInstance().setMainMenuWindow(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    if (newMyToken.isEmpty()) {
        qDebug() << "Varoitus: Tyhjä token tullut Main Menuun.";
        return;
    }
    myToken = newMyToken;
}

void MainMenu::setAccountId(const QString &newAccountId)
{
    accountid = newAccountId;
    getCustomerInfo();
}

void MainMenu::getCustomerInfo()
{
    qDebug() << "Account ID:" << accountid;
    if (myToken.isEmpty()) {
        return;
    }

    QString site_url = Environment::base_url() + QString("/customer/name/%1").arg(accountid);
    QNetworkRequest request((QUrl(site_url)));

    // Set headers
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray authHeader = "Bearer " + myToken;
    request.setRawHeader("Authorization", authHeader);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainMenu::handleCustomerInfo);
    reply = networkManager->get(request);
}

void MainMenu::handleCustomerInfo(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString customerName = jsonObj["name"].toString();
        if (!customerName.isEmpty()) {
            labelName->setText(customerName);
        } else {
            labelName->setText("Asiakasta ei löytynyt");
        }
    } else {
        labelName->setText("Virhe haettaessa asiakastietoja");
    }

    reply->deleteLater();
    networkManager->deleteLater();
}

void MainMenu::on_btnBalance_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Ei tokenia saatavilla balancelle";
        return;
    }
    TimerManager::getInstance().stopTimer();
    Balance *objBalance = new Balance(this);
    objBalance->setMyToken(myToken);
    objBalance->setAccountId(accountid);
    objBalance->open();

}

void MainMenu::on_btnTransactions_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Ei tokenia saatavilla Transactions";
        return;
    }
    TimerManager::getInstance().stopTimer();
    Transactions *objTransactions = new Transactions(this);
    objTransactions->setMyToken(myToken);
    qDebug() << "Token lähetty transactions:"<< myToken;
    objTransactions->setAccountId(accountid);
    objTransactions->open();
}

void MainMenu::on_btnWithdraw_clicked()
{
    if (myToken.isEmpty()) {
        qDebug() << "Ei tokenia saatavilla Withdraw";
        return;
    }
    TimerManager::getInstance().stopTimer();
    Withdraw *objWithdraw = new Withdraw(this);
    objWithdraw->setMyToken(myToken);
    objWithdraw->setIdCard(accountid);
    objWithdraw->open();
}

void MainMenu::on_btnLogout_clicked()
{
    disconnect(&TimerManager::getInstance(), &TimerManager::timerExpired,
               this, &MainMenu::handleTimerExpired);
    TimerManager::getInstance().stopTimer();
    myToken.clear();
    qDebug() << "Kirjaudutaan ulos ja tyhjennetään token";
    this->close();    
}

void MainMenu::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    if (this->isVisible()) {  // Tarkistetaan että ikkuna on oikeasti näkyvissä
        setupTimerConnections();
    }
}

void MainMenu::setupTimerConnections()
{
    // Poistetaan mahdolliset vanhat yhteydet
    disconnect(&TimerManager::getInstance(), &TimerManager::timerExpired,
               this, &MainMenu::handleTimerExpired);

    // Luodaan uusi yhteys
    if (this->isVisible()) {
        connect(&TimerManager::getInstance(), &TimerManager::timerExpired,
                this, &MainMenu::handleTimerExpired,
                Qt::UniqueConnection);

        TimerManager::getInstance().startTimer(this, TimerManager::WindowType::MAINMENU);
    }
}

void MainMenu::closeEvent(QCloseEvent* event)
{
    static bool closing = false;
    if (!closing) {
        closing = true;
        TimerManager::getInstance().stopTimer();
        qDebug() << "Ajastin pysäytetty MainMenussa";
    }
    QDialog::closeEvent(event);
    closing = false;
}

void MainMenu::handleTimerExpired()
{
    disconnect(&TimerManager::getInstance(), &TimerManager::timerExpired,
               this, &MainMenu::handleTimerExpired);
    myToken.clear();
    qDebug() << "Token tyhjennetty ajastimen loppuessa Main Menussa: " << myToken;
    accept();
}

