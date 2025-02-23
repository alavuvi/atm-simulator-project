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
}

void Balance::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}

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
    QNetworkReply *balanceReply = qobject_cast<QNetworkReply*>(sender());
    if (!balanceReply) return;

    balanceReply->deleteLater();

    if(balanceReply->error() == QNetworkReply::NoError)
    {
        QString response = balanceReply->readAll();
        qDebug() << "Server Response:" << response;

        QJsonDocument json_doc = QJsonDocument::fromJson(response.toUtf8());

        if (!json_doc.isNull() && json_doc.isArray()) {
            QJsonArray json_array = json_doc.array();

            if (!json_array.isEmpty()) {
                QJsonObject accountData = json_array[0].toObject();
                updateUI(accountData);
                // Haetaan transaktiot vasta kun balance on haettu onnistuneesti
                getRecentTransactions();
            } else {
                ui->labelBalance->setText("<FONT COLOR='#FFFFFF'>Error: Account not found</FONT>");
            }
        } else {
            ui->labelBalance->setText("<FONT COLOR='#FFFFFF'>Error: Invalid JSON format</FONT>");
            qDebug() << "Invalid JSON format. Raw response:" << response;
        }
    }
    else
    {
        ui->labelBalance->setText("<FONT COLOR='#FFFFFF'>Error getting balance</FONT>");
        qDebug() << "Error:" << balanceReply->errorString();
    }
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
                ui->labelOwner->setText("<FONT COLOR='#FFFFFF'>Ei nimeä</FONT>");
            }
        } else {
            ui->labelOwner->setText("<FONT COLOR='#FFFFFF'>Virhe</FONT>");
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
                        ui->labelCreditLimit->setText("<FONT COLOR='#FFFFFF'>Credit Limit: Not Available</FONT>");
                    }
                }
            } else {
                QJsonObject jsonObj = json_doc.object();
                if (jsonObj.contains("creditlimit")) {
                    double creditLimit = jsonObj["creditlimit"].toString().toDouble();
                    ui->labelCreditLimit->setText(QString("<FONT COLOR='#FFFFFF'>Credit Limit: %1 €</FONT>").arg(creditLimit, 0, 'f', 2));
                } else {
                    ui->labelCreditLimit->setText("<FONT COLOR='#FFFFFF'>Credit Limit: Not Available</FONT>");
                }
            }
        }
        else
        {
            ui->labelCreditLimit->setText("<FONT COLOR='#FFFFFF'>Error getting credit limit</FONT>");
            qDebug() << "Error:" << creditLimitReply->errorString();
        }
        creditLimitReply->deleteLater();
    });
}

void Balance::getRecentTransactions()
{
    QString start = QString::number(s);
    QString end = QString::number(e);

    QString site_url = Environment::base_url() + "/transactions/" + accountid + "/" + start + "/" + end;
    QNetworkRequest request((QUrl(site_url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + myToken);

    QNetworkReply *transactionsReply = transactionsManager->get(request);

    connect(transactionsReply, &QNetworkReply::finished, this, [this, transactionsReply]() {
        transactionsReply->deleteLater();

        if (transactionsReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = transactionsReply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

            if (jsonDoc.isNull()) {
                ui->textTransactions->setText("<FONT COLOR='#FFFFFF'>Error: Invalid JSON response</FONT>");
                qDebug() << "Invalid JSON response for transactions";
                return;
            }

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
                ui->textTransactions->setText("<FONT COLOR='#FFFFFF'>No transactions found</FONT>");
            }
        } else {
            ui->textTransactions->setText("<FONT COLOR='#FFFFFF'>Error loading transactions</FONT>");
            qDebug() << "Transactions error:" << transactionsReply->errorString();
        }
    });
}

void Balance::updateUI(const QJsonObject &accountData)
{
    if (accountData.contains("balance")) {
        double balance = accountData["balance"].toString().toDouble();
        ui->labelBalance->setText(QString("<FONT COLOR='#FFFFFF'>Current Balance: %1 €</FONT>").arg(balance, 0, 'f', 2));
    } else {
        ui->labelBalance->setText("<FONT COLOR='#FFFFFF'>Error: Missing balance data</FONT>");
    }
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
