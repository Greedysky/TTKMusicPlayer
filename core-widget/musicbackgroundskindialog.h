#ifndef MUSICBACKGROUNDSKINDIALOG_H
#define MUSICBACKGROUNDSKINDIALOG_H

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicBackgroundSkinDialog;
}

class MUSIC_WIDGET_EXPORT MusicBackgroundSkinDialog : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicBackgroundSkinDialog(QWidget *parent = 0);
    virtual ~MusicBackgroundSkinDialog();

    void clearAllItems();
    void addThemeListWidgetItem();
    void setCurrentBgTheme(const QString& theme,int alpha);

public slots:
    void changeToMySkin();
    void changeToNetSkin();
    void showPaletteDialog();
    void showCustomSkinDialog();

protected:
    Ui::MusicBackgroundSkinDialog *ui;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
