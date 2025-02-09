#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setAccountId(const QString &newAccountId);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);


private:
    Ui::Transactions *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
    QByteArray response_data;
    QNetworkReply *reply;
};

#endif // TRANSACTIONS_H
