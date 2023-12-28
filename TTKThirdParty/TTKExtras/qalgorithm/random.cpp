#include "random.h"
#include "ttkqtglobal.h"

#if TTK_QT_VERSION_CHECK(5,10,0)
#  include <QRandomGenerator>
#endif

namespace QAlgorithm
{
void initRandom()
{
#if !TTK_QT_VERSION_CHECK(5,10,0)
    qsrand(QDateTime::currentMSecsSinceEpoch());
#endif
}

int random(int value)
{
#if TTK_QT_VERSION_CHECK(5,10,0)
    return QRandomGenerator::global()->bounded(value);
#else
    return qrand() % value;
#endif
}
}
