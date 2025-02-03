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
    Q_PROPERTY(QByteArray myToken READ getMyToken WRITE setMyToken)

public:
    explicit SelectAccount(QWidget *parent = nullptr);
    ~SelectAccount();

    void setMyToken(const QByteArray &newMyToken);
    QByteArray getMyToken() const { return myToken; }
    void SetAccountID(const QJsonArray &newAccountID);

private slots:
    void on_btnDebit_clicked();
    void on_btnCredit_clicked();

private:
    void processAccounts();
    void handleCreditLimitResponse(QNetworkReply *reply);

    Ui::SelectAccount *ui;
    QByteArray myToken;
    QJsonArray accountID;
    QNetworkAccessManager *networkManager;
    int creditAccountId;  // Tilille, jossa creditlimit > 0
    int debitAccountId;   // Tilille, jossa creditlimit == 0
};

#endif // SELECTACCOUNT_H
