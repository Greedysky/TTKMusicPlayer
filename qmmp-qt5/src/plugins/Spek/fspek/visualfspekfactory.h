/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef VISUALFSPEKFACTORY_H
#define VISUALFSPEKFACTORY_H

#include <qmmp/spekfactory.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VisualFSpekFactory : public QObject, public SpekFactory
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.SpekFactoryInterface.1.0")
Q_INTERFACES(SpekFactory)

public:
    const SpekProperties properties() const;
    Spek *create(QWidget *parent);

};


#endif
