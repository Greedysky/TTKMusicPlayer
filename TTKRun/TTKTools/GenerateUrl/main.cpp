#include <QApplication>
#include "ttkinitialization.h"
#include "toolurl.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RegisterOrganization(TTK_APP);

    ToolUrl w;
    w.show();
    return app.exec();
}
