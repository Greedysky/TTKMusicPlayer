#ifndef MUSICGLOBALHOTKEYEDIT_H
#define MUSICGLOBALHOTKEYEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicGlobalHotKeyEdit(QWidget *parent = 0);

    ~MusicGlobalHotKeyEdit();

    /*!
     * Get class object name.
     */
    static QString getClassName();

protected:
    /*!
     * Override the widget event.
     */
    void keyPressEvent(QKeyEvent *event);

};

#endif // MUSICGLOBALHOTKEYEDIT_H
