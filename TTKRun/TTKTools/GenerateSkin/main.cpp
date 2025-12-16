#include <QApplication>
#include "ttkinitialization.h"
#include "toolskin.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TTK_REGISTER_ORGANIZATION(TTK_APP);

    ToolSkin w;
    w.show();
    return app.exec();
}
