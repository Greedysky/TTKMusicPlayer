#ifndef MUSICLRCTRANSLATEDWIDGET_H
#define MUSICLRCTRANSLATEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

/*! @brief The class of the lrc translate widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcTranslatedWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcTranslatedWidget(QWidget *parent = 0);

    ~MusicLrcTranslatedWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current translated string.
     */
    void setPlainText(const QString &name, const QString &text);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QLabel *m_titleName, *m_plainText;

};

#endif // MUSICLRCTRANSLATEDWIDGET_H
