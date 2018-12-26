#include "clientdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QUrl url;
    clientdialog w(url);
    w.show();

    return a.exec();
}
