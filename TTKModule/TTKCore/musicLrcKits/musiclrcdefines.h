#ifndef MUSICLRCDEFINES_H
#define MUSICLRCDEFINES_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStringList>
#include "musicglobaldefine.h"

/*! @brief The class of the lrc defines.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcDefines
{
public:
    /*!
     * Object contsructor.
     */
    MusicLrcDefines();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get inline lrc size.
     */
    QStringList getInlineLrcSize() const;
    /*!
     * Get desktop lrc size.
     */
    QStringList getDesktopLrcSize() const;

    /*!
     * Find inline lrc index.
     */
    int findInlineLrcIndex(int size) const;
    /*!
     * Find inline pre size.
     */
    int findInlinePreSize(int index) const;
    /*!
     * Find inline next size.
     */
    int findInlineNextSize(int index) const;
    /*!
     * Find inline first size.
     */
    int findInlineFirstSize() const;
    /*!
     * Find inline last size.
     */
    int findInlineLastSize() const;

    /*!
     * Find desktop lrc index.
     */
    int findDesktopLrcIndex(int size) const;
    /*!
     * Find desktop pre size.
     */
    int findDesktopPreSize(int index) const;
    /*!
     * Find desktop next size.
     */
    int findDesktopNextSize(int index) const;
    /*!
     * Find desktop first size.
     */
    int findDesktopFirstSize() const;
    /*!
     * Find desktop last size.
     */
    int findDesktopLastSize() const;

protected:
    QStringList m_inlineLrcSize, m_desktopLrcSize;

};

#endif // MUSICLRCDEFINES_H
