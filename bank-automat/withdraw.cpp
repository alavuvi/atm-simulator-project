#include "withdraw.h"
#include "ui_withdraw.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include "environment.h"

Withdraw::Withdraw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Withdraw),
    networkManager(new QNetworkAccessManager(this)),
    accountsManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    QList<QPair<QPushButton*, int>> buttons = {
        {ui->pushButton, 20},
        {ui->pushButton_2, 40},
        {ui->pushButton_3, 60},
        {ui->pushButton_4, 80},
        {ui->pushButton_5, 100}
    };

    for (const auto &btn : buttons) {
        connect(btn.first, &QPushButton::clicked, this, [=]() {
            sendWithdrawRequest(btn.second);
        });
    }

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Withdraw::onCustomAmountEntered);
}

Withdraw::~Withdraw()
{
    delete ui;
    delete accountsManager;
}

void Withdraw::setMyToken(const QByteArray &token)
{
    myToken = token;
    qDebug() << "Withdraw received token:" << myToken;
}

void Withdraw::setIdCard(const QString &cardId)
{
    idCard = cardId;
    qDebug() << "Withdraw received idCard: " << idCard;

    // Retrieve account info:
    QString accounts_url = Environment::base_url() + "/accountsbycard/" + cardId;
    QNetworkRequest accountsRequest(accounts_url);
    accountsRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    accountsRequest.setRawHeader("Authorization", "Bearer " + myToken);

    connect(accountsManager, &QNetworkAccessManager::finished, this, &Withdraw::handleAccountInfoForWithdrawal);
    accountsManager->get(accountsRequest);
}

void Withdraw::handleAccountInfoForWithdrawal(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray accountsArray = json_doc.array();

        if (accountsArray.size() == 1) {
            QJsonObject accountObject = accountsArray[0].toObject();
            if (accountObject.contains("idaccount") && accountObject["idaccount"].isDouble()) {
                accountID = QString::number(accountObject["idaccount"].toDouble());
                qDebug() << "Account ID retrieved:" << accountID;
            } else {
                qDebug() << "Invalid JSON: 'idaccount' missing or not a number";
                QMessageBox::critical(this, "Error", "Invalid account information received.");
            }
        } else if (accountsArray.size() > 1) {
            qDebug() << "Multiple accounts found. Handling not implemented yet.";
            QMessageBox::critical(this, "Error", "Multiple accounts found. Withdrawal not supported.");
        } else {
            qDebug() << "No accounts found.";
            QMessageBox::critical(this, "Error", "No accounts found for this card.");
        }
    } else {
        qDebug() << "Error retrieving account info:" << reply->errorString();
        QMessageBox::critical(this, "Error", "Could not retrieve account information.");
    }
    reply->deleteLater();
}

void Withdraw::onCustomAmountEntered()
{
    int amount = ui->lineEdit->text().toInt();
    if (amount <= 0) {
        QMessageBox::warning(this, "Virhe", "Syötä kelvollinen nostosumma!");
        return;
    }
    sendWithdrawRequest(amount);
}

void Withdraw::sendWithdrawRequest(int amount)
{
    if (myToken.isEmpty() || idCard.isEmpty() || accountID.isEmpty()) {
        QMessageBox::critical(this, "Virhe", "Token, kortin ID tai tilin ID puuttuu.");
        return;
    }

    QUrl url("http://localhost:3000/withdraw");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QJsonObject json;
    json["idcard"] = idCard;
    json["amount"] = amount;
    json["idaccount"] = accountID; // Include accountID

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this]() {
        handleNetworkReply(qobject_cast<QNetworkReply*>(sender()));
    });
}

void Withdraw::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Withdraw success:" << response;
        QMessageBox::information(this, "Onnistui", "Nosto onnistui!");
    } else {
        qDebug() << "Withdraw request failed:" << reply->errorString();
        QMessageBox::critical(this, "Virhe", "Nosto epäonnistui: " + reply->errorString());
    }
    reply->deleteLater();
}
