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
    MusicLrcDefines();
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    QStringList getInlineLrcSize() const;
    /*!
     * Get inline lrc size.
     */
    QStringList getDesktopLrcSize() const;
    /*!
     * Get desktop lrc size.
     */

    int findInlineLrcIndex(int size) const;
    /*!
     * Find inline lrc index.
     */
    int findInlinePreSize(int index) const;
    /*!
     * Find inline pre size.
     */
    int findInlineNextSize(int index) const;
    /*!
     * Find inline next size.
     */
    int findInlineFirstSize() const;
    /*!
     * Find inline first size.
     */
    int findInlineLastSize() const;
    /*!
     * Find inline last size.
     */

    int findDesktopLrcIndex(int size) const;
    /*!
     * Find desktop lrc index.
     */
    int findDesktopPreSize(int index) const;
    /*!
     * Find desktop pre size.
     */
    int findDesktopNextSize(int index) const;
    /*!
     * Find desktop next size.
     */
    int findDesktopFirstSize() const;
    /*!
     * Find desktop first size.
     */
    int findDesktopLastSize() const;
    /*!
     * Find desktop last size.
     */

protected:
    QStringList m_inlineLrcSize, m_desktopLrcSize;

};

#endif // MUSICLRCDEFINES_H
