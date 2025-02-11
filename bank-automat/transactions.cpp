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


void Transactions::on_btnTransactions_clicked()
{
    //Limit: shows the wanted amount of transactions
    int s = 0;
    int e = 10;
    QString start = QString::number(s);
    QString end = QString::number(e);
    //login
    QString site_url=Environment::base_url()+"/transactions/"+accountid+"/"+start+"/"+end;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray header="Bearer "+myToken;
    request.setRawHeader(QByteArray("Authorization"),(header));
    transactionsManager = new QNetworkAccessManager(this);
    connect(transactionsManager, &QNetworkAccessManager::finished, this, &Transactions::showTransactionsSlot);
    reply = transactionsManager->get(request);

    /*
    QStandardItemModel *table_model = new QStandardItemModel(transactionsList.size(),3);
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Transaction"));
    table_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Amount"));

    for (int row = 0; row < studentList.size(); ++row) {
        QStandardItem *date = new QStandardItem(transactionsList[row].getDate());
        table_model->setItem(row, 0, date);
        QStandardItem *transaction = new QStandardItem(transactionsList[row].getTrasnaction());
        table_model->setItem(row, 1, transaction);
        QStandardItem *amount = new QStandardItem(transactionsList[row].getAmount());
        table_model->setItem(row, 2, amount);
    }

    ui->tableStudents->setModel(table_model);
    */
}

/*
VANHA SLOT
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
*/

void Transactions::on_tableTransactions_clicked(const QModelIndex &index)
{
    /*
    UUS SLOT
    QVariant value=index.sibling(index.row(),index.column()).data();
    QString selected_value=QVariant(value).toString();
    */
}

