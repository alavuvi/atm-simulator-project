#ifndef SELECTACCOUNT_H
#define SELECTACCOUNT_H

#include <QDialog>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "environment.h"

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
    void setAccountId(const QJsonArray &newAccountId);


private slots:
    void on_btnDebit_clicked();
    void on_btnCredit_clicked();

private:
    void processAccounts();
    void handleCreditLimitResponse(QNetworkReply *reply);

    Ui::SelectAccount *ui;
    QByteArray myToken;
    QJsonArray accountId;
    QNetworkAccessManager *networkManager;
    int creditAccountId;
    int debitAccountId;
};

#endif // SELECTACCOUNT_H
