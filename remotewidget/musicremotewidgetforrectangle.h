#ifndef MUSICREMOTEWIDGETFORRECTANGLE_H
#define MUSICREMOTEWIDGETFORRECTANGLE_H

#include "musicremotewidget.h"

class MUSIC_EXPORT MusicRemoteWidgetForRectangle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForRectangle(QWidget *parent = 0);
    virtual ~MusicRemoteWidgetForRectangle();

    virtual void setLabelText(const QString&);
signals:

public slots:

protected:
    QLabel* m_songNameLabel;
    QWidget* m_toolWidget;

};

#endif // MUSICREMOTEWIDGETFORRECTANGLE_H
