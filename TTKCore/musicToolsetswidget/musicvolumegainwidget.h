#ifndef MUSICVOLUMEGAINWIDGET_H
#define MUSICVOLUMEGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
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


public Q_SLOTS:
    virtual void listCellClicked(int row, int col) override;
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
class MUSIC_TOOLSET_EXPORT MusicVolumeGainWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicVolumeGainWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVolumeGainWidget();

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
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
    void createItemFinished(const QString &track, const QString &album);
    /*!
     * Create table item finished.
     */
    void setControlEnable(bool enable);
    /*!
     * Enable or disable control state.
     */

    Ui::MusicVolumeGainWidget *ui;
    QProcess *m_process;
    QStringList m_paths;
    int m_currentIndex;

};

#endif // MUSICVOLUMEGAINWIDGET_H
