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

    // Lisätty QTimer alustus
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &Balance::getBalanceData);
    refreshTimer->start(1000); // Päivitä 5 sekunnin välein
}

Balance::~Balance()
{
    refreshTimer->stop();  // Pysäytä ajastin
    delete ui;
}

void Balance::setCardnumber(const QString &newCardnumber)
{
    cardnumber = newCardnumber;
    getBalanceData();  // Hae balance heti kun korttinumero asetetaan
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
    QString site_url = "http://localhost:3000/account/5" + cardnumber;
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

        if (json_doc.isArray()) {
            QJsonArray json_array = json_doc.array();

            if (!json_array.isEmpty()) {
                QJsonObject json_obj = json_array.first().toObject();

                QJsonValue balanceValue = json_obj["balance"];
                double balance = 0.0;

                if (balanceValue.isString()) {
                    balance = balanceValue.toString().toDouble();
                } else {
                    balance = balanceValue.toDouble();
                }

                // Asetetaan teksti ilman virheitä
                ui->labelBalance->setText(QString("Current Balance: %1 €").arg(balance, 0, 'f', 2));
            } else {
                ui->labelBalance->setText("Error: No balance data");
            }
        } else {
            ui->labelBalance->setText("Error: Invalid JSON format");
        }
    }
    else
    {
        ui->labelBalance->setText("Error getting balance");
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}
