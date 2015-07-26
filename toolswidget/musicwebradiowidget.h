#ifndef MUSICWEBRADIOWIDGET_H
#define MUSICWEBRADIOWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "musicobject.h"
#include "musicmovewidgetabstract.h"

class QProcess;
class QListWidgetItem;
class MusicWebRadioDatabase;

namespace Ui {
class MusicWebRadioWidget;
}

class MUSIC_WIDGET_EXPORT MusicWebRadioWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicWebRadioWidget(QWidget *parent = 0);
    ~MusicWebRadioWidget();

signals:

public slots:
    void radioPlay();
    void radioStop();
    void radioVolume(int num);
    void timeout();
    void updateRadioList(const QString&);
    void updateRecentList();
    void updateFavouriteList();
    void itemHasDoubleClicked(QListWidgetItem*);
    void radioStandardOutput();
    void radioColletButton();
    void radioDiscolletButton();

protected:
    virtual void closeEvent(QCloseEvent *event);
    void clearAllItems();
    Ui::MusicWebRadioWidget *ui;
    QProcess *m_radio;
    QTimer m_timer;
    int m_timerCount;
    QString m_radioUrl;
    QString m_currentRadioName;
    QIcon *m_collecticon;
    QIcon *m_discollecticon;
    MusicWebRadioDatabase *m_database;

};

#endif // MUSICWEBRADIOWIDGET_H
