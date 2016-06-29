#ifndef MUSICTOASTLABEL_H
#define MUSICTOASTLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QTimer>
#include "musicglobaldefine.h"

/*! @brief The class of the toast widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToastLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MusicToastLabel(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicToastLabel(const QString &text, QWidget *parent = 0);
    /*!
     * Object contsructor by context text.
     */
    ~MusicToastLabel();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setTimerInterval(int msecond);
    /*!
     * Set timer interval to hide.
     */
    int getTimerInterval() const;
    /*!
     * Get timer interval.
     */
    void setFontSize(int size);
    /*!
     * Set context font size.
     */
    int getFontSize() const;
    /*!
     * Get context font size.
     */
    void setBold(bool bold);
    /*!
     * Set context font bold.
     */
    bool bold() const;
    /*!
     * Get context font bold.
     */

public Q_SLOTS:
    void setText(const QString &text);
    /*!
     * Set context text.
     */

private Q_SLOTS:
    void closeAnimation();
    /*!
     * Close animation when tiem out.
     */

protected:
    QTimer m_timer;
    QFont m_font;

};

#endif // MUSICTOASTLABEL_H
