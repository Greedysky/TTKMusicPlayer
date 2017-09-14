#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicFileInformationWidget;
}

/*! @brief The class of the file information widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicFileInformationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFileInformationWidget(QWidget *parent = 0);

    virtual ~MusicFileInformationWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set music file song path name.
     */
    void setFileInformation(const QString &name);

public Q_SLOTS:
    /*!
     * Open the music at local path.
     */
    void musicOpenFileDir();
    /*!
     * Open the selected user pixmap.
     */
    void musicOpenImageFileDir();
    /*!
     * Music advance clicked.
     */
    void musicAdvanceClicked();
    /*!
     * Music modify tag start.
     */
    void musicEditTag();
    /*!
     * Music modify tag save.
     */
    void musicSaveTag();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Eet editLine enable.
     */
    void setEditLineEnable(bool enable);

    Ui::MusicFileInformationWidget *m_ui;
    QString m_path, m_imagePath;
    bool m_advanceOn;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
