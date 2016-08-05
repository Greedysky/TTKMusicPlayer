#ifndef MUSICAUTOTEST_H
#define MUSICAUTOTEST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTest>
#include <QDebug>
#include <QSharedPointer>

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

template <class T>
class MusicTest
{
public:
    QSharedPointer<T> m_child;

    MusicTest(const QString& name) : m_child(new T)
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
        QCoreApplication::setOrganizationName("TTKMusicPlayer"); \
        QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com"); \
        QCoreApplication::setApplicationName("TTKMusicPlayer"); \
        return MusicAutoTest::run(argc, argv); \
    }

#endif // MUSICAUTOTEST_H
