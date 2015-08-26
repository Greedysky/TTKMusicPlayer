#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

class MusicSpectrumWidget;

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicLeftAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLeftAreaWidget(QWidget *parent = 0);
    ~MusicLeftAreaWidget();

    void setupUi(Ui::MusicApplication* ui);

signals:

public slots:
    void musicSpectrumWidget();
    void musicStackedSongListWidgetChanged();
    void musicStackedToolsWidgetChanged();
    void musicStackedRadioWidgetChanged();
    void musicStackedMyDownWidgetChanged();
    
protected:
    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    QWidget *m_stackedWidget;
    MusicSpectrumWidget *m_musicSpectrumWidget;

};

#endif // MUSICLEFTAREAWIDGET_H
