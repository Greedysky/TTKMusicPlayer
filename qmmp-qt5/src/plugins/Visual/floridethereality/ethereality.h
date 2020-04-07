/* =================================================
 * This file is part of the TTK qmmp plugin project
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

#ifndef ETHEREALITY_H
#define ETHEREALITY_H

#include <QTimer>
#include <QWidget>
#include <QDateTime>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class Ethereality : public QWidget
{
    Q_OBJECT
public:
    explicit Ethereality(QWidget *parent = nullptr);

    void start();
    void stop();

    bool isRunning() const;

private Q_SLOTS:
    void timeout();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    void loadFile();

private:
    QTimer *m_timer;
    QStringList m_word_list;
    QStringList m_word;
};

#endif
