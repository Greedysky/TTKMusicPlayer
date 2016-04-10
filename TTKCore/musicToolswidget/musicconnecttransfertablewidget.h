#ifndef MUSICCONNECTTRANSFERTABLEWIDGET_H
#define MUSICCONNECTTRANSFERTABLEWIDGET_H

#include "musicitemdelegate.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the transfer file list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicConnectTransferTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicConnectTransferTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicConnectTransferTableWidget();

public Q_SLOTS:
    virtual void listCellClicked(int row, int col) override;
    /*!
     * Table widget list cell click.
     */

protected:
    MusicQueryTableDelegate *m_checkBoxDelegate;

};

#endif // MUSICCONNECTTRANSFERTABLEWIDGET_H
