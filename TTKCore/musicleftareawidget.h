#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class MusicQualityChoiceWidget;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app left area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicLeftAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLeftAreaWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLeftAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Set up app ui.
     */
    void musictLoveStateClicked();
    /*!
     * Reset current music love icon state.
     */

Q_SIGNALS:
    void currentLoveStateChanged();
    /*!
     * Current music love icon state changed.
     */
    void currentDownloadStateChanged();
    /*!
     * Current music download icon state changed.
     */

public Q_SLOTS:
    void musicDownloadSongToLocal();
    /*!
     * To download music data from net to load.
     */
    void musicDownloadSongFinished();
    /*!
     * To download music data from net finished.
     */
    void musicStackedSongListWidgetChanged();
    /*!
     * Change to song list widget.
     */
    void musicStackedToolsWidgetChanged();
    /*!
     * Change to tool sets widget.
     */
    void musicStackedRadioWidgetChanged();
    /*!
     * Change to radio widget.
     */
    void musicStackedMyDownWidgetChanged();
    /*!
     * Change to my download widget.
     */
    void musicStackedMobileWidgetChanged();
    /*!
     * Change to mobile widget.
     */
    void musicAnalyzerSpectrumWidget();
    /*!
     * Show analyzer spectrum widget.
     */
    void musicProjectMSpectrumWidget();
    /*!
     * Show projectM spectrum widget.
     */

protected:
    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    QWidget *m_stackedWidget;
    MusicQualityChoiceWidget *m_qualityChoiceWidget;

};

#endif // MUSICLEFTAREAWIDGET_H
