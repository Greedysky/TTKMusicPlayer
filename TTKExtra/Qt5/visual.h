/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

class Buffer;
class Decoder;
class Output;
class VisualFactory;

/*! @brief The Visual class provides the base interface class of visualizations.
 *  @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT Visual : public QWidget
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
     * Adds data for visualization.
     * Subclass should reimplement this function.
     * @param data Audio data.
     * @param size Size of audio data.
     * @param chan Number of channels.
     */
    virtual void add(unsigned char *data, qint64 size, int chan) = 0;
    /*!
     * Resets visual plugin buffers and widgets.
     * Subclass should reimplement this function.
     */
    virtual void clear() = 0;
    /*!
     * Returns mutex pointer.
     */
    QMutex *mutex();
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
     * Shows configuration dialog and updates settings automatically.
     * @param factory Visual plugin factory.
     * @param parent Parent widget.
     */


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

private:
    Decoder *m_decoder;
    Output *m_output;
    QMutex m_mutex;

    static QList<VisualFactory*> *m_factories;
    static QHash <VisualFactory*, QString> *m_files;
    static void checkFactories();
    static QList<Visual*>  m_visuals;
    static QHash<VisualFactory*, Visual*> m_vis_map; //internal visualization
    static QWidget *m_parentWidget;
    static QObject *m_receiver;
    static const char *m_member;
};

#endif
