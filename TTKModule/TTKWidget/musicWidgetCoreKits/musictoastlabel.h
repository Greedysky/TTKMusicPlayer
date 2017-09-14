#ifndef MUSICTOASTLABEL_H
#define MUSICTOASTLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicToastLabel(QWidget *parent = 0);
    /*!
     * Object contsructor by context text.
     */
    explicit MusicToastLabel(const QString &text, QWidget *parent = 0);

    ~MusicToastLabel();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set text margin in the body.
     */
    void setFontMargin(int height, int width);
    /*!
     * Set timer interval to hide.
     */
    void setTimerInterval(int msecond);
    /*!
     * Get timer interval.
     */
    int getTimerInterval() const;
    /*!
     * Set context font size.
     */
    void setFontSize(int size);
    /*!
     * Get context font size.
     */
    int getFontSize() const;
    /*!
     * Set context font bold.
     */
    void setBold(bool bold);
    /*!
     * Get context font bold.
     */
    bool bold() const;
    /*!
     * Popup the toast widget in parent widget.
     */
    void popup(QWidget *parent);

public Q_SLOTS:
    /*!
     * Set context text.
     */
    void setText(const QString &text);

private Q_SLOTS:
    /*!
     * Close animation when tiem out.
     */
    void closeAnimation();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    QTimer m_timer;
    QFont m_font;
    QPoint m_margin;

};

#endif // MUSICTOASTLABEL_H
