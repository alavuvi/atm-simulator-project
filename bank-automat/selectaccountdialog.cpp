#include "selectaccountdialog.h"
#include "ui_selectaccountdialog.h"
#include <QDebug>

SelectAccountDialog::SelectAccountDialog(QWidget *parent, const QJsonArray &accounts) :
    QDialog(parent),
    ui(new Ui::SelectAccountDialog),
    linkedAccounts(accounts)
{
    ui->setupUi(this);

    // Tässä voit lisätä dynaamisia nappuloita tilien mukaan, jos tarpeen
}

SelectAccountDialog::~SelectAccountDialog()
{
    delete ui;
}

void SelectAccountDialog::on_buttonDebit_clicked()
{
    emit accountSelected("debit");
    this->accept();
}

void SelectAccountDialog::on_buttonCredit_clicked()
{
    emit accountSelected("credit");
    this->accept();
}
