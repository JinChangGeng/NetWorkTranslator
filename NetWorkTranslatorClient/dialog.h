#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QButtonGroup>
#include <QByteArray>
#include <QTcpSocket>

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
    QButtonGroup *group;
     QTcpSocket *socket;
    void sqlConnect();
    void selectWord();
    QString bz;
private slots:
    void btnsClickedSlot(int);
    void ReadSlot();
    void lineTextChangedSlot(QString);

};

#endif // DIALOG_H
