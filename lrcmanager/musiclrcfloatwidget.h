#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

#include "musiclrcfloatabstractwidget.h"

class QPushButton;
class MusicLrcFloatPhotoWidget;
class MusicLrcFloatSettingWidget;

class MUSIC_WIDGET_EXPORT MusicLrcFloatWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatWidget(QWidget *parent = 0);
    ~MusicLrcFloatWidget();

public slots:
    void showFloatSettingWidget();
    void closeFloatSettingWidget();

protected:
    int m_valueX;
    QPushButton *m_more,*m_update,*m_search;
    QPushButton *m_wallp,*m_photo;
    MusicLrcFloatPhotoWidget* m_floatPhotoWidget;
    MusicLrcFloatSettingWidget* m_floatSettingWidget;

};

#endif // MUSICLRCFLOATWIDGET_H
