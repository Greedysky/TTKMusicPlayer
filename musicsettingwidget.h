#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

#include <QDialog>
#include <QTabWidget>
#include "core/musicmovedialogabstract.h"
#include "core/musicobject.h"
#include <QMouseEvent>
#include <QSettings>

namespace Ui {
class MusicSettingWidget;
}

class MUSIC_EXPORT MusicSettingWidget : public MusicMoveDialogAbstract
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
    void defaultLrcColorChanged(int);
    void inlineLrcTransChanged(int);
    void showInlineLrcDemo(int);
    void resetInlineParameter();

    void desktopFgChanged();
    void desktopBgChanged();
    void defaultDesktopLrcColorChanged(int);
    void desktopLrcTransChanged(int);
    void showDesktopLrcDemo(int);
    void resetDesktopParameter();

protected:
    void initInlineLrcWidget();
    void initDesktopLrcWidget();
    void lcrColorValue(Type key, QString type, QPushButton *obj);
    void lrcColorByDefault(Type key, int index);
    void lrcTransparentValue(Type key, int index);
    QSettings m_musicSettingParameter;

    Ui::MusicSettingWidget *ui;
    QColor m_lrcSelectedFg;
    QColor m_lrcSelectedBg;
    QColor m_DlrcSelectedFg;
    QColor m_DlrcSelectedBg;

};

#endif // MUSICSETTINGWIDGET_H
