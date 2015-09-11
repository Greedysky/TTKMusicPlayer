#ifndef MUSICBACKGROUNDSKINDIALOG_H
#define MUSICBACKGROUNDSKINDIALOG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundSkinDialog;
}

class MUSIC_WIDGET_EXPORT MusicBackgroundSkinDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicBackgroundSkinDialog(QWidget *parent = 0);
    virtual ~MusicBackgroundSkinDialog();

    void setCurrentBgTheme(const QString &theme, int alpha, int alphaR);
    void updateBackground();

public slots:
    void changeToMySkin();
    void changeToNetSkin();
    void showPaletteDialog();
    void showCustomSkinDialog();
    virtual int exec();

protected:
    void clearAllItems();
    void addThemeListWidgetItem();
    Ui::MusicBackgroundSkinDialog *ui;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
