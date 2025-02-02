#ifndef SELECTACCOUNT_H
#define SELECTACCOUNT_H

#include "environment.h"
#include <QDialog>
#include <QObject>
#include <QDebug>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class SelectAccount;
}

class SelectAccount : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAccount(QWidget *parent = nullptr);
    ~SelectAccount();

    void setMyToken(const QByteArray &newMyToken);
    void SetAccountID(const QJsonArray &newAccountID);



private slots:
    void on_btnDebit_clicked();
    void on_btnCredit_clicked();

private:
    Ui::SelectAccount *ui;
    QNetworkAccessManager *accountManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QByteArray myToken;
    QJsonArray accountID;
 //   QString debitAccount;
 //   QString creditAccount;
};

#endif // SELECTACCOUNT_H
