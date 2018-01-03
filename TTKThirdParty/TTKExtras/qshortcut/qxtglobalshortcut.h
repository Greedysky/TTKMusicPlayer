#ifndef QXTGLOBALSHORTCUT_H
#define QXTGLOBALSHORTCUT_H

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

#include "qxtglobal.h"
#include <QObject>
#include <QKeySequence>

class QxtGlobalShortcutPrivate;

/*! @brief The namespace of the qxt global shortcut.
 * @author libqxt <foundation@libqxt.org>
 */
class MUSIC_EXTRAS_EXPORT QxtGlobalShortcut : public QObject
{
    Q_OBJECT
    QXT_DECLARE_PRIVATE(QxtGlobalShortcut)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)
public:
    explicit QxtGlobalShortcut(QObject* parent = 0);
    explicit QxtGlobalShortcut(const QKeySequence& shortcut, QObject* parent = 0);

    virtual ~QxtGlobalShortcut();

    QKeySequence shortcut() const;
    bool setShortcut(const QKeySequence& shortcut);

    bool isEnabled() const;

Q_SIGNALS:
    void activated();

public Q_SLOTS:
    void setEnabled(bool enabled = true);
    void setDisabled(bool disabled = true);

};

#endif // QXTGLOBALSHORTCUT_H
