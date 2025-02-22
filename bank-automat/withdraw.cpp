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
    networkManager(new QNetworkAccessManager(this))
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
        connect(btn.first, &QPushButton::clicked, this, [this, btn]() {
            handleButtonClicked(btn.second);
        });
    }

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Withdraw::onCustomAmountEntered);
}

Withdraw::~Withdraw()
{
    delete ui;
    delete networkManager;
}

void Withdraw::setMyToken(const QByteArray &token)
{
    myToken = token;
    qDebug() << "Withdraw received token:" << myToken;
}

void Withdraw::setAccountId(const QString &accountId)
{
    accountID = accountId;
    qDebug() << "Withdraw received accountID: " << accountID;
}

void Withdraw::onCustomAmountEntered()
{
    int amount = ui->lineEdit->text().toInt();
    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "<FONT COLOR='#FFFFFF'>Enter approved amount !</FONT>");
        return;
    }
    handleButtonClicked(amount);
}

void Withdraw::handleButtonClicked(int amount)
{
    sendWithdrawRequest(amount);
}

void Withdraw::sendWithdrawRequest(int amount)
{
    if (myToken.isEmpty() || accountID.isEmpty()) {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#FFFFFF'>Token or accountid missing.</FONT>");
        return;
    }

    QUrl url(Environment::base_url()+"/withdraw");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer ") + myToken);

    QJsonObject json;
    json["idaccount"] = accountID;
    json["amount"] = amount;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    qDebug() << "Withdraw Request JSON:" << data;

    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleNetworkReply(reply);
    });
}

void Withdraw::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Withdraw successfull:" << response;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Success");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Enter approved amount!</FONT>");
        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
    } else {
        qDebug() << "Withdraw request failed:" << reply->errorString();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Withdraw failed:!</FONT>");
        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
    }
    reply->deleteLater();
}
