#include "regdecoder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegDecoder w;
    w.setWindowTitle("regDecodec ver.20210418 (C++ Qt6)");
    w.show();

    return a.exec();
}
