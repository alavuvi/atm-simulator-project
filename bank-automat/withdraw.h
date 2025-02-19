#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

    void setMyToken(const QByteArray &token);
    void setIdCard(const QString &cardId);
    void sendWithdrawRequest(int amount);

private slots:
    void onCustomAmountEntered();
    void handleNetworkReply(QNetworkReply *reply);
    void handleAccountInfoForWithdrawal(QNetworkReply *reply);

private:
    Ui::Withdraw *ui;
    QNetworkAccessManager *networkManager;
    QNetworkAccessManager *accountsManager;
    QByteArray myToken;
    QString idCard;
    QString accountID;
};

#endif
