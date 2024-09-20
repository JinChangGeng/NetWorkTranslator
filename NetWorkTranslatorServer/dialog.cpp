#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //数据库
    sqlConnect();
    server = new QTcpServer(this);
    //启动监听
    bool result = server->listen(QHostAddress::Any,6699);
    if(!result)
    {
        ui->textBrowser->append("监听失败");
    }
    // 新连接来了的通知的槽函数连接
    connect(server,SIGNAL(newConnection()),
            this,SLOT(newConnectedSlot()));
    ui->textBrowser->append("监听成功，端口号为：6699");
}

Dialog::~Dialog()
{
    if(server->isListening())
        server->close();
    delete ui;
    delete server;
}
//新连接
void Dialog::newConnectedSlot()
{
    socket = server->nextPendingConnection();

    connect(socket,SIGNAL(disconnected()),
            this,SLOT(isClientConnectedSlot()));

    connect(socket,SIGNAL(readyRead()),
            this,SLOT(ReadSlot()));

    // 获取对方IP
    QString ip = socket->peerAddress().toString();
    // 获取对方端口号
    quint16 port = socket->peerPort();
    ip.prepend("新连接来了！").append(":").append(QString::number(port));
    qDebug()<<ip;
    ui->textBrowser->append(ip);
}
void Dialog::isClientConnectedSlot()
{
    // 获取对方IP
    QString ip = socket->peerAddress().toString();
    // 获取对方端口号
    quint16 port = socket->peerPort();
    ip.prepend("客户端已断开！").append(":").append(QString::number(port));
    ui->textBrowser->append(ip);
}
//精确查找
void Dialog::ReadSlot()
{
    QByteArray buffer = socket->readAll();
    QString text(buffer);
    // 获取标志位
    QChar flag = text.at(0);  // 获取第一个字符
    QString word = text.mid(1); // 去掉第一个字符
    if(flag=='1')
    {
        //精确查找
        QString selectSql = "SELECT * FROM englishwords WHERE word=:word";
        QSqlQuery sqlSelect;
        if(word=="")
        {
            return;
        }
        sqlSelect.prepare(selectSql);
        sqlSelect.bindValue(":word",word);
        //绑定后的SQL语句
        if(sqlSelect.exec())
        {
            while(sqlSelect.next())// 循环取出
            {
                QString word=sqlSelect.value(0).toString();
                QString pro=sqlSelect.value(1).toString();
                QString mean=sqlSelect.value(2).toString();
                QString sendtext=word+"-"+pro+"-"+mean+"\n";
                //发送
                QByteArray sendBuffer = sendtext.toUtf8();
                socket->write(sendBuffer);
            }
        }
    }
    //模糊查找
    else if(flag=='0')
    {
        QString selectSql = "SELECT * FROM englishwords WHERE word LIKE ? limit 5";
        //获得用户输入
        QSqlQuery sqlSelectLike;
        //预处理
        sqlSelectLike.prepare(selectSql);
        sqlSelectLike.addBindValue(word.append("%"));
        //qDebug()<<word; %
        if(word=="%")
        {
            return;
        }
        if(sqlSelectLike.exec())
        {
            while(sqlSelectLike.next())// 循环取出
            {
                QString word=sqlSelectLike.value(0).toString();
                QString pro=sqlSelectLike.value(1).toString();
                QString mean=sqlSelectLike.value(2).toString();
                QString sendtext=word+"-"+pro+"-"+mean+"\n";
                //发送
                QByteArray sendBuffer = sendtext.toUtf8();
                socket->write(sendBuffer);
            }
        }

    }
}
void Dialog::sqlConnect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("englishwords.db");
    if(db.open())
    {
        qDebug()<<"连接成功";
    }
    else
    {
        //返回错误信息
        QSqlError errorInfo=db.lastError();
        //提取错误文本
        QString errorText=errorInfo.text();
        // 展示错误
        QMessageBox::critical(this,"错误",errorText);
    }
}
