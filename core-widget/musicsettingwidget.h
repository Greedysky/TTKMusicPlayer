#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

#include <QDialog>
#include <QTabWidget>
#include <QMouseEvent>
#include "musicmovedialogabstract.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

namespace Ui {
class MusicSettingWidget;
}

class MUSIC_WIDGET_EXPORT MusicSettingWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT

public:
    typedef enum Type{ Inline, Desktop} Type;
    explicit MusicSettingWidget(QWidget *parent = 0);
    virtual ~MusicSettingWidget();

    void initControllerParameter();

signals:
    void parameterSettingChanged();

public slots:
    void changeInfoWidget();
    void changeShotKeyWidget();
    void changeInlineLrcWidget();
    void changeDesktopLrcWidget();
    void commitTheResults();

    void inlineLrcFgChanged();
    void inlineLrcBgChanged();
    void defaultLrcColorChanged(int index);
    void inlineLrcTransChanged(int index);
    void showInlineLrcDemo(int index);
    void resetInlineParameter();

    void desktopFgChanged();
    void desktopBgChanged();
    void defaultDesktopLrcColorChanged(int index);
    void desktopLrcTransChanged(int index);
    void showDesktopLrcDemo(int index);
    void resetDesktopParameter();

protected:
    void initInlineLrcWidget();
    void initDesktopLrcWidget();
    void lcrColorValue(Type key, QString type, QPushButton *obj);
    void lrcColorByDefault(Type key, int index);
    void lrcTransparentValue(Type key, int index) const;

    Ui::MusicSettingWidget *ui;
    QColor m_lrcSelectedFg;
    QColor m_lrcSelectedBg;
    QColor m_DlrcSelectedFg;
    QColor m_DlrcSelectedBg;

};

#endif // MUSICSETTINGWIDGET_H
