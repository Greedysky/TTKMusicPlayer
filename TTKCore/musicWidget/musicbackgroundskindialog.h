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

class MusicBackgroundListWidget;

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

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setCurrentBgTheme(const QString &theme, int alpha, int listAlpha);
    /*!
     * Select current item by name\alpha\listAlpha when the widget show.
     */
    void updateBackground(const QString &text);
    /*!
     * Update this bg when user change the current bg.
     */
    int getListBgSkinAlpha() const;
    /*!
     * Get kist bg skin alpha.
     */

    void setSkinTransToolText(int value);
    /*!
     * Set skin transparent tool text.
     */
    void setListTransToolText(int value);
    /*!
     * Set list transparent tool text.
     */

Q_SIGNALS:
    void currentTextChanged(const QString &text);
    /*!
     * Update app bg when user change the current bg emit.
     */
    void currentColorChanged(const QString &path);
    /*!
     * Update app bg when user change the current bg emit.
     */

public Q_SLOTS:
    void showPaletteDialog();
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog(const QString &path);
    /*!
     * Change index to palette dialog.
     */
    void showCustomSkinDialog();
    /*!
     * Change index to custom skin dialog.
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

    Ui::MusicBackgroundSkinDialog *m_ui;
    MusicBackgroundListWidget *m_backgroundList;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
