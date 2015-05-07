#ifndef MUSICLOCALSONGSMANAGERWIDGET_H
#define MUSICLOCALSONGSMANAGERWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include "../core/musicobject.h"
#include "../core/musicmovedialogabstract.h"

class QMovie;
class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

class MUSIC_EXPORT MusicLocalSongsManagerWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongsManagerWidget(QWidget *parent = 0);
    ~MusicLocalSongsManagerWidget();

signals:
    void addSongToPlay(const QStringList&);

public slots:
    void selectedAllItems(bool);
    void auditionButtonClick();
    void addButtonClick();
    void itemCellOnClick(int row, int col);
    void itemDoubleClicked(int row, int);
    void setSongNamePath(const QStringList&,const QStringList&);
    void filterIndexChanged();
    void musicSearchIndexChanged(int,int);

    void setShowlistButton();
    void setShowPathButton();

protected:
    Ui::MusicLocalSongsManagerWidget *ui;
    void clearAllItems();
    void addAllItems(const QStringList&,const QStringList&);
    void addLoaclSongList();
    void addDrivesList();
    void itemsSelected();

    QStringList m_filename;
    QStringList m_fileDir;
    MusicLocalSongsManagerThread* m_thread;
    QMap<int,QList<int> > m_searchfileListCache;
    int m_currentIndex;
    QMovie* m_movie;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
