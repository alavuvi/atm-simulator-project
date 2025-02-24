#include "withdraw.h"
#include "ui_withdraw.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include "environment.h"
#include "timermanager.h"

Withdraw::Withdraw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Withdraw),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    //Ajastimen kutsuminen
    TimerManager::getInstance().startTimer(this, TimerManager::WindowType::OPERATIONS);
    connect(&TimerManager::getInstance(), &TimerManager::returnToMainMenuRequested,
            this, &Withdraw::close);


    connect(ui->Btnam100, &QPushButton::clicked, this, [this]() { handleButtonClicked(100); });
    connect(ui->Btnam50, &QPushButton::clicked, this, [this]() { handleButtonClicked(50); });
    connect(ui->Btnam20, &QPushButton::clicked, this, [this]() { handleButtonClicked(20); });


    QList<QPushButton*> numberButtons = {
        ui->Btnn0, ui->Btnn1, ui->Btnn2, ui->Btnn3, ui->Btnn4,
        ui->Btnn5, ui->Btnn6, ui->Btnn7, ui->Btnn8, ui->Btnn9
    };

    for (int i = 0; i < numberButtons.size(); ++i) {
        connect(numberButtons[i], &QPushButton::clicked, this, [this, i]() {
            ui->lineEdit->setText(ui->lineEdit->text() + QString::number(i));
            TimerManager::getInstance().resetTimer();  // Add this line
        });
    }


    connect(ui->Btnok, &QPushButton::clicked, this, &Withdraw::onCustomAmountEntered);
    connect(ui->Btncorr, &QPushButton::clicked, this, [this]() {
        QString text = ui->lineEdit->text();
        text.chop(1);
        ui->lineEdit->setText(text);
    });


    connect(ui->Btnback, &QPushButton::clicked, this, [this]() {
        TimerManager::getInstance().stopTimer(); // stop timer
        this->close();
        // set main menu as an active window and start timer there
        if (TimerManager::getInstance().getMainMenuWindow()) {
            TimerManager::getInstance().startTimer(
                TimerManager::getInstance().getMainMenuWindow(),
                TimerManager::WindowType::MAINMENU
                );
        }
    });
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
    TimerManager::getInstance().resetTimer();  // timer reset
    int amount = ui->lineEdit->text().toInt();
    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "<FONT COLOR='#FFFFFF'>Enter approved amount!</FONT>");
        return;
    }
    handleButtonClicked(amount);
}

void Withdraw::onNumberButtonClicked()
{
    //TimerManager::getInstance().resetTimer(); //timer reset
}

void Withdraw::onCorrectButtonClicked()
{
    //TimerManager::getInstance().resetTimer(); //timer reset
}

void Withdraw::onConfirmButtonClicked()
{
    //TimerManager::getInstance().resetTimer(); //timer reset
}

void Withdraw::onBackButtonClicked()
{

}

void Withdraw::handleButtonClicked(int amount)
{
    sendWithdrawRequest(amount);
    TimerManager::getInstance().resetTimer(); //timer reset
}
void Withdraw::fetchBalanceAndWithdraw(int amount)
{
    if (myToken.isEmpty() || accountID.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Token or accountID missing.</FONT>");
        msgBox.setIcon(QMessageBox::Critical);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
        return;
    }

    QUrl url(Environment::base_url() + "/balance/" + accountID);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QByteArray("Bearer ") + myToken);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, amount]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            QJsonObject jsonObject = jsonResponse.object();

            double balance = jsonObject["balance"].toDouble();
            double creditLimit = jsonObject["creditLimit"].toDouble();
            double availableFunds = balance + creditLimit;

            qDebug() << "Balance:" << balance << " Credit Limit:" << creditLimit;

            if (amount > availableFunds) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error");
                msgBox.setText("<FONT COLOR='#FFFFFF'>Insufficient funds!</FONT>");
                msgBox.setIcon(QMessageBox::Warning);

                msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

                QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
                okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
                msgBox.exec();
            } else {
                sendWithdrawRequest(amount);
            }
        } else {
            qDebug() << "Failed to fetch balance:" << reply->errorString();

            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("<FONT COLOR='#FFFFFF'>Failed to fetch balance.</FONT>");
            msgBox.setIcon(QMessageBox::Critical);

            msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

            QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
            okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
            msgBox.exec();
        }
        reply->deleteLater();
    });
}


void Withdraw::sendWithdrawRequest(int amount)
{
    if (myToken.isEmpty() || accountID.isEmpty()) {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#FFFFFF'>Token or accountID missing.</FONT>");
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
        qDebug() << "Withdraw successful:" << response;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Success");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Withdrawal successful!</FONT>");
        msgBox.setIcon(QMessageBox::Information);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
    } else {
        qDebug() << "Withdraw request failed:" << reply->errorString();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("<FONT COLOR='#FFFFFF'>Withdrawal failed!</FONT>");
        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setStyleSheet("QMessageBox { background-color: rgb(38,38,38);}");

        QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
        okButton->setStyleSheet("color: white; background-color: rgb(38,38,38);");
        msgBox.exec();
    }
    reply->deleteLater();
}
