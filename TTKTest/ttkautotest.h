#ifndef TTKAUTOTEST_H
#define TTKAUTOTEST_H

#include <QTest>
#include <QDebug>
#include <QObject>
#include <QSharedPointer>

namespace TTKAutoTest
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
class TTKTest
{
public:
    QSharedPointer<T> m_child;

    TTKTest(const QString& name) : m_child(new T)
    {
        m_child->setObjectName(name);
        TTKAutoTest::addTest(m_child.data());
    }
};

#define DECLARE_TEST(className) static TTKTest<className> t(#className);

#define TEST_MAIN \
    int main(int argc, char *argv[]) \
    { \
        QApplication app(argc, argv); \
        QCoreApplication::setOrganizationName("TTKMusicPlayer"); \
        QCoreApplication::setOrganizationDomain("TTKMusicPlayer.com"); \
        QCoreApplication::setApplicationName("TTKMusicPlayer"); \
        return TTKAutoTest::run(argc, argv); \
    }

#endif // TTKAUTOTEST_H
