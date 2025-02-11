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

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &Balance::getBalanceData);
    refreshTimer->start(5000); // 5 sekunnin välein

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

void Balance::updateUI(const QJsonObject &accountData)
{
    // Saldo
    if (accountData.contains("balance")) {
        double balance = accountData["balance"].toString().toDouble();
        ui->labelBalance->setText(QString("Current Balance: %1 €").arg(balance, 0, 'f', 2));
    } else {
        ui->labelBalance->setText("Error: Missing balance data");
    }

    // Tili numero
    if (accountData.contains("accountnumber")) {
        QString accountNumber = accountData["accountnumber"].toString();
        ui->labelOwner->setText(QString("Erkki Esimerkki").arg(accountNumber));
    } else {
        ui->labelOwner->setText("Error: Missing account number data");
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
