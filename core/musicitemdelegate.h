#ifndef MUSICITEMDELEGATE_H
#define MUSICITEMDELEGATE_H

#include <QItemDelegate>

class QCheckBox;

class MusicCheckBoxDelegate : public QItemDelegate
{
public:
    explicit MusicCheckBoxDelegate(QObject* parent = 0);
    virtual ~MusicCheckBoxDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
protected:
    QCheckBox *m_checkBox;

};


class MusicQueryTableDelegate : public MusicCheckBoxDelegate
{
public:
    explicit MusicQueryTableDelegate(QObject* parent = 0);
    virtual ~MusicQueryTableDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};

#endif // MUSICITEMDELEGATE_H
