#include "environment.h"
#include "transactions.h"
#include "ui_transactions.h"

#include <QStandardItemModel>

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

void Transactions::showTransactionsSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();
    QStandardItemModel *model = new QStandardItemModel(json_array.size(), 3, this);
    model->setHeaderData(0, Qt::Horizontal, "Datetime");
    model->setHeaderData(1, Qt::Horizontal, "Transaction");
    model->setHeaderData(2, Qt::Horizontal, "Amount");

    for (int row = 0; row < json_array.size(); ++row) {
        QJsonObject json_obj = json_array[row].toObject();

        QString datetime = json_obj["datetime"].toString();
        QString transaction = json_obj["transaction"].toString();
        QString amount = json_obj["amount"].toString();

        model->setItem(row, 0, new QStandardItem(datetime));
        model->setItem(row, 1, new QStandardItem(transaction));
        model->setItem(row, 2, new QStandardItem(amount));
    }

    ui->tableTransactions->setModel(model);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    reply->deleteLater();
    transactionsManager->deleteLater();

}
void Transactions::on_btnTransactions_clicked()
{
    int s = 0;
    int e = 10;
    QString start = QString::number(s);
    QString end = QString::number(e);

    QString site_url=Environment::base_url()+"/transactions/"+accountid+"/"+start+"/"+end;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray header= "Bearer "+myToken;
    request.setRawHeader(QByteArray("Authorization"),(header));
    transactionsManager = new QNetworkAccessManager(this);
    connect(transactionsManager, &QNetworkAccessManager::finished, this, &Transactions::showTransactionsSlot);
    reply = transactionsManager->get(request);
}

void Transactions::on_btn_older_clicked()
{
    static int s = 0;
    static int e = 10;
    s += 10;
    e += 10;
    QString start = QString::number(s);
    QString end = QString::number(e);

    QString site_url=Environment::base_url()+"/transactions/"+accountid+"/"+start+"/"+end;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray header = "Bearer " + myToken;
    request.setRawHeader(QByteArray("Authorization"), header);
    transactionsManager = new QNetworkAccessManager(this);
    connect(transactionsManager, &QNetworkAccessManager::finished, this, &Transactions::showTransactionsSlot);
    reply = transactionsManager->get(request);
}

void Transactions::on_btnBack_clicked()
{

}
