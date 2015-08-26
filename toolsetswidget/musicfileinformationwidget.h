#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include "musicmovedialogabstract.h"

namespace Ui {
class MusicFileInformationWidget;
}

class MUSIC_WIDGET_EXPORT MusicFileInformationWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicFileInformationWidget(QWidget *parent = 0);
    ~MusicFileInformationWidget();

    void setFileInformation(const QString &name);

signals:
public slots:
    void musicOpenFileDir();

protected:
    QString transSizeByte(int size);
    Ui::MusicFileInformationWidget *ui;
    QString m_path;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
