#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QTimer>

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

    void setAccountId(const QString &id);

    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnShowTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);

private:
    Ui::Transactions *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // TRANSACTIONS_H
