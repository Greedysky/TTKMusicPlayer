#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include "../core/musicmovedialogabstract.h"

namespace Ui {
class MusicFileInformationWidget;
}

class MUSIC_EXPORT MusicFileInformationWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicFileInformationWidget(QWidget *parent = 0);
    ~MusicFileInformationWidget();

    void setFileInformation(const QString&);

signals:
public slots:
    void musicOpenFileDir();

protected:
    Ui::MusicFileInformationWidget *ui;
    QString m_path;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
