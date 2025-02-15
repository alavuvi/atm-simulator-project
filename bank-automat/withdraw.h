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

    void setCardnumber(const QString &newCardnumber);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_pushButton_clicked();  // 20 €
    void on_pushButton_2_clicked(); // 40 €
    void on_pushButton_3_clicked(); // 60 €
    void on_pushButton_4_clicked(); // 80 €
    void on_pushButton_5_clicked(); // 100 €
    void onCustomWithdraw(); // oma summa
    void onWithdrawReply(QNetworkReply *reply);

private:
    void sendWithdrawRequest(int amount);

    Ui::Withdraw *ui;
    QString cardnumber;
    QByteArray myToken;
    QNetworkAccessManager *networkManager;
};

#endif
