#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class Spek;
class QCheckBox;

namespace Ui {
class MusicSpectrumWidget;
}

/*! @brief The class of the music spectrum object.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOLSET_EXPORT MusicSpectrum
{
    QString m_name;
    QWidget *m_obj;
}MusicSpectrum;
MUSIC_DECLARE_LISTS(MusicSpectrum)

/*! @brief The class of the music spectrum widget all.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumWidget(QWidget *parent = 0);

    virtual ~MusicSpectrumWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

public Q_SLOTS:
    /*!
     * Tab Index Changed.
     */
    void tabIndexChanged(int index);
    /*!
     * Spectrum Type Changed.
     */
    void spectrumTypeChanged(int index);
    /*!
     * Spectrum Plus Type Changed.
     */
    void spectrumPlusTypeChanged(int index);
    /*!
     * Spectrum Dazzle Type Changed.
     */
    void spectrumDazzleTypeChanged(int index);
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Local file button clicked.
     */
    void localFileButtonClicked();
    /*!
     * Open file button clicked.
     */
    void openFileButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * New spectrum widget.
     */
    void newSpectrumWidget(QCheckBox *box, const QString &name, QLayout *layout);
    /*!
     * New spek widget.
     */
    void newSpekWidget(QCheckBox *box, const QString &name, QLayout *layout);
    /*!
     * Adjust widget layout.
     */
    void adjustWidgetLayout(int offset);
    /*!
     * Show spectrum by name and state.
     */
    void showSpectrum(const QString &name, bool state);
    /*!
     * Find spectrum widget index by name.
     */
    int findSpectrumWidget(const QString &name);
    /*!
     * FSpek state button clicked.
     */
    void fspekStateChanged();

    MusicSpectrums m_types;
    Ui::MusicSpectrumWidget *m_ui;

};

#endif // MUSICSPECTRUMWIDGET_H
