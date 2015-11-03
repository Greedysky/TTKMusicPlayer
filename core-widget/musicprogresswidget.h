#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

#define WIDTH_S  20
#define HEIGHT_S 20

class QLabel;

#include <QProgressDialog>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicProgressWidget : public QProgressDialog
{
    Q_OBJECT
public:
    explicit MusicProgressWidget(QWidget *parent = 0);
    ~MusicProgressWidget();

    void setTitle(const QString &name);

public slots:
    void show();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void initWidget();

    QLabel *m_background, *m_textLabel;
    QProgressBar *m_progressBar;

};

#endif // MUSICPROGRESSWIDGET_H
