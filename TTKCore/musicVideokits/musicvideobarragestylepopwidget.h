#ifndef MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
#define MUSICVIDEOBARRAGESTYLEPOPWIDGET_H

#include "musictoolmenuwidget.h"

class MusicVideoBarrageStylePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicVideoBarrageStylePopWidget(QWidget *MusicToolMenuWidget = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

signals:
public slots:

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

};

#endif // MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
