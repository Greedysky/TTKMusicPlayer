#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class Spek;
namespace Ui {
class MusicSpectrumWidget;
}

typedef struct MUSIC_TOOLSET_EXPORT MusicSpectrum
{
    QString m_name;
    QWidget *m_obj;
}MusicSpectrum;
TTK_DECLARE_LISTS(MusicSpectrum)

/*! @brief The class of the music spectrum widget all.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicSpectrumWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSpectrumWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Reset window open flag.
     */

public Q_SLOTS:
    void tabIndexChanged(int index);
    /*!
     * Tab Index Changed.
     */
    void spectrumTypeChanged(int index);
    /*!
     * Spectrum Type Changed.
     */
    void show();
    /*!
     * Override show function.
     */
    void localFileButtonClicked();
    /*!
     * Local file button clicked.
     */
    void openFileButtonClicked();
    /*!
     * Open file button clicked.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void adjustWidgetLayout(int offset);
    /*!
     * Adjust widget layout.
     */
    void showSpectrum(const QString &name, bool state);
    /*!
     * Show spectrum by name and state.
     */
    int findSpectrumWidget(const QString &name);
    /*!
     * Find spectrum widget index by name.
     */
    void createSpekWidget();
    /*!
     * Create spek widget.
     */

    Spek *m_spekWidget;
    MusicSpectrums m_types;
    Ui::MusicSpectrumWidget *m_ui;

};

#endif // MUSICSPECTRUMWIDGET_H
