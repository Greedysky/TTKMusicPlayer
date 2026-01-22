#ifndef MUSICSCREENSAVERWIDGET_H
#define MUSICSCREENSAVERWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "ttkabstractresizeinterface.h"
#include "musictransitionanimationlabel.h"

class MusicDownloadQueueRequest;

/*! @brief The class of the screen saver hover item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicScreenSaverHoverItem : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
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
     * Display item.
     */
    void display(const QPoint &point);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void itemClicked(int index, bool status);

private Q_SLOTS:
    /*!
     * Switch button on or off.
     */
    void switchButtonState();

private:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override final;
    virtual void focusOutEvent(QFocusEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override final;

    int m_index;
    QString m_path;
    QLabel *m_parent;
    QPushButton *m_enableButton;
};



/*! @brief The class of the screen saver list item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicScreenSaverListItem : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicScreenSaverListItem(QObject *object, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicScreenSaverListItem();

    /*!
     * Set item file name.
     */
    void setFilePath(const QString &path);
    /*!
     * Set item status.
     */
    void setStatus(int index, bool status);
    /*!
     * Set item visible or not.
     */
    void setHoverVisible(bool v);

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;

    MusicScreenSaverHoverItem *m_hoverItem;

};



/*! @brief The class of the screen saver list widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicScreenSaverListWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicScreenSaverListWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicScreenSaverListWidget();

    /*!
     * Create cell item by input data.
     */
    void addCellItem(QObject *object, const QString &path, int index, bool status);
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

    QGridLayout *m_gridLayout;
    QList<MusicScreenSaverListItem*> m_items;

};



/*! @brief The class of the screen saver widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicScreenSaverWidget : public QWidget, public TTKAbstractResizeInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicScreenSaverWidget(QWidget *parent = nullptr);

    /*!
     * Apply settings parameters.
     */
    void applyParameter();
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

public:
    /*!
     * Parse settings parameters.
     */
    static QVector<bool> parseSettingParameter();

private Q_SLOTS:
    /*!
     * Input data changed
     */
    void inputDataChanged();
    /*!
     * switch button on or off.
     */
    void switchButtonState();
    /*!
     * Download data from net finished.
     */
    void downloadDataFinished(const QString &bytes);
    /*!
     * Current item has clicked.
     */
    void currentItemClicked(int index, bool status);

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();

    bool m_currentState;
    QLineEdit *m_inputEdit;
    QPushButton *m_caseButton;
    MusicDownloadQueueRequest *m_networkRequest;
    MusicScreenSaverListWidget *m_backgroundList;
};



/*! @brief The class of the screen saver background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicScreenSaverBackgroundWidget : public MusicTransitionAnimationLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicScreenSaverBackgroundWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicScreenSaverBackgroundWidget();

    /*!
     * Execute module.
     */
    void execute();

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
    virtual bool eventFilter(QObject *watched, QEvent *event) override final;

private:
    bool m_state;
    bool m_isRunning;
    QTimer *m_runningTimer;
    QTimer *m_backgroundTimer;

};

#endif // MUSICSCREENSAVERWIDGET_H
