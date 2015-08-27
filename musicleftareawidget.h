#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

class MusicSpectrumWidget;
class MusicLocalSongSearch;

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
    QString getSearchedText() const;
    void clearSearchedText()const;

signals:

public slots:
    void musicSearch();
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
    MusicLocalSongSearch *m_musicLocalSongSearch;

};

#endif // MUSICLEFTAREAWIDGET_H
