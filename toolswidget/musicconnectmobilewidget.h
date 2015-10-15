#ifndef MUSICCONNECTMOBILEWIDGET_H
#define MUSICCONNECTMOBILEWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

class QStackedWidget;

class MUSIC_TOOL_EXPORT MusicConnectMobileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicConnectMobileWidget(QWidget *parent = 0);
    ~MusicConnectMobileWidget();

signals:

public slots:
    void changeStatckedWidgetFirst();
    void changeStatckedWidgetSecond();
    void changeStatckedWidgetThird();

protected:
    void initFirstWidget();
    void initSecondWidget();
    void initThirdWidget();

    QStackedWidget *m_stackedWidget;

};

#endif // MUSICCONNECTMOBILEWIDGET_H
