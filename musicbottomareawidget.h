#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

#include <QWidget>
#include <QMenu>
#include "musiclibexportglobal.h"

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBottomAreaWidget(QWidget *parent = 0);

    void setupUi(Ui::MusicApplication* ui);

signals:
public slots:

protected:
    void createToolPopupMenu();
    
    Ui::MusicApplication *m_ui;
    QMenu m_toolPopupMenu;

};

#endif // MUSICBOTTOMAREAWIDGET_H
