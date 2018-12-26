#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QtWebSockets/QtWebSockets>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QButtonGroup>
#include <QObject>
#include <QWidget>
#include <QUrl>
#include <time.h>
#include <QByteArray>

class clientdialog : public QWidget
{
    Q_OBJECT

public:
    clientdialog(const QUrl &url,bool debug = false,QWidget *parent = 0);
    ~clientdialog();

Q_SIGNALS:
void closed();

private Q_SLOTS:
    void connectToServer();
    void onTextMessageReceived(const QString &message);
    void closeConnection();

public slots:
    void stopClicked();
    void onconnected();
    void onSendButtonClicked();
    void onCleanButtonClicked();

private:
    QLineEdit *m_iplineedit;
    QSpinBox *m_portspinbox;
    QPushButton *m_linkbutton;
    QPushButton *m_disconnectbutton;
    QTextEdit *m_sendmessagetextedit;
    QPushButton *m_sendbutton;
    QTextEdit *m_receivemessageTextEdit;
    QPushButton *m_clean;
    QLabel *statusLabel;
    QButtonGroup *pButtonGroup;
    QUrl m_url;
    QWebSocket m_websocket;
    bool m_debug;
    QDateTime *current_date_time;
};

#endif // CLIENTDIALOG_H
