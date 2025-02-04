#include "balance.h"
#include "ui_balance.h"
#include <QJsonObject>
#include <QDebug>

Balance::Balance(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Balance)
{
    ui->setupUi(this);
    this->setWindowTitle("Balance");
    networkManager = new QNetworkAccessManager(this);
}

Balance::~Balance()
{
    delete ui;
}

void Balance::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
    getBalanceData();
}

void Balance::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

void Balance::on_btnBack_clicked()
{
    this->close();
}

void Balance::getBalanceData()
{
    QString site_url = "http://localhost:3000/balance/" + cardnumber;
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), "Bearer " + myToken);
    reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Balance::balanceReceived);
}

void Balance::balanceReceived()
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString response = reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject json_obj = json_doc.object();
        double balance = json_obj["balance"].toDouble();

        ui->labelBalance->setText(QString("Current Balance: %.2f €").arg(balance));
    }
    else
    {
        ui->labelBalance->setText("Error getting balance");
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}
