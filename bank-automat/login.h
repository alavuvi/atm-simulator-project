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

    void setCardId(const QString &newCardId);
    void setMyToken(const QByteArray &newMyToken);

public slots:
    void loginSlot(QNetworkReply *reply);

private:
    Ui::Login *ui;

    int failedAttempts;
    QNetworkAccessManager *loginManager;
    QNetworkAccessManager *cardStatusManager;
    QByteArray response_data;
    QNetworkReply *reply;
    QByteArray myToken;

    void resetFailedAttempts();

    void onNumberButtonClicked();
    void onBackButtonClicked();
    void onOkButtonClicked();

    void handleAccountsResponse(QNetworkReply *reply);
    void updateCardStatus(const QString &cardId);
    void handleUpdateStatusResponse(QNetworkReply *reply);
};

#endif // LOGIN_H
