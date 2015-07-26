#ifndef MUSICLOCALSONGSEARCH_H
#define MUSICLOCALSONGSEARCH_H

#include <QtWidgets>
#include "musiclibexportglobal.h"

class MusicLocalSongSearchEdit;

class MUSIC_WIDGET_EXPORT MusicLocalSongSearch : public QDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearch(QWidget *parent = 0);
    virtual ~MusicLocalSongSearch();

    QString getSearchedText() const;
    void clearSearchedText();

private:
    QLabel* m_showIcon;
    MusicLocalSongSearchEdit* m_searchLine;
    QToolButton* m_closeButton;

};

#endif // MUSICLOCALSONGSEARCH_H
