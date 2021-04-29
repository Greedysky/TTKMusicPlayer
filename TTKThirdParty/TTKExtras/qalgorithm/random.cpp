#include "random.h"

#if TTK_QT_VERSION_CHECK(5,10,0)
#include <QRandomGenerator>
#endif

void QRandom::initRandom()
{
#if !TTK_QT_VERSION_CHECK(5,10,0)
    qsrand(QDateTime::currentMSecsSinceEpoch());
#endif
}

int QRandom::random(int value)
{
#if TTK_QT_VERSION_CHECK(5,10,0)
    return QRandomGenerator::global()->bounded(value);
#else
    return qrand() % value;
#endif
}
