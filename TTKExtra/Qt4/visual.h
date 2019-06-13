/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef VISUAL_H
#define VISUAL_H

#include <QMutex>
#include <QStringList>
#include <QWidget>
#include <QHash>
#include <stddef.h>
#include "qmmp_export.h"

#define QMMP_VISUAL_INTERVAL  40
#define QMMP_VISUAL_NODE_SIZE 512 //samples

class VisualFactory;
class VisualBuffer;

/*! @brief The Visual class provides the base interface class of visualizations.
 *  @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Visual : public QWidget
{
    Q_OBJECT
public:
    /*!
    * Object contsructor.
    * @param parent Parent object.
    * @param f Widget flags.
    */
    Visual(QWidget *parent, Qt::WindowFlags f = 0);
    /*!
     * Destructor.
     */
    virtual ~Visual();
    /*!
    * Returns a list of visual factories.
    */
    static QList<VisualFactory*> factories();
    /*!
     * Returns plugin file path.
     * @param factory Visual plugin factory.
     */
    static QString file(VisualFactory *factory);
    /*!
     * Sets whether the visual plugin is enabled.
     * @param factory Visual plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(VisualFactory* factory, bool enable = true);
    /*!
     * Returns \b true if visual plugin is enabled, otherwise returns \b false
     * @param factory Effect plugin factory.
     */
    static bool isEnabled(VisualFactory* factory);
    /*!
     * Adds external visualization \b visual.
     */
    static void add(Visual*visual);
    /*!
     * Removes external visualization \b visual.
     */
    static void remove(Visual*);
    /*!
     * Prepares visual plugins for usage.
     * @param parent Parent widget.
     * @param receiver Receiver object.
     * @param member A slot to receive changes of active visualizations list.
     */
    static void initialize(QWidget *parent, QObject *receiver = 0, const char *member = 0);
    /*!
     * Returns a pointer to a list of created visual objects.
     */
    static QList<Visual *> *visuals();
    /*!
     * Adds data for visualization.
     * @param pcm Audio data.
     * @param samples Number of samples.
     * @param channels Number of channels.
     * @param ts Elapsed time (in milliseconds).
     * @param delay Audio output delay.
     */
    static void addAudio(float *pcm, int samples, int channels, qint64 ts, qint64 delay);
    /*!
     * Clears visualization buffer.
     */
    static void clearBuffer();

public slots:
    /*!
     * Starts visualization.
     */
    virtual void start() = 0;
    /*!
     * Stops visualization.
     */
    virtual void stop() = 0;

signals:
    /*!
     * Emitted when visual widget is closed by user.
     */
    void closedByUser();

protected:
    /*!
     * QWidget's close event. Reimplementation should call base function.
     * @param event QCloseEvent insatance.
     */
    virtual void closeEvent (QCloseEvent *event);
    /*!
     * Takes visualization data. Caller should allocate \b QMMP_VISUAL_NODE_SIZE
     * bytes for each channel. If buffer for right channel is not specified,
     * this function will average data from left and right channels.
     * @param left Left channel buffer.
     * @param right Right channel buffer.
     */
    bool takeData(float *left, float *right = 0);
    /*!
     * Take visual maxed value range.
     */
    float takeMaxRange() const;

private:
    static QList<VisualFactory*> *m_factories;
    static QHash <VisualFactory*, QString> *m_files;
    static void checkFactories();
    static QList<Visual*>  m_visuals;
    static QHash<VisualFactory*, Visual*> m_vis_map; //internal visualization
    static QWidget *m_parentWidget;
    static QObject *m_receiver;
    static const char *m_member;
    static VisualBuffer m_buffer;
};

#endif
