#include "login.h"
#include "ui_login.h"
#include "environment.h"
#include "mainmenu.h"
#include "selectaccount.h"
#include "timermanager.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , failedAttempts(0)
{
    ui->setupUi(this);

    // Asetetaan pinOutput-tekstikenttään EchoMode Password
    ui->pinOutput->setEchoMode(QLineEdit::Password);

    // QList ja for-loop numeronapeille.
    QList<QPushButton*> numberButtons = {
        ui->button00, ui->button01, ui->button02, ui->button03, ui->button04,
        ui->button05, ui->button06, ui->button07, ui->button08, ui->button09
    };

    for (QPushButton* button : numberButtons) {
        connect(button, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    }

    // Ok- ja back-napit
    connect(ui->buttonOk, &QPushButton::clicked, this, &Login::onOkButtonClicked);
    connect(ui->buttonBack, &QPushButton::clicked, this, &Login::onBackButtonClicked);

    // Aloita 10 sekunnin ajastus kirjautumiselle
    TimerManager::getInstance().startTimer(this, TimerManager::WindowType::LOGIN);
}

Login::~Login()
{
    delete ui;
}

void Login::resetFailedAttempts()
{
    failedAttempts = 0;
}

void Login::setCardId(const QString &newCardId)
{
    ui->labelCardId->setText(newCardId);
}

void Login::onNumberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        // Resetoi ajastin, kun käyttäjä painaa numeronappia
        TimerManager::getInstance().resetTimer();

        QString currentText = ui->pinOutput->text();
        if (currentText.length() < 4)
        {
            ui->pinOutput->setText(currentText + button->text());
        }
    }
}

void Login::onBackButtonClicked()
{
    // Resetoi ajastin, kun käyttäjä painaa back-nappia
    TimerManager::getInstance().resetTimer();

    QString currentText = ui->pinOutput->text();
    currentText.chop(1);
    ui->pinOutput->setText(currentText);
}

void Login::onOkButtonClicked()
{
    QJsonObject jsonObj;
    jsonObj.insert("idcard", ui->labelCardId->text());
    jsonObj.insert("pin", ui->pinOutput->text());

    QString site_url = Environment::base_url() + "/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    loginManager = new QNetworkAccessManager(this);

    connect(loginManager, &QNetworkAccessManager::finished, this, &Login::loginSlot);
    reply = loginManager->post(request, QJsonDocument(jsonObj).toJson());
}

void Login::loginSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    if(response_data.length() < 2){
        qDebug() << "Server is not responding!";
        ui->labelInfo->setText("Server is not Responding!!");
    }
    else {
        if(response_data == "-11") {
            ui->labelInfo->setText("Database Error!");
        }
        if(response_data =="-12") {
            QMessageBox::warning(this, "Card locked!",
                                 "You're card is locked! Please, contact sysadmin!");
            ui->labelInfo->setText("Card is locked! Contact sysadmin!");
            TimerManager::getInstance().stopTimer();
            this->close();
        }
        else {
            if(response_data != "false" && response_data.length() > 20) {
                ui->labelInfo->setText("Login OK");

                setMyToken(response_data);
                qDebug() << "Token asetettu:" << myToken;

                QByteArray authHeader = "Bearer " + myToken;
                QString cardId = ui->labelCardId->text();
                QString accounts_url = Environment::base_url() + "/accountsbycard/" + cardId;
                QNetworkRequest accountsRequest(accounts_url);
                accountsRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                accountsRequest.setRawHeader("Authorization", authHeader);

                QNetworkAccessManager *accountsManager = new QNetworkAccessManager(this);
                connect(accountsManager, &QNetworkAccessManager::finished, this, &Login::handleAccountsResponse);

                accountsManager->get(accountsRequest);
            }
            else {
                failedAttempts++;
                if(failedAttempts >= 3){
                    QMessageBox::warning(this, "PIN Error!",
                                         "Wrong PIN entered 3 times. Card is now locked! Please, contact sysadmin!");
                    ui->labelInfo->setText("Wrong PIN entered 3 times. Card is locked!");
                    QString cardId = ui->labelCardId->text();
                    updateCardStatus(cardId);
                    TimerManager::getInstance().stopTimer();
                    this->close();
                }
                else{
                    ui->labelInfo->setText("Wrong PIN entered!");
                }
            }
        }
        reply->deleteLater();
        loginManager->deleteLater();
    }
}

void Login::handleAccountsResponse(QNetworkReply *reply)
{
    response_data=reply->readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray accountsArray = json_doc.array();

    int accountCount = accountsArray.size();
    qDebug() << "Tilit kortilla: "<<accountCount;
    if(accountCount > 1){
        TimerManager::getInstance().stopTimer(); //pysäytetään ajastin siirryttäessä eteenpäin
        SelectAccount *objSelectAccount = new SelectAccount(this);
        objSelectAccount->setMyToken(myToken);
        objSelectAccount->setAccountId(accountsArray);
        objSelectAccount->open();

        this->close();
    }

    else if (accountCount == 1) {
        QJsonObject jsonObj = accountsArray[0].toObject();
        int accountNumber = jsonObj["idaccount"].toInt();
        QString accountID = QString::number(accountNumber);
        qDebug() << "Korttiin liitetty accountId:" << accountID;
        TimerManager::getInstance().stopTimer(); //pysäytetään ajastin siirryttäessä eteenpäin

        MainMenu *objMainMenu = new MainMenu(nullptr);
        objMainMenu->setMyToken(myToken);
        objMainMenu->setAccountId(accountID);
        objMainMenu->open();

        this->close();

    }
    else{
        qDebug() << "Ei tiliä linkitettynä korttiin!";
    }
    reply->deleteLater();
}

void Login::updateCardStatus(const QString &cardId)
{
    QString site_url = Environment::base_url() + "/lockcard";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj.insert("idcard", cardId);

    cardStatusManager = new QNetworkAccessManager(this);
    connect(cardStatusManager, &QNetworkAccessManager::finished,
            this, &Login::handleUpdateStatusResponse);

    cardStatusManager->post(request, QJsonDocument(jsonObj).toJson());
}

void Login::handleUpdateStatusResponse(QNetworkReply *reply)
{
    QByteArray response_data = reply->readAll();
    qDebug() << "Kortin tila päivitetty:" << response_data;

    reply->deleteLater();
    cardStatusManager->deleteLater();
}

void Login::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}
