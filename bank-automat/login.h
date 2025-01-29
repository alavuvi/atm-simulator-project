#ifndef LOGIN_H
#define LOGIN_H

#include "accounttype.h"
#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>


namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    void setCardNumber(const QString &cardNumber);
    ~Login();

    void handleFailedLogin();
public slots:
    void handleLoginTimeout();

private slots:
    void onNumberButtonClicked();
    void onOkButtonClicked();
    void onBackButtonClicked();

    // login slot network
    void loginSlot (QNetworkReply *reply);


private:
    Ui::Login *ui;
    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;

    int failedAttempts;
    QTimer *loginTimeoutTimer;

    void startLoginTimeout();
    void resetFailedAttempts();

    void handleMultipleAccounts(const QJsonArray &accounts, const QByteArray &token);
    void openMainMenu(const QString &accountType, const QByteArray &token);
    AccountType *accountTypeWidget;
};

#endif // LOGIN_H
