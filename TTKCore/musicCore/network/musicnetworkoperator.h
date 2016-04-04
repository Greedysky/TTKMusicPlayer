#ifndef MUSICNETWORKOPERATOR_H
#define MUSICNETWORKOPERATOR_H

#include "musicglobaldefine.h"

/*! @brief The class of music network operator.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkOperator : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkOperator(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkOperator();

    void startToOperator();
    /*!
     * Start to get current ip operator from net.
     */

Q_SIGNALS:
    void getNetworkOperatorFinished(const QString &name);
    /*!
     * Get network operator is finished.
     */

public Q_SLOTS:
    void downLoadFinished();
    /*!
     * Download data from ip net finished.
     */

};

#endif // MUSICNETWORKOPERATOR_H
