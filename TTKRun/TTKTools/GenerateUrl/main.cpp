#include <QApplication>
#include "ttkinitialization.h"
#include "toolurl.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TTK_REGISTER_ORGANIZATION(TTK_APP);

    ToolUrl w;
    w.show();
    return app.exec();
}
