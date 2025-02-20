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
    void setAccountId(const QString &accountId);

private slots:
    void onCustomAmountEntered();
    void handleNetworkReply(QNetworkReply *reply);
    void handleButtonClicked(int amount);
    void sendWithdrawRequest(int amount);

private:
    Ui::Withdraw *ui;
    QNetworkAccessManager *networkManager;
    QByteArray myToken;
    QString accountID;
};

#endif
