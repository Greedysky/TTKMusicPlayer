#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include "musiclibexportglobal.h"

class MusicSystemTrayMenu;

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBottomAreaWidget(QWidget *parent = 0);
    ~MusicBottomAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void setDestopLrcVisible(const QString &status);
    void showPlayStatus(bool status);
    void setLabelText(const QString &name);
    void setSystemCloseConfig(const QString &status);
    void setSystemCloseConfig(bool status)
            { m_systemCloseConfig = status;}
    bool getSystemCloseConfig()
            { return m_systemCloseConfig;}
    bool systemTrayIsVisible()
            { return m_systemTray->isVisible();}
    void showMessage(const QString &title, const QString &text);

signals:
    void setShowDesktopLrc(bool show);
    void setWindowLockedChanged();

public slots:
    void lockDesktopLrc(bool lock);
    void desktopLrcClosed();
    void iconActivated(QSystemTrayIcon::ActivationReason);

protected:
    void createToolPopupMenu();
    void createSystemTrayIcon();
    void createMenuActions();

    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    QMenu m_toolPopupMenu;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    bool m_systemCloseConfig;

};

#endif // MUSICBOTTOMAREAWIDGET_H
