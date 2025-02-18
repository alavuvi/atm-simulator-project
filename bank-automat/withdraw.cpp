#include "withdraw.h"
#include "ui_withdraw.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

Withdraw::Withdraw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Withdraw),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    qDebug() << "Withdraw received token:" << myToken;
}

void Withdraw::setIdCard(const QString &newIdCard)
{
    idCard = newIdCard;
    qDebug() << "Withdraw received idCard: " << idCard;
}

void Withdraw::sendWithdrawRequest(int amount)
{
    if (idCard.isEmpty()) {
        QMessageBox::critical(this, "Error", "ID Card is not set!");
        return;
    }

    QNetworkRequest request(QUrl("http://localhost:3000/withdraw"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QJsonObject json;
    json["idcard"] = idCard.toInt();
    json["amount"] = amount;

    qDebug() << "Sending withdraw request: " << QJsonDocument(json).toJson();

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &Withdraw::handleWithdrawReply);
}

void Withdraw::handleWithdrawReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", "Withdrawal successful!");
        qDebug() << "Withdraw successful!";
    } else {
        QByteArray errorData = reply->readAll();
        QMessageBox::critical(this, "Error", "Withdraw request failed!");
        qDebug() << "Withdraw request failed: " << reply->errorString() << " Error Data: " << errorData;
    }

    reply->deleteLater();
}

void Withdraw::on_btnWithdraw_clicked()
{
    bool ok;
    int amount = ui->lineEdit->text().toInt(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Enter a valid amount.");
        return;
    }

    sendWithdrawRequest(amount);
}
