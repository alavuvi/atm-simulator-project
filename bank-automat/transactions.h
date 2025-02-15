#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "qlabel.h"
#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setAccountId(const QString &newAccountId);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);
    void on_btn_older_clicked();
    void on_btn_newer_clicked();
    void on_btnBack_clicked();
    void handleCustomerInfo(QNetworkReply *reply);

private:
    Ui::Transactions *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
    QByteArray response_data;
    QNetworkReply *reply;
    QLabel *labelName;

    int s = 0;
    int e = 10;
    void loadTransactions();

    void getCustomerInfo();
};

#endif // TRANSACTIONS_H
