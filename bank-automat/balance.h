#ifndef BALANCE_H
#define BALANCE_H

#include <QDialog>
#include <QtNetwork>
#include <QJsonDocument>

namespace Ui {
class Balance;
}

class Balance : public QDialog
{
    Q_OBJECT

public:
    explicit Balance(QWidget *parent = nullptr);
    ~Balance();
    void setCardnumber(const QString &newCardnumber);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnBack_clicked();
    void balanceReceived();

private:
    Ui::Balance *ui;
    QString cardnumber;
    QByteArray myToken;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    void getBalanceData();
};

#endif // BALANCE_H
