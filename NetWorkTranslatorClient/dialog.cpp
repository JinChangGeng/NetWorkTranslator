#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent), bz('0'),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    group = new QButtonGroup(this);
    group->addButton(ui->pushButtonSelect,1);
    group->addButton(ui->pushButtonClear,2);
    group->addButton(ui->pushButtonConnect,3);
    socket =new  QTcpSocket(this);
    ui->pushButtonSelect->setEnabled(false);
    ui->pushButtonClear->setEnabled(false);
    connect(group,SIGNAL(buttonClicked(int)),
            this,SLOT(btnsClickedSlot(int)));

    //发送改变的单词
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(lineTextChangedSlot(QString)));
    // 读取信息的槽函数
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(ReadSlot()));
}

Dialog::~Dialog()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
    delete socket;
}
void Dialog::btnsClickedSlot(int id)
{
    if(id==1)
    {
        ui->textBrowser->clear();
        bz='1';
        QString word=ui->lineEdit->text();
        word.prepend(bz);
        QByteArray buffer = word.toUtf8();
        socket->write(buffer);
    }
    else if(id==2)
    {
        ui->textBrowser->clear();
        ui->lineEdit->clear();
    }
    else if(id==3)
    {
        QString buttontext=ui->pushButtonConnect->text();
        if(buttontext=="断开连接")
        {
            socket->close();
            ui->textBrowser->clear();
            ui->pushButtonConnect->setText("连接");
            ui->pushButtonSelect->setEnabled(false);
            ui->pushButtonClear->setEnabled(false);
            return;
        }

        ui->pushButtonClear->setEnabled(true);
        ui->pushButtonSelect->setEnabled(true);
        ui->pushButtonConnect->setText("断开连接");

        QString ip=ui->lineEditIP->text();
        qint64 port=ui->spinBox->value();
        socket->connectToHost(ip,port);
    }
}
void Dialog::ReadSlot()
{
    ui->textBrowser->clear();
    QByteArray buffer = socket->readAll();
    QString text(buffer);
    ui->textBrowser->append(text);
}
//把查的单词给服务器
void Dialog::lineTextChangedSlot(QString word)
{
    ui->textBrowser->clear();
    // 为模糊搜索添加标志位 '0'
    word.prepend("0");
    QByteArray buffer = word.toUtf8();
    socket->write(buffer);
}
