#ifndef MUSICVIDEOQUALITYPOPWIDGET_H
#define MUSICVIDEOQUALITYPOPWIDGET_H

#include "musicobject.h"
#include "musictoolmenuwidget.h"

class MusicVideoQualityPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicVideoQualityPopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVideoQualityPopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQualityActionState();
    /*!
     * Set quality action state enable or disable.
     */
    void setQualityText(const QString &url);
    /*!
     * Set current media quality text.
     */

signals:
    void getMusicMvInfo(MusicObject::MusicSongAttributes &data);
    /*!
     * Get music mv information data.
     */
    void mvURLChanged(const QString &data);
    /*!
     * Set current media url by selected quality.
     */

public slots:
    void movieQualityChoiced(QAction *action);
    /*!
     * Movie quality choiced by index.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */
    QString findMVUrlByBitrate(int bitrate);
    /*!
     * Find mv url by given bitrate.
     */
    int findMVBitrateByUrl(const QString &url);
    /*!
     * Find mv bitrate by given url.
     */
    bool findExistByBitrate(int bitrate);
    /*!
     * Check given bitrate is exist or not.
     */

    QActionGroup *m_actionGroup;

};

#endif // MUSICVIDEOQUALITYPOPWIDGET_H
