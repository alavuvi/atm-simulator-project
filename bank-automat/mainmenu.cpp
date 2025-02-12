#include "mainmenu.h"
#include "environment.h"
#include "ui_mainmenu.h"
#include "balance.h"
#include "transactions.h"
#include "withdraw.h"

MainMenu::MainMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    labelName = ui->label;
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    if (newMyToken.isEmpty()) {
        qDebug() << "WARNING: Empty token received in setMyToken";
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
    qDebug() << "Token content:" << myToken;
    qDebug() << "Account ID:" << accountid;
    if (myToken.isEmpty()) {
        qDebug() << "Authentication token is empty";
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
        qDebug() << "Ei tokenia saatavilla Transactions";
        return;
    }
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
    Withdraw *objWithdraw = new Withdraw(this);
    objWithdraw->setMyToken(myToken);
    //tämä ottaa käyttöön, jos accountid:tä tarvitaan withdrawissa
    //objWithdraw->setAccountId(accountid);
    objWithdraw->open();
}

void MainMenu::on_btnLogout_clicked()
{
    myToken.clear();
    qDebug() << "Kirjaudutaan ulos ja tyhjennetään token";
    this->close();
}
