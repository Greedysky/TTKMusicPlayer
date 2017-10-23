#ifndef MUSICRUNOBJECT_H
#define MUSICRUNOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QDebug>
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicRunObjectPrivate;

/*! @brief The class of the music run object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicRunObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRunObject(QObject *parent = 0);

    /*!
     * Check current setting file's validation.
     */
    void checkValid();
    /*!
     * To run main window.
     */
    void run(int argc, char **argv);

private Q_SLOTS:
    /*!
     * Run finished.
     */
    void finished(int code);

private:
    MUSIC_DECLARE_PRIVATE(MusicRunObject)

};

#endif // MUSICRUNOBJECT_H
