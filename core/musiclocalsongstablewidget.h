#ifndef MUSICLOCALSONGSTABLEWIDGET_H
#define MUSICLOCALSONGSTABLEWIDGET_H

#include "musictablewidgetabstract.h"

class MusicLocalSongsTableWidget : public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongsTableWidget(QWidget *parent = 0);

    void clearShowlist();
    void clearShowPath();
    void createShowlist();
    void createShowPath();

public slots:
    virtual void listCellClicked(int, int){}

};

#endif // MUSICLOCALSONGSTABLEWIDGET_H
