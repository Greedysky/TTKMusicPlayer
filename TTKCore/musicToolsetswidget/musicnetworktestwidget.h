#ifndef MUSICNETWORKTESTWIDGET_H
#define MUSICNETWORKTESTWIDGET_H

#include "musicglobaldefine.h"
#include "musicabstractmovedialog.h"

class MusicNetworkTestThread;

namespace Ui {
class MusicNetworkTestWidget;
}

class MUSIC_TOOLSET_EXPORT MusicNetworkTestWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicNetworkTestWidget(QWidget *parent = 0);
    ~MusicNetworkTestWidget();

public Q_SLOTS:
    virtual int exec();
    void networkData(long upload, long download);

protected:

    Ui::MusicNetworkTestWidget *ui;
    MusicNetworkTestThread *m_thead;
    long m_totalUp, m_totalDown;

};

#endif // MUSICNETWORKTESTWIDGET_H
