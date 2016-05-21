#ifndef KUGOUWINDOW_H
#define KUGOUWINDOW_H

#include <QWidget>
#include "kugouurl.h"

class MUSIC_EXTRAS_EXPORT KugouWindow : public QWidget
{
    Q_OBJECT
public:
    explicit KugouWindow(QWidget *parent = 0);
    ~KugouWindow();

};

#endif // KUGOUWINDOW_H
