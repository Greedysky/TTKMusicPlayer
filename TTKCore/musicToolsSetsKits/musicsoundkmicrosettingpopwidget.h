#ifndef MUSICSOUNDKMICROSETTINGPOPWIDGET_H
#define MUSICSOUNDKMICROSETTINGPOPWIDGET_H

#include "musictoolmenuwidget.h"

class MusicSoundKMicroSettingPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = 0);

    /*!
     * Object contsructor.
     */
    virtual ~MusicSoundKMicroSettingPopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

};

#endif // MUSICSOUNDKMICROSETTINGPOPWIDGET_H
