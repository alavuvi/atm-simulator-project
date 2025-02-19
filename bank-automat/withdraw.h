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

    void setMyToken(const QByteArray &newMyToken);
    void setIdCard(const QString &newIdCard);
    void sendWithdrawRequest(int amount);

private slots:
    void on_btnWithdraw_clicked();
    void handleWithdrawReply();

private:
    Ui::Withdraw *ui;
    QNetworkAccessManager *networkManager;
    QByteArray myToken;
    QString idCard;
};

#endif
