#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include "musicabstractmovedialog.h"

class QProcess;

namespace Ui {
class MusicTransformWidget;
}

/*! @brief The class of the transform widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTransformWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTransformWidget)
public:
    enum class Module
    {
        Music,  /*!< Music module*/
        Krc     /*!< Krc module*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicTransformWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTransformWidget();

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
     * Set diff button clicked by index.
     */
    void buttonClicked(int index);
    /*!
     * Override exec function.
     */
    virtual int exec();

private:
    /*!
     * Get transform song name.
     */
    QString transformSongName() const;
    /*!
     * Init control parameter.
     */
    void initialize();
    /*!
     * Start a process to transform.
     */
    bool processTransform();
    /*!
     * Set control enable false when it begin.
     */
    void setCheckedControl(bool enabled);

    Ui::MusicTransformWidget *m_ui;
    QProcess *m_process;
    QStringList m_path;
    Module m_currentType;

};

#endif // MUSICTRANSFROMWIDGET_H
