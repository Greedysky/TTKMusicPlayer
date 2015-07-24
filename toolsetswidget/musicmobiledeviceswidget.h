#ifndef MUSICMOBILEDEVICESWIDGET_H
#define MUSICMOBILEDEVICESWIDGET_H

#include <QLabel>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class QToolButton;

class MUSIC_EXPORT MusicMobileDevicesWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicMobileDevicesWidget(QWidget *parent = 0);
    ~MusicMobileDevicesWidget();

signals:

public slots:
    void showMobileManager();

protected:
    QToolButton *m_closeButton;
    QToolButton *m_openButton;

};

#endif // MUSICMOBILEDEVICESWIDGET_H
