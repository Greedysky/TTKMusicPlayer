#include "musicconsolemodule.h"
#include "ttkinitialization.h"
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QGuiApplication>
using TTKApplication = QGuiApplication;
#else
#  include <QApplication>
using TTKApplication = QApplication;
#endif

int main(int argc, char *argv[])
{
    TTKApplication app(argc, argv);
    RegisterOrganization(TTK_APP);

    MusicConsoleModule console;
    return console.initialize() ? EXIT_SUCCESS : EXIT_FAILURE;
}
