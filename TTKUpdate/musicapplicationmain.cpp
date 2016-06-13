#include "musicupdatemainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(argc == 2)
    {
        MusicUpdateMainWindow w;
        w.show();
        w.setFilePath(argv[1], QString());
        return a.exec();
    }
    return 0;
}
