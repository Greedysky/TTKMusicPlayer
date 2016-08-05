#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "musicautotest.h"
#include <QObject>

class MainWindow : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init();

};
//DECLARE_TEST(MainWindow)

#endif // MAINWINDOW_H
