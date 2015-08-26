#ifndef MUSICTIMERWIDGET_H
#define MUSICTIMERWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicmovedialogabstract.h"

namespace Ui {
class MusicTimerWidget;
}

class MUSIC_WIDGET_EXPORT MusicTimerWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicTimerWidget(QWidget *parent = 0);
    ~MusicTimerWidget();

    void setSongStringList(const QStringList &list);

signals:
    void timerParameterChanged();

public slots:
    void changeFirstWidget();
    void changeSecondWidget();
    void changeThreeWidget();
    void commitTheResults();
    void setEnabledControlFalse1();
    void setEnabledControlTrue1();
    void setEnabledControlFalse2();
    void setEnabledControlTrue2();
    void setEnabledControlFalse3();
    void setEnabledControlTrue3();

protected:
    Ui::MusicTimerWidget *ui;

    void initFirstWidget();
    void initSecondWidget();
    void initThreeWidget();
    void initComboParameter();
    void initParemeter();
    void writeParemeter() const;

    QStringList m_hour;
    QStringList m_second;
    QStringList m_repeat;

};

#endif // MUSICTIMERWIDGET_H
