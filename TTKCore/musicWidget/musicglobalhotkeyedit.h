#ifndef MUSICGLOBALHOTKEYEDIT_H
#define MUSICGLOBALHOTKEYEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include "musicglobaldefine.h"

/*! @brief The class of the global hotKey lineEdit.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicGlobalHotKeyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicGlobalHotKeyEdit(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicGlobalHotKeyEdit();

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    void keyPressEvent(QKeyEvent *event);
    /*!
     * Override the widget event.
     */

};

#endif // MUSICGLOBALHOTKEYEDIT_H
