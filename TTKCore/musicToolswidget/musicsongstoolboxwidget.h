#ifndef MUSICSONGSTOOLBOXWIDGET_H
#define MUSICSONGSTOOLBOXWIDGET_H

#include <QLabel>
#include <QBoxLayout>

class MusicSongsToolBoxTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxTopWidget(int index, QWidget *parent = 0);

signals:
    void mousePressAt(int index);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
//    void paintEvent(QPaintEvent *event);
    int m_index;
};


class MusicSongsToolBoxWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent = 0);
    ~MusicSongsToolBoxWidgetItem();

    void addItem(QWidget *item);
    void removeItem(QWidget *item);

    void setTitle(const QString &text);
    QString getTitle() const;

    void setItemHide(bool hide);
    bool itemHide() const;

signals:
    void addNewItem();
    /*!
     * Add new play list item.
     */
    void deleteItem(int index);
    /*!
     * Delete selected play list item.
     */
    void changItemName(int index, const QString &name);
    /*!
     * Open rename selected play list item widget.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    QLabel *m_labelIcon, *m_labelText;
    QVBoxLayout *m_layout;
    QList<QWidget*> m_itemList;

};


class MusicSongsToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxWidget(QWidget *parent = 0);
    ~MusicSongsToolBoxWidget();

    void addItem(QWidget *item, const QString &text);

    int currentIndex() const;

    void setItemText(int index, const QString &text);
    QString itemText(int index) const;

    int count() const;

signals:
public slots:
    void setCurrentIndex(int index);
    void mousePressAt(int index);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    int m_currentIndex;
    QVBoxLayout *m_layout;
    QList<MusicSongsToolBoxWidgetItem*> m_itemList;

};

#endif // MUSICSONGSTOOLBOXWIDGET_H
