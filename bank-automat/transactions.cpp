#include "environment.h"
#include "transactions.h"
#include "ui_transactions.h"

Transactions::Transactions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Transactions)
{
    ui->setupUi(this);
}

Transactions::~Transactions()
{
    delete ui;
}

void Transactions::setAccountId(const QString &newAccountId)
{
    accountid = newAccountId;
    ui->labelAccountId->setText(accountid);
}

void Transactions::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}


void Transactions::on_btnTransactions_clicked()
{
    QString start = "1";
    QString end = "3";
    QString site_url=Environment::base_url()+"/transactions/"+accountid+"/"+start+"/"+end;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray header="Bearer "+myToken;
    request.setRawHeader(QByteArray("Authorization"),(header));

    transactionsManager = new QNetworkAccessManager(this);
    connect(transactionsManager, &QNetworkAccessManager::finished, this, &Transactions::showTransactionsSlot);
    reply = transactionsManager->get(request);
}

void Transactions::showTransactionsSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    QString transactions;
    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        transactions+=json_obj["datetime"].toString()+","+json_obj["transaction"].toString()+","+json_obj["amount"].toString()+"\r";
    }
    ui->textTransactions->setText(transactions);

    reply->deleteLater();
    transactionsManager->deleteLater();
}
