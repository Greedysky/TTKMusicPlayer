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

class MUSIC_WIDGET_EXPORT MusicBackgroundSkinDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicBackgroundSkinDialog(QWidget *parent = 0);
    virtual ~MusicBackgroundSkinDialog();

    void setCurrentBgTheme(const QString &theme, int alpha, int listAlpha);
    void updateBackground();
    int getListBgSkinAlpha() const;

Q_SIGNALS:
    void currentTextChanged(const QString &text);

public Q_SLOTS:
    void changeToMySkin();
    void changeToNetSkin();
    void showPaletteDialog();
    void showCustomSkinDialog();
    void showCustomSkin(const QString &path);
    void itemUserClicked(QListWidgetItem *item);
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void addThemeListWidgetItem();
    void cpoyFileFromLocal(const QString &path);

    Ui::MusicBackgroundSkinDialog *ui;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
