#ifndef MUSICREPLAYGAINWIDGET_H
#define MUSICREPLAYGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

/*! @brief The class of the replay gain table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicReplayGainTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicReplayGainTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicReplayGainTableWidget(QWidget *parent = nullptr);

    virtual ~MusicReplayGainTableWidget();

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;

};


namespace Ui {
class MusicReplayGainWidget;
}
class Light;
class QProcess;
/*! @brief The class of the replay gain widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicReplayGainWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicReplayGainWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicReplayGainWidget(QWidget *parent = nullptr);

    virtual ~MusicReplayGainWidget();

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
     * Music gain input value changed.
     */
    void lineTextChanged(const QString &text);
    /*!
     * Analysis output by process.
     */
    void analysisOutput();
    /*!
     * Apply output by process.
     */
    void applyOutput();
    /*!
     * Confirm Data changed.
     */
    void confirmDataChanged();

protected:
    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Create table item finished.
     */
    void createItemFinished(const QString &track, const QString &album);
    /*!
     * Enable or disable control state.
     */
    void setControlEnabled(bool enable);

    Ui::MusicReplayGainWidget *m_ui;
    QProcess *m_process;
    QStringList m_paths;
    Light *m_replayGainWidget;
    int m_currentIndex;

};

#endif // MUSICREPLAYGAINWIDGET_H
