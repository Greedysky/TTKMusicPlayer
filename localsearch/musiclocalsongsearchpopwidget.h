#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

#include <QWidget>
#include "musictablewidgetabstract.h"

class QPushButton;

class MUSIC_WIDGET_EXPORT MusicLocalSongSearchPopTableWidget :
            public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = 0);
    ~MusicLocalSongSearchPopTableWidget();

    void clearAllItems();
    void createItems(int index, const QString &name,
                     const QString &time);

public slots:
    virtual void listCellClicked(int, int){}

};


class MUSIC_WIDGET_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = 0);
    ~MusicLocalSongSearchPopWidget();

    void createItems();
    QString utcTimeToLocal(const QString &time);

signals:
    void setText(const QString &text);

public slots:
    void clearButtonClicked();
    void cellDoubleClicked(int row, int column);

protected:
    virtual void leaveEvent(QEvent *event);
    MusicLocalSongSearchPopTableWidget *m_popTableWidget;
    QPushButton *m_clearButton;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
