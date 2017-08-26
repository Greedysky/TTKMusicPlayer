#ifndef KUGOUOBJECT_H
#define KUGOUOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
///PushButton
//////////////////////////////////////////////////////
    const QString MPushButtonStyle01 = " \
                    QPushButton{ border:none; color:rgb(135, 135, 135); font-size:14px;} \
                    QPushButton:hover{ color:rgb(104, 169, 236);} \
                    QPushButton:checked{ color:rgb(40, 143, 231);}";

    const QString MPushButtonStyle02 = " \
                    QPushButton{ color:rgb(104, 169, 236);}";

///Widget
//////////////////////////////////////////////////////
    const QString MWidgetStyle01 = " \
                    QWidget{ background:white;}";

}

#endif //KUGOUOBJECT_H
