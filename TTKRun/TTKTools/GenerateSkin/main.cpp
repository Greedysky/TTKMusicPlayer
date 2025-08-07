#include <QApplication>
#include "ttkinitialization.h"
#include "toolskin.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TTKRegisterOrganization(TTK_APP);

    ToolSkin w;
    w.show();
    return app.exec();
}
