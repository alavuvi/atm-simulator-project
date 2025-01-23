#include "environment.h"
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
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
}

Login::~Login()
{
    delete ui;
}

void Login::setCardNumber(const QString &cardNumber)
{
    ui->labelCardnumber->setText(cardNumber);
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
    qDebug()<<response_data;
    reply->deleteLater();
    postManager->deleteLater();
}
