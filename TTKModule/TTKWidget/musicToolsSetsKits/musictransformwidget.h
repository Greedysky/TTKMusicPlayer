#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

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

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovedialog.h"

#define LINE_WIDTH 380

class QProcess;

namespace Ui {
class MusicTransformWidget;
}

/*! @brief The class of the transform widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicTransformWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTransformWidget)
public:
    enum TransformType
    {
        Music,  /*!< Music category*/
        Lrc,    /*!< Lrc category*/
    };
    /*!
     * Object contsructor.
     */
    explicit MusicTransformWidget(QWidget *parent = nullptr);

    virtual ~MusicTransformWidget();

public Q_SLOTS:
    /*!
     * Selected input path.
     */
    void initInputPath();
    /*!
     * Selected out path.
     */
    void initOutputPath();
    /*!
     * Start to transform.
     */
    void startTransform();
    /*!
     * Transform finished.
     */
    void transformFinish();
    /*!
     * Input is dir not file.
     */
    void folderBoxChecked();
    /*!
     * Transform krc file to lrc file.
     */
    void krc2lrcBoxChecked(bool check);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Get transform song name.
     */
    QString getTransformSongName() const;
    /*!
     * Init control parameter.
     */
    void initControlParameter() const;
    /*!
     * Start a process to transform.
     */
    bool processTransform(const QString &para) const;
    /*!
     * Set music control enable or false when trans lrc.
     */
    void setMusicCheckedControl(bool enable);
    /*!
     * Set control enable false when it begin.
     */
    void setCheckedControl(bool enable);

    Ui::MusicTransformWidget *m_ui;
    QProcess *m_process;
    QStringList m_path;
    TransformType m_currentType;

};

#endif // MUSICTRANSFROMWIDGET_H
