#ifndef MUSICMESSAGEBOX_H
#define MUSICMESSAGEBOX_H

#include <QWidget>
#include "musicmovedialogabstract.h"

namespace Ui {
class MusicMessageBox;
}

class MUSIC_WIDGET_EXPORT MusicMessageBox : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicMessageBox(QWidget *parent = 0);
    virtual ~MusicMessageBox();

    void setText(const QString &text) const;

public slots:
    void buttonClicked(int index);
    virtual int exec();

protected:
    Ui::MusicMessageBox *ui;
    int m_status;

};

#endif // MUSICMESSAGEBOX_H
