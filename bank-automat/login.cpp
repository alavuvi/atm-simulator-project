#include "login.h"
#include "ui_login.h"
#include "environment.h"
#include "mainmenu.h"
#include "selectaccount.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , failedAttempts(0)
    , loginTimeoutTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Aseta pinOutput-tekstikenttään EchoMode Password
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

    connect(loginTimeoutTimer, &QTimer::timeout, this, &Login::handleLoginTimeout);

    // Aloita 10 sekunnin ajastus kirjautumiselle
    startLoginTimeout();
}

Login::~Login()
{
    delete ui;
}

void Login::startLoginTimeout()
{
    loginTimeoutTimer->start(10000);
}

void Login::resetFailedAttempts()
{
    failedAttempts = 0;
}

void Login::setCardId(const QString &newCardId)
{
    ui->labelCardId->setText(newCardId);
}

// Tämä suoritetaan, kun login ei onnistu 10 sekunnin sisällä
void Login::handleLoginTimeout()
{
    ui->labelInfo->setText("Palataan takaisin, oikeaa PIN-koodia ei annettu aikarajan sisällä!");
    QTimer::singleShot(3000, this, &Login::close);
}

// Slotti numeronapeille
void Login::onNumberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString currentText = ui->pinOutput->text();
        if (currentText.length() < 4)
        {
            ui->pinOutput->setText(currentText + button->text());
        }
    }
}

// Slot backspace napille
void Login::onBackButtonClicked()
{
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
    //connect(loginManager, SIGNAL(finished(QNetworkReply*)),
    //        this, SLOT(loginSlot(QNetworkReply*)));
    //Alla uudella versiolla sama
    connect(loginManager, &QNetworkAccessManager::finished, this, &Login::loginSlot);
    reply = loginManager->post(request, QJsonDocument(jsonObj).toJson());
}

// Tarkistaa palvelimelta saadut tiedot tai saadaanko palvelimeen yhtettä
void Login::loginSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    if(response_data.length() < 2){
        qDebug() << "Palvelin ei vastaa!";
        ui->labelInfo->setText("Palvelin ei vastaa!");
    }
    else {
        if(response_data == "-11") {
            ui->labelInfo->setText("Tietokanta virhe!");
        }
        if(response_data =="-12") {
            ui->labelInfo->setText("Korttisi on lukittu!");
        }
        else {
            if(response_data != "false" && response_data.length() > 20) {
                ui->labelInfo->setText("Login OK");
                loginTimeoutTimer->stop();

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
                    ui->labelInfo->setText("Syötit väärän PIN koodin 3 kertaa. Korttisi on lukittu");
                    QString cardId = ui->labelCardId->text();
                    updateCardStatus(cardId);
                    // Aloittaa kolmen sekunnin ajastimen ja sulkee ikkunan
                    QTimer::singleShot(3000, this, &Login::close);
                }
                else{
                    ui->labelInfo->setText("Väärä Kortti-ID/PIN koodi");
                }
            }
        }
        reply->deleteLater();
        loginManager->deleteLater();
    }
}

// Käsitellään tilitiedot
void Login::handleAccountsResponse(QNetworkReply *reply)
{
    response_data=reply->readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray accountsArray = json_doc.array();

    int accountCount = accountsArray.size();
    qDebug() << "Tilit kortilla: "<<accountCount;
    if(accountCount > 1){
        SelectAccount *objSelectAccount = new SelectAccount(this);
        objSelectAccount->setMyToken(myToken);
        // qDebug() << "Token lähetty Select Account:" << myToken;
        objSelectAccount->setAccountId(accountsArray);
        objSelectAccount->open();

        this->close();
    }

    else if (accountCount == 1) {
        QJsonObject jsonObj = accountsArray[0].toObject();
        int accountNumber = jsonObj["idaccount"].toInt();
        QString accountID = QString::number(accountNumber);
        qDebug() << "Korttiin liitetty accountId:" << accountID;

        MainMenu *objMainMenu = new MainMenu(this);
        objMainMenu->open();
        objMainMenu->setAccountId(accountID);
        objMainMenu->setMyToken(myToken);
        // qDebug() << "Token lähetetty Main Menu:" << myToken;

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
