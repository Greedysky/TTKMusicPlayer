#ifndef QXTGLOBALSHORTCUT_H
#define QXTGLOBALSHORTCUT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
