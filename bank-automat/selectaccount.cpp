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

void SelectAccount::SetAccountID(const QJsonArray &newAccountID)
{
    accountID = newAccountID;
    qDebug() << "Account array:" << accountID;
    // Esikäsittele tilit, jotta saadaan creditAccountId ja debitAccountId
    processAccounts();
}

void SelectAccount::processAccounts()
{
    if(myToken.isEmpty()) {
        qDebug() << "Error: No token available";
        return;
    }

    for (int i = 0; i < accountID.size(); i++) {
        QJsonObject account = accountID[i].toObject();
        int id = account["idaccount"].toInt();
        qDebug() << "Processing account ID:" << id;

        QString baseUrl = Environment::base_url();
        QUrl url(baseUrl + "/creditlimit/" + QString::number(id));
        QNetworkRequest request(url);

        QByteArray headerValue = "Bearer " + myToken;
        request.setRawHeader("Authorization", headerValue);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = networkManager->get(request);
        reply->setProperty("accountId", id);

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleCreditLimitResponse(reply);
        });
    }
}

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
            qDebug() << "Set as credit account:" << creditAccountId;
            ui->btnCredit->setEnabled(true);
        } else {
            debitAccountId = accountId;
            qDebug() << "Set as debit account:" << debitAccountId;
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
       // objMainMenu->setAccountid(creditAccountId);
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
        // Voit siirtää debit tilin id:n MainMenu:lle esim. setAccountID(debitAccountId)
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
        qDebug() << "Warning: Empty token received";
    } else {
        qDebug() << "Token received in SelectAccount:" << myToken;
    }
}
