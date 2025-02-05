#include "selectaccount.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "ui_selectaccount.h"
#include "mainmenu.h"


SelectAccount::SelectAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectAccount)
    , networkManager(new QNetworkAccessManager(this))
    , creditAccountId(-1)
    , debitAccountId(-1)
{
    ui->setupUi(this);
}

SelectAccount::~SelectAccount()
{
    delete ui;
}

 // Esikäsittele tilit, jotta saadaan creditAccountId ja debitAccountId arrayhin
void SelectAccount::SetAccountID(const QJsonArray &newAccountID)
{
    accountID = newAccountID;
    qDebug() << "Account array:" << accountID;
    processAccounts();
}

// Käsitellään yksittäiset accountid:t creditlimit metodilla for-loopin sisällä
void SelectAccount::processAccounts()
{
    if(myToken.isEmpty()) {
        qDebug() << "Error: Ei tokenia saatavilla";
        return;
    }

    for (int i = 0; i < accountID.size(); i++) {
        QJsonObject account = accountID[i].toObject();
        int id = account["idaccount"].toInt();
        qDebug() << "Käsitellään account ID:" << id;

        QString baseUrl = Environment::base_url();
        QUrl url(baseUrl + "/creditlimit/" + QString::number(id));
        QNetworkRequest request(url);

        QByteArray authHeader = "Bearer " + myToken;
        qDebug() << authHeader;
        request.setRawHeader("Authorization", authHeader);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = networkManager->get(request);
        reply->setProperty("accountId", id);

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleCreditLimitResponse(reply);
        });
    }
}

// Asetetaan tilityypit vertaamalla creditlimit taulussa olevaan tietoon
void SelectAccount::handleCreditLimitResponse(QNetworkReply *reply)
{
    QByteArray response_data = reply->readAll();
    int accountId = reply->property("accountId").toInt();

    qDebug() << "Raw response:" << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();

    if (!json_array.isEmpty()) {
        QString creditLimitStr = json_array[0].toObject()["creditlimit"].toString();
        double creditLimit = creditLimitStr.toDouble();
        qDebug() << "Account:" << accountId << "Credit limit:" << creditLimit;

        if (creditLimit > 0.0) {
            creditAccountId = accountId;
            qDebug() << "Asetetaan credit tiliksi:" << creditAccountId;
            ui->btnCredit->setEnabled(true);
        } else {
            debitAccountId = accountId;
            qDebug() << "Asetetaan debit tiliksi:" << debitAccountId;
            ui->btnDebit->setEnabled(true);
        }
    }

    reply->deleteLater();
}

void SelectAccount::on_btnCredit_clicked()
{
    if (creditAccountId != -1) {
        MainMenu *objMainMenu = new MainMenu(this);
        objMainMenu->setMyToken(myToken);
       // qDebug() << "Token lähetetty Main Menu:" << myToken;
        objMainMenu->setAccountId(QString::number(creditAccountId));
        objMainMenu->open();
        this->close();
    }
    else {
        qDebug() << "Credit-tiliä ei löytynyt.";
    }
}

void SelectAccount::on_btnDebit_clicked()
{
    if (debitAccountId != -1) {
        MainMenu *objMainMenu = new MainMenu(this);
        objMainMenu->setMyToken(myToken);
       // qDebug() << "Token lähetetty Main Menu:" << myToken;
        objMainMenu->setAccountId(QString::number(debitAccountId));
        objMainMenu->open();
        this->close();
    }
    else {
        qDebug() << "Debit-tiliä ei löytynyt.";
    }
}

void SelectAccount::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    if(myToken.isEmpty()) {
        qDebug() << "Warning: Tyhjä token saatu";
    } else {
        qDebug() << "Token vastaanotettu Select Accountissa:" << myToken;
    }
}
