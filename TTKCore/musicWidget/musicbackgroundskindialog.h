#ifndef MUSICBACKGROUNDSKINDIALOG_H
#define MUSICBACKGROUNDSKINDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundSkinDialog;
}

class QListWidgetItem;

/*! @brief The class of the local background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundSkinDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicBackgroundSkinDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicBackgroundSkinDialog();

    void setCurrentBgTheme(const QString &theme, int alpha, int listAlpha);
    /*!
     * Select current item by name\alpha\listAlpha when the widget show.
     */
    void updateBackground();
    /*!
     * Update this bg when user change the current bg.
     */
    int getListBgSkinAlpha() const;
    /*!
     * Get kist bg skin alpha.
     */

Q_SIGNALS:
    void currentTextChanged(const QString &text);
    /*!
     * Update app bg when user change the current bg emit.
     */

public Q_SLOTS:
    void changeToMySkin();
    /*!
     * Change index to my skin.
     */
    void changeToNetSkin();
    /*!
     * Change index to remote skin.
     */
    void showPaletteDialog();
    /*!
     * Change index to palette dialog.
     */
    void showCustomSkinDialog();
    /*!
     * Change index to custom skin dialog.
     */
    void showCustomSkin(const QString &path);
    /*!
     * Set current bg when user click the remote bg update.
     */
    void itemUserClicked(QListWidgetItem *item);
    /*!
     * User click the current local item.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void addThemeListWidgetItem();
    /*!
     * Add all theme into list.
     */
    void cpoyFileFromLocal(const QString &path);
    /*!
     * Copy custom file to local themes dir path.
     */

    Ui::MusicBackgroundSkinDialog *ui;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
