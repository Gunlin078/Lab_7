#include "rest_clientwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    REST_ClientWindow w;
    w.show();
    return a.exec();
}
