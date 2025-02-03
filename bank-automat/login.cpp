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

    // Numeropainikkeet
    connect(ui->button00, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button01, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button02, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button03, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button04, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button05, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button06, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button07, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button08, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button09, &QPushButton::clicked, this, &Login::onNumberButtonClicked);

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

void Login::setCardNumber(const QString &cardNumber)
{
    ui->labelCardnumber->setText(cardNumber);
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

// Koodi korttinumero ja PIN-tiedon tarkistamiseen backendistä
void Login::onOkButtonClicked()
{

    QJsonObject jsonObj;
    jsonObj.insert("idcard", ui->labelCardnumber->text());
    jsonObj.insert("pin", ui->pinOutput->text());

    QString site_url = Environment::base_url() + "/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postManager = new QNetworkAccessManager(this);
    connect(postManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(loginSlot(QNetworkReply*)));

    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
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
        else {
            if(response_data != "false" && response_data.length() > 20) {
                ui->labelInfo->setText("Login OK");
                loginTimeoutTimer->stop();

                setMyToken(response_data);
                qDebug() << "Token asetettu:" << myToken;

                QByteArray authHeader = "Bearer " + myToken;

                // Tee GET-pyyntö linkitetyille tileille
                QString cardId = ui->labelCardnumber->text();
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
                    ui->labelInfo->setText("Syötit väärän PIN koodin 3 kertaa. Suljetaan...");
                    // Aloittaa kolmen sekunnin ajastimen ja sulkee ikkunan
                    QTimer::singleShot(3000, this, &Login::close);
                }
                else{
                    ui->labelInfo->setText("Väärä Kortti-ID/PIN koodi");
                }
            }
        }
        reply->deleteLater();
        postManager->deleteLater();
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
        objSelectAccount->setMyToken(myToken); // Verify this runs
        qDebug() << "Token lähetty Select Account:" << myToken;
        objSelectAccount->SetAccountID(accountsArray);
        objSelectAccount->open();

        this->close();
    }

    else if (accountCount == 1) {
        QJsonObject jsonObj = accountsArray[0].toObject();
        int accountNumber = jsonObj["idaccount"].toInt();
        QString accountID = QString::number(accountNumber);
        qDebug() << "accountId:" << accountID;

        MainMenu *objMainMenu = new MainMenu(this);
        objMainMenu->open();
        objMainMenu->setAccountid(accountID);
        objMainMenu->setMyToken(myToken);
        qDebug() << "Token lähetetty Main Menu:" << myToken;

        this->close();

    }
    else{
        qDebug() << "Ei tiliä linkitettynä korttiin!";
    }

    reply->deleteLater();
}

void Login::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
}
