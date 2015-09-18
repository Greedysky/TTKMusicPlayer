#ifndef MUSICITEMDELEGATE_H
#define MUSICITEMDELEGATE_H

#include <QItemDelegate>
#include "musiclibexportglobal.h"

class QCheckBox;
class QProgressBar;

class MUSIC_WIDGET_EXPORT MusicCheckBoxDelegate : public QItemDelegate
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


class MUSIC_WIDGET_EXPORT MusicQueryTableDelegate : public MusicCheckBoxDelegate
{
public:
    explicit MusicQueryTableDelegate(QObject* parent = 0);
    virtual ~MusicQueryTableDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};


class MUSIC_WIDGET_EXPORT MusicProgressBarDelegate : public QItemDelegate
{
public:
    explicit MusicProgressBarDelegate(QObject* parent = 0);
    ~MusicProgressBarDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
private:
    QProgressBar *m_progress;

};

#endif // MUSICITEMDELEGATE_H
