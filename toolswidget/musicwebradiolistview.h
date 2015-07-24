#ifndef MUSICWEBRADIOLISTVIEW_H
#define MUSICWEBRADIOLISTVIEW_H

#include <QListWidget>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MusicWebRadioWidget;

class MUSIC_EXPORT MusicWebRadioListView : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicWebRadioListView(QWidget *parent = 0);
    ~MusicWebRadioListView();

signals:

public slots:
    void itemHasClicked(QListWidgetItem*);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicWebRadioWidget *m_radioDialog;

};

#endif // MUSICWEBRADIOLISTVIEW_H
