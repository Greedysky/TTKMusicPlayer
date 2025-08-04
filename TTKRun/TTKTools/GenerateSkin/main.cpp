#include <QApplication>
#include "ttkinitialization.h"
#include "toolskin.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RegisterOrganization(TTK_APP);

    ToolSkin w;
    w.show();
    return app.exec();
}
