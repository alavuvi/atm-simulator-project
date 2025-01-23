#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCardnumber_clicked()
{
    QString cardNumber = lineCardnumber->text();
    // Lähetä korttinumero backendille
    sendCardNumberToBackend(cardNumber);
}

void sendCardNumberToBackend(const QString &cardNumber) {
    // Tee HTTP-pyyntö Node.js backendille
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:3000/checkCardNumber"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["cardNumber"] = cardNumber;
    QJsonDocument doc(json);

    connect(manager, &QNetworkAccessManager::finished, this, [](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            // Käsittele vastaus
        } else {
            // Käsittele virhe
        }
        reply->deleteLater();
    });

    manager->post(request, doc.toJson());
}
};
