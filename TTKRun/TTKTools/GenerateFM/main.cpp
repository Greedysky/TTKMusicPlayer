#include <QApplication>
#include "ttkinitialization.h"
#include "toolfm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TTKRegisterOrganization(TTK_APP);

    ToolQTFM qtfm;
    qtfm.startToRequest();

    ToolXIFM xifm;
    xifm.startToRequest();

    return app.exec();
}
