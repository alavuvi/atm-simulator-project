#include "environment.h"
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , failedAttempts(0)
    // , lockoutTimer(new QTimer(this))
    , loginTimeoutTimer(new QTimer(this))
{
    ui->setupUi(this);

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

    // connect(lockoutTimer, &QTimer::timeout, this, &Login::handleLockoutTimeout);
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


// void Login::handleLockoutTimeout()
// {
//     ui->labelInfo->setText("Lukittu, palataan takaisin edelliseen ikkunaan.");
//     this->close();
// }


void Login::setCardNumber(const QString &cardNumber)
{
    ui->labelCardnumber->setText(cardNumber);
}

// Tämä suoritetaan, kun login ei onnistu 10 sekunnin sisällä
void Login::handleLoginTimeout()
{
    ui->labelInfo->setText("Palataan takaisin, oikeaa PIN-koodia ei annettu aikarajan sisällä!");
    QTimer::singleShot(5000, this, &Login::close);
   // this->close();
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

// Slot OK napille
void Login::onOkButtonClicked()
{
    QJsonObject jsonObj;
    // Tähän koodi millä tarkistetaan korttinumero ja pinkoodi backendistä
    jsonObj.insert("cardnumber", ui->labelCardnumber->text());
    jsonObj.insert("pin", ui->pinOutput->text());

    QString site_url=Environment::base_url()+"/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postManager = new QNetworkAccessManager(this);
    connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));
    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
}


// login slot tarkistaa palvelimelta kirjautumistiedot
void Login::loginSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    if(response_data.length()<2){
        qDebug()<<"Palvelin ei vastaa!";
        ui->labelInfo->setText("Palvelin ei vastaa!");
    }
    else {
        if(response_data=="-11") {
            ui->labelInfo->setText("Tietokanta virhe!");
        }
        else {
            if(response_data!="false" && response_data.length()>20) {
                ui->labelInfo->setText("Login OK");
                loginTimeoutTimer->stop();
            }
            else {
                failedAttempts++;
                if(failedAttempts >= 3){
                    ui->labelInfo->setText("Syötit väärän PIN koodin 3 kertaa. Suljetaan...");
                    // Aloittaa viiden sekunnin ajastimen ja sulkee ikkunan
                    QTimer::singleShot(5000, this, &Login::close);
                }
                else{
                    ui->labelInfo->setText("Väärä kortinnumero/PIN koodi");
                }
            }
    }
    reply->deleteLater();
    postManager->deleteLater();
}
}
