#ifndef MUSICVOLUMEGAINWIDGET_H
#define MUSICVOLUMEGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the volume gain table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicVolumeGainTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVolumeGainTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVolumeGainTableWidget(QWidget *parent = nullptr);

    virtual ~MusicVolumeGainTableWidget();

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

};


namespace Ui {
class MusicVolumeGainWidget;
}
class QProcess;
/*! @brief The class of the volume gain widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicVolumeGainWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVolumeGainWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVolumeGainWidget(QWidget *parent = nullptr);

    virtual ~MusicVolumeGainWidget();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();

private Q_SLOTS:
    /*!
     * Add input file button clicked.
     */
    void addFileButtonClicked();
    /*!
     * Add input files button clicked.
     */
    void addFilesButtonClicked();
    /*!
     * Remove input file button clicked.
     */
    void rmFileButtonClicked();
    /*!
     * Remove input files button clicked.
     */
    void rmFilesButtonClicked();
    /*!
     * Analysis input files button clicked.
     */
    void analysisButtonClicked();
    /*!
     * Apply input files button clicked.
     */
    void applyButtonClicked();
    /*!
     * Music gain volume input value changed.
     */
    void volumeLineTextChanged(const QString &text);
    /*!
     * Analysis output by process.
     */
    void analysisOutput();
    /*!
     * Apply output by process.
     */
    void applyOutput();

protected:
    /*!
     * Create table item finished.
     */
    void createItemFinished(const QString &track, const QString &album);
    /*!
     * Enable or disable control state.
     */
    void setControlEnable(bool enable);

    Ui::MusicVolumeGainWidget *m_ui;
    QProcess *m_process;
    QStringList m_paths;
    int m_currentIndex;

};

#endif // MUSICVOLUMEGAINWIDGET_H
