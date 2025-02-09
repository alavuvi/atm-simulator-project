#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
<<<<<<< HEAD
#include <QTimer>
=======
>>>>>>> main

namespace Ui {
class Transactions;
}

class Transactions : public QDialog
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent = nullptr);
    ~Transactions();

<<<<<<< HEAD
    void setAccountId(const QString &id);

    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnShowTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);

=======
    void setAccountId(const QString &newAccountId);
    void setMyToken(const QByteArray &newMyToken);

private slots:
    void on_btnTransactions_clicked();
    void showTransactionsSlot(QNetworkReply *reply);


>>>>>>> main
private:
    Ui::Transactions *ui;
    QString accountid;
    QByteArray myToken;
    QNetworkAccessManager *transactionsManager;
<<<<<<< HEAD
    QNetworkReply *reply;
    QByteArray response_data;
=======
    QByteArray response_data;
    QNetworkReply *reply;
>>>>>>> main
};

#endif // TRANSACTIONS_H
