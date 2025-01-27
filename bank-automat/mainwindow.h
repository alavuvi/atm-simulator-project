#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QlineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNumberButtonClicked();
    void onOkButtonClicked();
    void onBackButtonClicked();


// void on_btnCardnumber_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
