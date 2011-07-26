#include <QtGui/QApplication>
#include "qgoban.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGoban w;
    w.show();
    return a.exec();
}
