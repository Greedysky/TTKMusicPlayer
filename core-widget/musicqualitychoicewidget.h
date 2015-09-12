#ifndef MUSICQUALITYCHOICEWIDGET_H
#define MUSICQUALITYCHOICEWIDGET_H

#include <QToolButton>
#include "musicabstracttablewidget.h"

class MUSIC_WIDGET_EXPORT MusicQualityChoiceTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceTableWidget(QWidget *parent = 0);
    virtual ~MusicQualityChoiceTableWidget();

public slots:
    virtual void listCellClicked(int row, int column);
    void createItems();

};


class MUSIC_WIDGET_EXPORT MusicQualityChoiceWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceWidget(QWidget *parent = 0);
    ~MusicQualityChoiceWidget();

signals:
public slots:

protected:
    void initWidget();

};

#endif // MUSICQUALITYCHOICEWIDGET_H
