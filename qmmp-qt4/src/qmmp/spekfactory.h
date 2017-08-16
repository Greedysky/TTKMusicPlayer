/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef SPEKFACTORY_H
#define SPEKFACTORY_H

#include <QWidget>

/*! @brief Helper class to spek widget.
 * @author Greedysky <greedysky@163.com>
 */
class Q_DECL_EXPORT Spek : public QWidget
{
public:
    /*!
     * Constructor.
     */
    explicit Spek(QWidget *parent = 0) : QWidget(parent) {}
    /*!
     * Destructor.
     */
    virtual ~Spek() {}
     /*!
     * Open spek file path.
     * Subclass should reimplement this function.
     */
    virtual void open(const QString &path) = 0;
    /*!
     * Start spek plugin.
     * Subclass should reimplement this function.
     */
    virtual void start() = 0;
    /*!
     * Stop spek plugin.
     * Subclass should reimplement this function.
     */
    virtual void stop() = 0;

};

/*! @brief Helper class to store spek plugin properies.
 * @author Greedysky <greedysky@163.com>
 */
class Q_DECL_EXPORT SpekProperties
{
public:
    /*!
     * Constructor
     */
    SpekProperties()
    {
        hasAbout = false;
        hasSettings = false;
    }
    QString name;      /*!< Spek plugin full name */
    QString shortName; /*!< Spek plugin short name for internal usage */
    bool hasAbout;     /*!< Should be \b true if plugin has about dialog, otherwise returns \b false */
    bool hasSettings;  /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};
/*! @brief Spek plugin interface (Spek factory).
 * @author Greedysky <greedysky@163.com>
 */
class Q_DECL_EXPORT SpekFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~SpekFactory() {}
     /*!
     * Returns Spek plugin properties.
     */
    virtual const SpekProperties properties() const = 0;
    /*!
     * Creates Spek provided by plugin.
     * @param parent Parent object.
     */
    virtual Spek *create(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(SpekFactory, "SpekFactory/1.0")

#endif
