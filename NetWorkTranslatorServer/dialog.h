#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QByteArray>
#include <QMessageBox>
#include <QIODevice>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QSqlDatabase db;
    void sqlConnect();
    void selectWord();
private slots:
    void newConnectedSlot();
    void isClientConnectedSlot();
    void ReadSlot();

};

#endif // DIALOG_H
