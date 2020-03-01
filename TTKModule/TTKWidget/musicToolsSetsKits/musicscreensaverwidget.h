#ifndef MUSICSCREENSAVERWIDGET_H
#define MUSICSCREENSAVERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "musictransitionanimationlabel.h"

class MusicDownloadQueueCache;


/*! @brief The class of the screen saver hover item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicScreenSaverHoverItem : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverHoverItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverHoverItem(QLabel *parent = nullptr);

    /*!
     * Set item file name.
     */
    void setFilePath(const QString &path);

    /*!
     * Set item status.
     */
    void setStatus(int index, bool status);

    /*!
     * show item.
     */
    void showItem(const QPoint &point);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void itemClicked(int index, bool status);

private Q_SLOTS:
    /*!
     * Case button on and off.
     */
    void caseButtonOnAndOff();

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    int m_index;
    QString m_path;
    QLabel *m_parent;
    QPushButton *m_enableButton;
};



/*! @brief The class of the screen saver list item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicScreenSaverListItem : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverListItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverListItem(QObject *object, QWidget *parent = nullptr);

    virtual ~MusicScreenSaverListItem();

    /*!
     * Set item file name.
     */
    void setFilePath(const QString &path);

    /*!
     * Set item status.
     */
    void setStatus(int index, bool status);

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;

    MusicScreenSaverHoverItem *m_hoverItem;
};



/*! @brief The class of the screen saver list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicScreenSaverListWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverListWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverListWidget(QWidget *parent = nullptr);

    virtual ~MusicScreenSaverListWidget();

    /*!
     * Create item by name and path.
     */
    void createItem(QObject *object, const QString &path, int index, bool status);

    /*!
     * Resize window bound by widget resize called.
     */
    virtual void resizeWindow();

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;

    QGridLayout *m_gridLayout;
    QList<MusicScreenSaverListItem*> m_items;

};



/*! @brief The class of the screen saver widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicScreenSaverWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverWidget(QWidget *parent = nullptr);

    virtual ~MusicScreenSaverWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();
    /*!
     * Parse settings parameters.
     */
    static QVector<bool> parseSettingParameter();

    /*!
     * Resize window bound by widget resize called.
     */
    virtual void resizeWindow();

private Q_SLOTS:
    /*!
     * Input data changed
     */
    void inputDataChanged();
    /*!
     * Case button on and off.
     */
    void caseButtonOnAndOff();
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const QString &data);
    /*!
     * Current item has clicked.
     */
    void itemHasClicked(int index, bool status);

private:
    /*!
     * Init widget and parameters.
     */
    void initialize();

    bool m_currentState;
    QLineEdit *m_inputEdit;
    QPushButton *m_caseButton;
    MusicDownloadQueueCache *m_downloadQueue;
    MusicScreenSaverListWidget *m_backgroundList;
};



/*! @brief The class of the screen saver background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicScreenSaverBackgroundWidget : public MusicTransitionAnimationLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverBackgroundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverBackgroundWidget(QWidget *parent = nullptr);

    virtual ~MusicScreenSaverBackgroundWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();

private Q_SLOTS:
    /*!
     * Screen saver time out.
     */
    void runningTimeout();
    /*!
     * Screen saver time out.
     */
    void backgroundTimeout();

private:
    /*!
     * Override the widget event.
     */
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_state;
    bool m_isRunning;
    QTimer *m_runningTimer;
    QTimer *m_backgroundTimer;

};

#endif // MUSICSCREENSAVERWIDGET_H
