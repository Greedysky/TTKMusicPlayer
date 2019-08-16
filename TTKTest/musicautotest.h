#ifndef MUSICAUTOTEST_H
#define MUSICAUTOTEST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QDebug>
#include <QtTest/QTest>
#include <QSharedPointer>
#include "musicobject.h"

namespace MusicAutoTest
{
    typedef QList<QObject*> TestList;

    inline TestList& testList()
    {
        static TestList list;
        return list;
    }

    inline bool findObject(QObject* object)
    {
        TestList& list = testList();
        if(list.contains(object))
        {
            return true;
        }
        foreach(QObject* test, list)
        {
            if(test->objectName() == object->objectName())
            {
                return true;
            }
        }
        return false;
    }

    inline void addTest(QObject* object)
    {
        TestList& list = testList();
        if(!findObject(object))
        {
            list.append(object);
        }
    }

    inline int run(int argc, char *argv[])
    {
        int ret = 0;
        foreach(QObject* test, testList())
        {
            ret += QTest::qExec(test, argc, argv);
        }
        return ret;
    }
}

template <typename T>
/*! @brief The class of the app test object.
 * @author Greedysky <greedysky@163.com>
 */
class MusicTest
{
public:
    QSharedPointer<T> m_child;

    MusicTest(const QString &name) : m_child(new T)
    {
        m_child->setObjectName(name);
        MusicAutoTest::addTest(m_child.data());
    }
    static QString getClassName()
    {
        return "MusicTest";
    }
};

#define DECLARE_TEST(className) static MusicTest<className> t(#className);

#define TEST_MAIN \
    int main(int argc, char *argv[]) \
    { \
        QApplication app(argc, argv); \
        QCoreApplication::setOrganizationName(APP_NAME); \
        QCoreApplication::setOrganizationDomain(APP_COME_NAME); \
        QCoreApplication::setApplicationName(APP_NAME); \
        return MusicAutoTest::run(argc, argv); \
    }

#endif // MUSICAUTOTEST_H
