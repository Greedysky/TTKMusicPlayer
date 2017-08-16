/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef VISUALRIPPLESFACTORY_H
#define VISUALRIPPLESFACTORY_H


#include <QObject>

#include <qmmp/visualfactory.h>
#include <qmmp/visual.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VisualRipplesFactory : public QObject, public VisualFactory
{
Q_OBJECT
Q_INTERFACES(VisualFactory)

public:
    const VisualProperties properties() const;
    Visual *create(QWidget *parent);

};


#endif
