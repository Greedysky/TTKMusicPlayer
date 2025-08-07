#include <QApplication>
#include "ttkinitialization.h"
#include "toolfm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TTKRegisterOrganization(TTK_APP);

    ToolFM w;
    w.startToRequest();
    return app.exec();
}
