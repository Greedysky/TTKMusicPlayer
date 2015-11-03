#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

#include <QProgressDialog>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicProgressWidget : public QProgressDialog
{
    Q_OBJECT
public:
    explicit MusicProgressWidget(QWidget *parent = 0);
    ~MusicProgressWidget();

//    void setTitle(const QString &name);
//    void setValue(int value);
//    void setRange(int min, int max);

public slots:
    virtual int exec();

protected:


};

#endif // MUSICPROGRESSWIDGET_H
