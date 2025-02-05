#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QList ja for-loop numeronapeille.
    QList<QPushButton*> numberButtons = {
        ui->button00, ui->button01, ui->button02, ui->button03, ui->button04,
        ui->button05, ui->button06, ui->button07, ui->button08, ui->button09
    };

    for (QPushButton* button : numberButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    }

    // Ok- ja back-napit
    connect(ui->buttonOk, &QPushButton::clicked, this, &MainWindow::onOkButtonClicked);
    connect(ui->buttonBack, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
}

// Slotti numeronapeille
void MainWindow::onNumberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString buttonText = button->text();
        QString cardNumber = ui->lineCardnumber->text();
        cardNumber.append(buttonText);
        ui->lineCardnumber->setText(cardNumber);
    }
}

// Slot backspace napille
void MainWindow::onBackButtonClicked()
{
    QString cardnumber = ui->lineCardnumber->text();
    cardnumber.chop(1);
    ui->lineCardnumber->setText(cardnumber);
}

// Slot OK napille
void MainWindow::onOkButtonClicked()
{
    Login *objLogin=new Login(this);
    QString cardNumber = ui->lineCardnumber->text();
    objLogin->setCardNumber(cardNumber);
    ui->lineCardnumber->clear();
    objLogin->open();
}

MainWindow::~MainWindow()
{
    delete ui;
}
