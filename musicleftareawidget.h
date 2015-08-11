#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicLeftAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLeftAreaWidget(QWidget *parent = 0);

    void setupUi(Ui::MusicApplication* ui);

signals:
public slots:

protected:
    Ui::MusicApplication *m_ui;

};

#endif // MUSICLEFTAREAWIDGET_H
