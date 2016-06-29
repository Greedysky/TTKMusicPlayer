#ifndef MUSICMESSAGEBOX_H
#define MUSICMESSAGEBOX_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicMessageBox;
}

/*! @brief The class of the messagebox widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMessageBox : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicMessageBox(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicMessageBox(const QString &text, QWidget *parent = 0);
    /*!
     * Object contsructor by context text.
     */
    MusicMessageBox(const QString &title, const QString &text,
                    QWidget *parent = 0);
    /*!
     * Object contsructor by context text and title.
     */
    virtual ~MusicMessageBox();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setTitle(const QString &text) const;
    /*!
     * Set box title.
     */
    QString title() const;
    /*!
     * Get box title.
     */

    void setText(const QString &text) const;
    /*!
     * Set context text.
     */
    QString text() const;
    /*!
     * Get context text.
     */

public Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Confirm or cancal button clicked, set the state.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    Ui::MusicMessageBox *ui;
    int m_status;

};

#endif // MUSICMESSAGEBOX_H
