#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QNetworkReply>
#include <QTimer>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void setCardNumber(const QString &cardNumber);
    void setMyToken(const QByteArray &newMyToken);

public slots:
    void loginSlot(QNetworkReply *reply);

private:
    Ui::Login *ui;
    int failedAttempts;
    QTimer *loginTimeoutTimer;
    QNetworkAccessManager *postManager;
    QByteArray response_data;
    QNetworkReply *reply;
    QByteArray myToken;

    void startLoginTimeout();
    void resetFailedAttempts();

    void handleLoginTimeout();
    void onNumberButtonClicked();
    void onBackButtonClicked();
    void onOkButtonClicked();
    void handleAccountsResponse(QNetworkReply *reply);
};

#endif // LOGIN_H
