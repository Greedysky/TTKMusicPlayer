#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicRightAreaWidget(QWidget *parent = 0);

    void setupUi(Ui::MusicApplication* ui);

signals:
public slots:

protected:
    Ui::MusicApplication *m_ui;

};

#endif // MUSICRIGHTAREAWIDGET_H
