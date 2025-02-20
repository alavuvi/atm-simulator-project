#ifndef BALANCE_H
#define BALANCE_H

#include <QDialog>
#include <QtNetwork>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class Balance;
}

class Balance : public QDialog
{
    Q_OBJECT

public:
    explicit Balance(QWidget *parent = nullptr);
    ~Balance();
    void setAccountId(const QString &newAccountId);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnBack_clicked();
    void balanceReceived();


private:
    Ui::Balance *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *networkManager;
    QNetworkAccessManager *customerManager;
    QNetworkAccessManager *creditLimitManager;
    QNetworkAccessManager *transactionsManager;
    QNetworkReply *reply;
    QTimer *refreshTimer;
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void getBalanceData();
    void getCustomerInfo();
    void getCreditLimitData();
    void getRecentTransactions();
    void updateUI(const QJsonObject &accountData);
    QString accountOwner;
    QStringList recentTransactions;
    int s = 0;
    int e = 10;
    QByteArray response_data;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent* event) override;
};

#endif // BALANCE_H
