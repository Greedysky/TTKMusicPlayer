#ifndef MUSICNETWORKOPERATOR_H
#define MUSICNETWORKOPERATOR_H

#include "musicglobaldefine.h"

#define IP_DOWNLOAD "ip_tmp"
const QString IP_CHECK_URL = "http://1212.ip138.com/ic.asp";

/*! @brief The class of downloading art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkOperator : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkOperator(QObject *parent = 0);
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
