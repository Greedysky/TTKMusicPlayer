#ifndef MUSICQUALITYCHOICEWIDGET_H
#define MUSICQUALITYCHOICEWIDGET_H

#include <QToolButton>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicQualityChoiceWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceWidget(QWidget *parent = 0);
    ~MusicQualityChoiceWidget();

signals:

public slots:
};

#endif // MUSICQUALITYCHOICEWIDGET_H
