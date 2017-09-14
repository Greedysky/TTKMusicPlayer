#ifndef MUSICMESSAGEBOX_H
#define MUSICMESSAGEBOX_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicMessageBox(QWidget *parent = 0);
    /*!
     * Object contsructor by context text.
     */
    explicit MusicMessageBox(const QString &text, QWidget *parent = 0);
    /*!
     * Object contsructor by context text and title.
     */
    MusicMessageBox(const QString &title, const QString &text,
                    QWidget *parent = 0);

    virtual ~MusicMessageBox();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set box title.
     */
    void setTitle(const QString &text) const;
    /*!
     * Get box title.
     */
    QString title() const;
    /*!
     * Set context text.
     */
    void setText(const QString &text) const;
    /*!
     * Get context text.
     */
    QString text() const;

public Q_SLOTS:
    /*!
     * Confirm or cancal button clicked, set the state.
     */
    void buttonClicked(int index);
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * Override show function.
     */
    void show();

protected:
    Ui::MusicMessageBox *m_ui;
    int m_status;

};

#endif // MUSICMESSAGEBOX_H
