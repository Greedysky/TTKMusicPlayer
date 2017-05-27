#ifndef MUSICSOUNDKMICROSETTINGPOPWIDGET_H
#define MUSICSOUNDKMICROSETTINGPOPWIDGET_H

#include "musictoolmenuwidget.h"

class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSettingPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = 0);

    /*!
     * Object contsructor.
     */

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
