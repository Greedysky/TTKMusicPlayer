#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

#include <QWidget>
#include "../core/musictablewidgetabstract.h"

class QPushButton;

class MUSIC_EXPORT MusicLocalSongSearchPopTableWidget :
            public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = 0);
    ~MusicLocalSongSearchPopTableWidget();

    void clearAllItems();
    void createItems(int , const QString& , const QString& );

public slots:
    virtual void listCellClicked(int, int){}

};


class MUSIC_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = 0);
    ~MusicLocalSongSearchPopWidget();

    void createItems();
    QString utcTimeToLocal(const QString& time);

signals:
    void setText(const QString&);

public slots:
    void clearButtonClicked();
    void cellDoubleClicked(int,int);

protected:
    virtual void leaveEvent(QEvent *event);
    MusicLocalSongSearchPopTableWidget* m_popTableWidget;
    QPushButton *m_clearButton;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
