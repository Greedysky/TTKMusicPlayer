#ifndef MUSICBACKGROUNDSKINDIALOG_H
#define MUSICBACKGROUNDSKINDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundSkinDialog;
}

class MusicBackgroundListWidget;
class MusicBackgroundRemoteWidget;

/*! @brief The class of the local background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundSkinDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundSkinDialog(QWidget *parent = 0);

    virtual ~MusicBackgroundSkinDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set artist background picture by name.
     */
    static QPixmap setMBackground(QString &name);
    /*!
     * Theme valid check.
     */
    static bool themeValidCheck(QString &name, QString &path);
    /*!
     * Copy art file to local themes dir path.
     */
    static QString cpoyArtFileToLocal(const QString &path);
    /*!
     * Update art file theme.
     */
    void updateArtFileTheme(const QString &theme);

    /*!
     * Select current item by name\alpha\listAlpha when the widget show.
     */
    void setCurrentBgTheme(const QString &theme, int alpha, int listAlpha);
    /*!
     * Get list bg skin alpha.
     */
    int getBackgroundListAlpha() const;
    /*!
     * Get bg window skin enable.
     */
    bool getBackgroundTransparentEnable() const;

    /*!
     * Set skin transparent tool text.
     */
    void setSkinTransToolText(int value);
    /*!
     * Set list transparent tool text.
     */
    void setListTransToolText(int value);

Q_SIGNALS:
    /*!
     * Update app bg when user change the current bg emit.
     */
    void currentTextChanged(const QString &text);

public Q_SLOTS:
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog();
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog(const QString &path);
    /*!
     * Change index to custom skin dialog.
     */
    void showCustomSkinDialog();
    /*!
     * Background list widget changed.
     */
    void backgroundListWidgetChanged(int index);
    /*!
     * Background list widget item has clicked.
     */
    void backgroundListWidgetItemClicked(const QString &name);
    /*!
     * My background list widget item has clicked.
     */
    void myBackgroundListWidgetItemClicked(const QString &name);
    /*!
     * Remote background list widget item has clicked.
     */
    void remoteBackgroundListWidgetItemClicked(const QString &name);
    /*!
     * Update app bg when user change the current bg emit.
     */
    void currentColorChanged(const QString &path);
    /*!
     * Window transparent changed.
     */
    void windowTransparentChanged(bool state);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Background list widget item has clicked changed.
     */
    void listWidgetItemClicked(MusicBackgroundListWidget *item, const QString &name);
    /*!
     * Add all theme into list.
     */
    void addThemeListWidgetItem();
    /*!
     * Copy custom file to local themes dir path.
     */
    void cpoyFileFromLocal(const QString &path);
    /*!
     * Copy custom file index.
     */
    static int cpoyFileToLocalIndex();
    /*!
     * Copy custom file .
     */
    static int cpoyFileToLocal(const QString &path);

    int m_myThemeIndex;
    Ui::MusicBackgroundSkinDialog *m_ui;
    MusicBackgroundListWidget *m_backgroundList, *m_myBackgroundList;
    MusicBackgroundRemoteWidget *m_remoteBackgroundList;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
