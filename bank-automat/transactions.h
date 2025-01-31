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

    void setCardnumber(const QString &newCardnumber);

    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnShowTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);

private:
    Ui::Transactions *ui;
    QString cardnumber;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // TRANSACTIONS_H
