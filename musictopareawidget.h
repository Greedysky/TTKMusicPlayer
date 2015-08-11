#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

#include <QWidget>
#include <QTimer>
#include "musiclibexportglobal.h"

class MusicUserWindow;
class MusicBackgroundSkinDialog;

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTopAreaWidget(QWidget *parent = 0);
    ~MusicTopAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void setParameters(const QString&, int);
    QString getBgSkin();
    int getBgSkinAlpha();

signals:
    void updateToolStyle();
    void updateArtPicture();

public slots:
    void musicShowSkinChangedWindow();
    void musicUserContextLogin();
    void musicBgTransparentChanged();
    void musicBgTransparentChanged(int);
    void musicBackgroundSkinChanged(const QString&);
    void musicBackgroundChanged();
    void musicBgThemeDownloadFinished();

protected:
    void drawWindowBackgroundRect();
    void drawWindowBackgroundRectString(const QString&);

    Ui::MusicApplication *m_ui;
    MusicUserWindow* m_msuicUserWindow;
    MusicBackgroundSkinDialog* m_musicbgskin;
    QString m_currentBgSkin;
    int m_alpha;
    QTimer m_pictureCarouselTimer;


};

#endif // MUSICTOPAREAWIDGET_H
