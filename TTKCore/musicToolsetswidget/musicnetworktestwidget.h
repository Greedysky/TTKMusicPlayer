#ifndef MUSICNETWORKTESTWIDGET_H
#define MUSICNETWORKTESTWIDGET_H

#include <QWidget>
#include "musicglobaldefine.h"

namespace Ui {
class MusicNetworkTestWidget;
}

class MUSIC_TOOLSET_EXPORT MusicNetworkTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicNetworkTestWidget(QWidget *parent = 0);
    ~MusicNetworkTestWidget();

signals:

public slots:

protected:
    Ui::MusicNetworkTestWidget *ui;

};

#endif // MUSICNETWORKTESTWIDGET_H
