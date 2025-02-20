#include "balance.h"
#include "ui_balance.h"
#include <QJsonObject>
#include <QDebug>
#include <environment.h>
#include "timermanager.h"

Balance::Balance(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Balance)
{
    ui->setupUi(this);
    this->setWindowTitle("Balance");
    networkManager = new QNetworkAccessManager(this);
    customerManager = new QNetworkAccessManager(this);
    creditLimitManager = new QNetworkAccessManager(this);
    transactionsManager = new QNetworkAccessManager(this);

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &Balance::getBalanceData);
    //refreshTimer->start(5000); // 5 sekunnin välein

    inactivityTimer = new QTimer(this);
    inactivityTimer->setSingleShot(true);
    connect(inactivityTimer, &QTimer::timeout, this, &Balance::close);
    resetInactivityTimer();

    this->installEventFilter(this);
}

Balance::~Balance()
{
    refreshTimer->stop();
    inactivityTimer->stop();

    delete ui;
}

void Balance::setAccountId(const QString &newAccountId)
{
    accountid = newAccountId;
    getBalanceData();
    getCustomerInfo();
    getCreditLimitData();
    getRecentTransactions();
}

void Balance::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

// Ikkunan sulkeutuessa käynnistää main menu-valikon ajastimen
void Balance::closeEvent(QCloseEvent* event)
{
    refreshTimer->stop();
    inactivityTimer->stop();

    TimerManager& timerManager = TimerManager::getInstance();
    if (timerManager.getMainMenuWindow()) {
        timerManager.startTimer(
            timerManager.getMainMenuWindow(),
            TimerManager::WindowType::MAINMENU);
    }

    QDialog::closeEvent(event);
}

void Balance::on_btnBack_clicked()
{
    this->close();
}


void Balance::getBalanceData()
{
    QString site_url = Environment::base_url()+"/account/"+accountid;
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
        qDebug() << "Server Response:" << response;

        QJsonDocument json_doc = QJsonDocument::fromJson(response.toUtf8());

        if (!json_doc.isNull() && json_doc.isArray()) {
            QJsonArray json_array = json_doc.array();

            if (!json_array.isEmpty()) {
                QJsonObject accountData = json_array[0].toObject();
                updateUI(accountData);
            } else {
                ui->labelBalance->setText("Error: Account not found");
            }
        } else {
            ui->labelBalance->setText("Error: Invalid JSON format");
            qDebug() << "Invalid JSON format. Raw response:" << response;
        }
    }
    else
    {
        ui->labelBalance->setText("Error getting balance");
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}

void Balance::getCustomerInfo()
{
    QString site_url = Environment::base_url() + "/customer/name/" + accountid;
    QNetworkRequest request((QUrl(site_url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QNetworkReply *customerReply = customerManager->get(request);
    connect(customerReply, &QNetworkReply::finished, this, [this, customerReply](){
        if (customerReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = customerReply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObj = jsonDoc.object();
            QString customerName = jsonObj["name"].toString();

            if (!customerName.isEmpty()) {
                ui->labelOwner->setText(customerName);
            } else {
                ui->labelOwner->setText("Ei nimeä");
            }
        } else {
            ui->labelOwner->setText("Virhe");
            qDebug() << "Customer info error:" << customerReply->errorString();
        }
        customerReply->deleteLater();
    });
}

void Balance::getCreditLimitData()
{
    QString site_url = Environment::base_url()+"/account/"+accountid;
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), "Bearer " + myToken);

    QNetworkReply *creditLimitReply = creditLimitManager->get(request);
    connect(creditLimitReply, &QNetworkReply::finished, this, [this, creditLimitReply](){
        if(creditLimitReply->error() == QNetworkReply::NoError)
        {
            QByteArray response = creditLimitReply->readAll();


            QJsonDocument json_doc = QJsonDocument::fromJson(response);
            if (json_doc.isArray()) {
                QJsonArray json_array = json_doc.array();
                if (!json_array.isEmpty()) {
                    QJsonObject jsonObj = json_array[0].toObject();
                    if (jsonObj.contains("creditlimit")) {
                        double creditLimit = jsonObj["creditlimit"].toString().toDouble();
                        ui->labelCreditLimit->setText(QString("Credit Limit: %1 €").arg(creditLimit, 0, 'f', 2));
                    } else {
                        ui->labelCreditLimit->setText("Credit Limit: Not Available");
                    }
                }
            } else {
                QJsonObject jsonObj = json_doc.object();
                if (jsonObj.contains("creditlimit")) {
                    double creditLimit = jsonObj["creditlimit"].toString().toDouble();
                    ui->labelCreditLimit->setText(QString("Credit Limit: %1 €").arg(creditLimit, 0, 'f', 2));
                } else {
                    ui->labelCreditLimit->setText("Credit Limit: Not Available");
                }
            }
        }
        else
        {
            ui->labelCreditLimit->setText("Error getting credit limit");
            qDebug() << "Error:" << creditLimitReply->errorString();
        }
        creditLimitReply->deleteLater();
    });
}

void Balance::getRecentTransactions()
{
    response_data=reply->readAll();
    qDebug()<<response_data;
    QString start = QString::number(s);
    QString end = QString::number(e);

    QString site_url = Environment::base_url() + "/transactions/" + accountid+"/"+start+"/"+end;
    QNetworkRequest request((QUrl(site_url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QNetworkReply *transactionsReply = transactionsManager->get(request);
    connect(transactionsReply, &QNetworkReply::finished, this, [this, transactionsReply]() {
        if (transactionsReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = transactionsReply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonArray jsonArray = jsonDoc.array();

            if (!jsonArray.isEmpty()) {
                QString transactionsText;
                for (const QJsonValue &value : jsonArray) {
                    QJsonObject obj = value.toObject();
                    QString datetime = obj["datetime"].toString();
                    QString type = obj["transaction"].toString();
                    QString amount = obj["amount"].toString();
                    transactionsText += datetime + " - " + type + ": " + amount + "€\n";
                }
                ui->textTransactions->setText(transactionsText);
            } else {
                ui->textTransactions->setText("No transactions found");
            }
        } else {
            ui->textTransactions->setText("Error loading transactions");
            qDebug() << "Transactions error:" << transactionsReply->errorString();
        }
        reply->deleteLater();
        transactionsManager->deleteLater();
    });
}

void Balance::updateUI(const QJsonObject &accountData)
{
    // Saldo
    if (accountData.contains("balance")) {
        double balance = accountData["balance"].toString().toDouble();
        ui->labelBalance->setText(QString("Current Balance: %1 €").arg(balance, 0, 'f', 2));
    } else {
        ui->labelBalance->setText("Error: Missing balance data");
    }

    ui->textTransactions->setText("No transactions found");
}

void Balance::resetInactivityTimer()
{
    inactivityTimer->start(10000);
}

bool Balance::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::KeyPress ||
        event->type() == QEvent::MouseButtonPress)
    {
        resetInactivityTimer();
    }
    return QDialog::eventFilter(obj, event);
}
