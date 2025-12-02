#ifndef MINIBLINK_H
#define MINIBLINK_H

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

#include <QWidget>

namespace wke {
class CWebView;
}

class Miniblink : public QWidget
{
    Q_OBJECT
public:
    explicit Miniblink(QWidget *parent = nullptr);
    ~Miniblink();

    static bool initialize();

    void load(const QString& url);
    void setHtml(const QString& html);

    void setUrl(const QString &url);
    QString url() const;

    void finish(bool ok);

Q_SIGNALS:
    void loadFinished(bool ok);

public Q_SLOTS:
    void stop();
    void back();
    void forward();
    void reload();

protected:
    virtual void resizeEvent(QResizeEvent *event) override final;

private:
    static int m_ref;
    wke::CWebView* m_webView;
};

#endif // MINIBLINK_H
