#include "tomogram.h"
#include "view.h"
#include <QApplication>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w(NULL);
    QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setVersion(3, 5);
        format.setProfile(QSurfaceFormat::CoreProfile);
        w.setFormat(format);
        w.LoadData("FOURDIX-1.bin");
        bool ok = false;
        int value = 0;
        value = QInputDialog::getInt(NULL, QString::fromUtf8("Change Axis"), QString::fromUtf8("0 - Z, 1 - Y, 2 - X"), 0, 0, 2, 1, &ok);
        if (ok) {
            w.setAxis(value);
        }
    w.show();
    return a.exec();
}
