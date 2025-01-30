#ifndef SELECTACCOUNTDIALOG_H
#define SELECTACCOUNTDIALOG_H

#include <QDialog>
#include <QJsonArray>

namespace Ui {
class SelectAccountDialog;
}

class SelectAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAccountDialog(QWidget *parent, const QJsonArray &accounts);
    ~SelectAccountDialog();

signals:
    void accountSelected(const QString &accountType); // Vain yksi parametri

private slots:
    void on_buttonDebit_clicked();
    void on_buttonCredit_clicked();

private:
    Ui::SelectAccountDialog *ui;
    QJsonArray linkedAccounts;
};

#endif // SELECTACCOUNTDIALOG_H
