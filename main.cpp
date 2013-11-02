#include "logo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logo w;
    w.show();

    return a.exec();
}
