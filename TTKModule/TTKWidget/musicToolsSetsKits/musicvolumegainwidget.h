#ifndef MUSICVOLUMEGAINWIDGET_H
#define MUSICVOLUMEGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the volume gain table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicVolumeGainTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicVolumeGainTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVolumeGainTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
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
public:
    explicit MusicVolumeGainWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVolumeGainWidget();

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
    void show();
    /*!
     * Override show function.
     */

private Q_SLOTS:
    void addFileButtonClicked();
    /*!
     * Add input file button clicked.
     */
    void addFilesButtonClicked();
    /*!
     * Add input files button clicked.
     */
    void rmFileButtonClicked();
    /*!
     * Remove input file button clicked.
     */
    void rmFilesButtonClicked();
    /*!
     * Remove input files button clicked.
     */
    void analysisButtonClicked();
    /*!
     * Analysis input files button clicked.
     */
    void applyButtonClicked();
    /*!
     * Apply input files button clicked.
     */
    void volumeLineTextChanged(const QString &text);
    /*!
     * Music gain volume input value changed.
     */
    void analysisOutput();
    /*!
     * Analysis output by process.
     */
    void applyOutput();
    /*!
     * Apply output by process.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createItemFinished(const QString &track, const QString &album);
    /*!
     * Create table item finished.
     */
    void setControlEnable(bool enable);
    /*!
     * Enable or disable control state.
     */

    Ui::MusicVolumeGainWidget *m_ui;
    QProcess *m_process;
    QStringList m_paths;
    int m_currentIndex;

};

#endif // MUSICVOLUMEGAINWIDGET_H
