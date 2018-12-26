#include "clientdialog.h"
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore>
#include <QDebug>
#include <iostream>

clientdialog::clientdialog(const QUrl &url,bool debug,QWidget *parent)
    : QWidget(parent),m_url(url),m_debug(debug)
{
    //布局1
    QLabel *iplabel = new QLabel("IP地址");
    m_iplineedit = new QLineEdit;
    QLabel *portlabel = new QLabel("端口");
    m_portspinbox = new QSpinBox;
    m_portspinbox->setRange(0,65535);
    m_linkbutton = new QPushButton("连接");
    m_disconnectbutton = new QPushButton("断开");
    pButtonGroup = new QButtonGroup();//提供了一个容器来组织按钮部件组
    pButtonGroup->setExclusive(true);//任何一个时间内操作组中只有一个切换操作可以被激活。如果用户选择组中的其它切换操作，那么被用户选择的被激活，其它的变为非活动的。
    m_linkbutton->setCheckable(true);//有两种状态，按下/弹起
    m_disconnectbutton->setCheckable(true);
    pButtonGroup->addButton(m_linkbutton,0);
    pButtonGroup->addButton(m_disconnectbutton,1);
    QHBoxLayout *qhboxlayout1 = new QHBoxLayout;//水平布局
    qhboxlayout1->addWidget(iplabel);//添加控件
    qhboxlayout1->addWidget(m_iplineedit);
    qhboxlayout1->addWidget(portlabel);
    qhboxlayout1->addWidget(m_portspinbox);
    qhboxlayout1->addWidget(m_linkbutton);
    qhboxlayout1->addWidget(m_disconnectbutton);

    //布局2
    QLabel *sendmessagelabel = new QLabel("发送消息");
    QHBoxLayout *qhboxlayout2 = new QHBoxLayout;
    qhboxlayout2->addWidget(sendmessagelabel);

    //布局3
    m_sendmessagetextedit = new QTextEdit;
    m_sendmessagetextedit->setFixedHeight(50);//高度设为50，不变
    m_sendbutton = new QPushButton("发送");
    m_sendbutton->setFixedHeight(50);//高度设为50，不变
    QHBoxLayout *qhboxlayout3 = new QHBoxLayout;//添加控件
    qhboxlayout3->addWidget(m_sendmessagetextedit);
    qhboxlayout3->addWidget(m_sendbutton);

    //布局4
    QLabel *receivemessagelabel = new QLabel("接收消息");
    QHBoxLayout *qhboxlayout4 = new QHBoxLayout;
    qhboxlayout4->addWidget(receivemessagelabel);

    //布局5
    m_receivemessageTextEdit = new QTextEdit;
    QHBoxLayout *qhboxlayout5 = new QHBoxLayout;
    qhboxlayout5->addWidget(m_receivemessageTextEdit);
    m_receivemessageTextEdit->setReadOnly(true);//设置控件为只读

    //布局6
    statusLabel = new QLabel("连接状态");
    m_clean = new QPushButton("清除");
    QHBoxLayout *qhboxlayout6 = new QHBoxLayout;
    qhboxlayout6->addWidget(statusLabel);
    qhboxlayout6->addStretch();//添加一个可拉伸空间
    qhboxlayout6->addWidget(m_clean);

    //主页面布局
    QVBoxLayout *mainlayout = new QVBoxLayout;//垂直布局
    mainlayout->addLayout(qhboxlayout1,1);//添加布局到盒子
    mainlayout->addLayout(qhboxlayout2,0.5);
    mainlayout->addLayout(qhboxlayout3,1);
    mainlayout->addLayout(qhboxlayout4,0.5);
    mainlayout->addLayout(qhboxlayout5,3);
    mainlayout->addLayout(qhboxlayout6,1);
    setLayout(mainlayout);
    setWindowTitle("Websocket Client");

    connect(m_linkbutton,SIGNAL(clicked(bool)),this,SLOT(connectToServer()));//监测按钮
    connect(m_disconnectbutton,SIGNAL(clicked(bool)),this,SLOT(stopClicked()));
    connect(m_sendbutton,SIGNAL(clicked(bool)),this,SLOT(onSendButtonClicked()));
    connect(m_clean,SIGNAL(clicked(bool)),this,SLOT(onCleanButtonClicked()));
    connect(&m_websocket,SIGNAL(connected()),this,SLOT(onconnected()));//监测套接字
    connect(&m_websocket,SIGNAL(disconnected()),this,SLOT(closeConnection()));
    connect(&m_websocket,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextMessageReceived(QString)));
}

clientdialog::~clientdialog()
{
    m_websocket.errorString();//套接字错误信息
    m_websocket.close();//关闭套接字
}

//断开连接操作
void clientdialog::closeConnection()
{
    m_linkbutton->setEnabled(true);
    m_disconnectbutton->setEnabled(false);
    m_sendmessagetextedit->setEnabled(false);
    m_sendbutton->setEnabled(false);
    m_receivemessageTextEdit->setEnabled(false);
    m_clean->setEnabled(false);
    statusLabel->setText(tr("断开连接"));
}

//连接服务器
void clientdialog::connectToServer()
{
    QString path = QString("ws://%1:%2").arg(m_iplineedit->text()).arg(m_portspinbox->text());
    QUrl url = QUrl(path);
    m_websocket.open(url);//连接
}

//连接上以后
void clientdialog::onconnected()
{
    statusLabel->setText(tr("连接"));
    m_linkbutton->setEnabled(false);//禁用控件
    m_disconnectbutton->setEnabled(true);//启用控件
    m_sendmessagetextedit->setEnabled(true);
    m_sendbutton->setEnabled(true);
    m_receivemessageTextEdit->setEnabled(true);
    m_clean->setEnabled(true);
}

//收到消息
void clientdialog::onTextMessageReceived(const QString &message)
{
    QString time = current_date_time->currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    m_receivemessageTextEdit->setText(time + "\n" + message);
    //创建文件对象
    QFile file("log.txt");
    //读写打开文件
    bool ok = file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    if(!ok)
    {
        qDebug() << "file open error:" << file.errorString();
        return ;
    }
    //写文件
    QTextStream out(&file);
    out << time <<endl;
    out << message << endl;
    //关闭文件
    file.close();
}

//断开
void clientdialog::stopClicked()
{
    m_websocket.close();
}

//发送消息
void clientdialog::onSendButtonClicked()
{
    QString msg = m_sendmessagetextedit->document()->toPlainText();
    m_websocket.sendTextMessage(msg);
}

//清除内容
void clientdialog::onCleanButtonClicked()
{
    m_receivemessageTextEdit->clear();
}
