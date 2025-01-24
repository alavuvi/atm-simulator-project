#ifndef LOGIN_H
#define LOGIN_H

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

public slots:
    // void handleLockoutTimeout();
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

    // Login timerit
    int failedAttempts;
    // QTimer *lockoutTimer;
    QTimer *loginTimeoutTimer;

    void startLoginTimeout();
    void resetFailedAttempts();
};

#endif // LOGIN_H
