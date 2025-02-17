#include "withdraw.h"
#include "ui_withdraw.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Withdraw::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Withdraw::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Withdraw::on_pushButton_3_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Withdraw::on_pushButton_4_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Withdraw::on_pushButton_5_clicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Withdraw::onCustomWithdraw);

    connect(networkManager, &QNetworkAccessManager::finished, this, &Withdraw::onWithdrawReply);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
}

void Withdraw::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

void Withdraw::sendWithdrawRequest(int amount)
{
    QNetworkRequest request(QUrl("http://localhost:3000/withdraw")); // Muuta osoite tarvittaessa
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QJsonObject json;
    json["amount"] = amount;
    json["cardnumber"] = cardnumber;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());
    qDebug() << "Sending withdraw request: " << json;
}

void Withdraw::on_pushButton_clicked() { sendWithdrawRequest(20); }
void Withdraw::on_pushButton_2_clicked() { sendWithdrawRequest(40); }
void Withdraw::on_pushButton_3_clicked() { sendWithdrawRequest(60); }
void Withdraw::on_pushButton_4_clicked() { sendWithdrawRequest(80); }
void Withdraw::on_pushButton_5_clicked() { sendWithdrawRequest(100); }

// Mukautettu summa
void Withdraw::onCustomWithdraw()
{
    bool ok;
    int amount = ui->lineEdit->text().toInt(&ok);
    if (ok && amount > 0) {
        sendWithdrawRequest(amount);
    } else {
        qDebug() << "Incorrect amount!";
    }
}

//palvelimen vastaus
void Withdraw::onWithdrawReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "Withdraw succesfully completed: " << responseData;
    } else {
        qDebug() << "Withdraw request failed: " << reply->errorString();
    }
    reply->deleteLater();
}
