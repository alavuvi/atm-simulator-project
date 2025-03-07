#include "environment.h"
#include "transactions.h"
#include "ui_transactions.h"
#include <QStandardItemModel>
#include "timermanager.h"

Transactions::Transactions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Transactions)
{
    ui->setupUi(this);
    labelName = ui->label1;
    //Ajastimen kutsuminen
    TimerManager::getInstance().startTimer(this, TimerManager::WindowType::OPERATIONS);
    connect(&TimerManager::getInstance(), &TimerManager::returnToMainMenuRequested,
            this, &Transactions::close);
}

Transactions::~Transactions()
{
    delete ui;
}

void Transactions::setAccountId(const QString &newAccountId)
{
    accountid = newAccountId;
    getCustomerInfo();
}

void Transactions::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

void Transactions::getCustomerInfo()
{
    qDebug() << "Account ID:" << accountid;
    if (myToken.isEmpty()) {
    //    qDebug() << "Token tyhjä";
        return;
    }

    QString site_url = Environment::base_url() + QString("/customer/name/%1").arg(accountid);
    QNetworkRequest request((QUrl(site_url)));

    // Set headers
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray authHeader = "Bearer " + myToken;
    request.setRawHeader("Authorization", authHeader);

    transactionsManager = new QNetworkAccessManager(this);
    connect(transactionsManager, &QNetworkAccessManager::finished,
            this, &Transactions::handleCustomerInfo);
    reply = transactionsManager->get(request);
}

void Transactions::handleCustomerInfo(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString customerName = jsonObj["name"].toString();
        if (!customerName.isEmpty()) {
            labelName->setText(customerName);
        } else {
            labelName->setText("<FONT COLOR='#FFFFFF'>Asiakasta ei löytynyt</FONT>");
        }
    } else {
        labelName->setText("<FONT COLOR='#FFFFFF'>Virhe haettaessa asiakastietoja</FONT>");
    }

    reply->deleteLater();
    transactionsManager->deleteLater();
}

void Transactions::showTransactionsSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    qDebug()<<response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();

    if (!json_array.isEmpty()) {
        totalTransactions = qMax(totalTransactions, s + json_array.size());
    }

    if (json_array.isEmpty()) {
        qDebug() << "No more transactions to show.";
        return;
    }

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
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    reply->deleteLater();
    transactionsManager->deleteLater();

}

void Transactions::loadTransactions()
{
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

void Transactions::on_btnTransactions_clicked()
{
    s = 0;
    e = 10;
    loadTransactions();
    TimerManager::getInstance().resetTimer(); //timer reset
}

void Transactions::on_btn_older_clicked()
{
    if (s + 10 > totalTransactions) {
        qDebug() << "No older transactions available.";
        return;
    }

    s += 10;
    e += 0;

    loadTransactions();
    TimerManager::getInstance().resetTimer(); //timer reset
}

void Transactions::on_btn_newer_clicked()
{
    if (s - 10 < 0) {
        qDebug() << "No newer transactions available.";
        return;
    }

    s -= 10;
    e -= 0;

    loadTransactions();
    TimerManager::getInstance().resetTimer(); //timer reset
}


void Transactions::on_btnBack_clicked()
{
    TimerManager::getInstance().stopTimer(); // Pysäytetään nykyinen ajastin
    this->close();
    // Asetetaan mainmenu aktiiviseksi ikkunaksi ja aloitetaan ajastin
    if (TimerManager::getInstance().getMainMenuWindow()) {
        TimerManager::getInstance().startTimer(
            TimerManager::getInstance().getMainMenuWindow(),
            TimerManager::WindowType::MAINMENU
            );
    }
}

void Transactions::handleReturnToMainMenu()
{
    this->close();
}
