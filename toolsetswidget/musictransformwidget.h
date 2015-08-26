#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicmovedialogabstract.h"

class QMovie;
class QProcess;

namespace Ui {
class MusicTransformWidget;
}

class MUSIC_WIDGET_EXPORT MusicTransformWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicTransformWidget(QWidget *parent = 0);
    ~MusicTransformWidget();

signals:

public slots:
    void initInputPath();
    void initOutputPath();
    void startTransform();
    void transformFinish(int index);
    void folderBoxChecked(bool check);

protected:
    QString getTransformSongName() const;
    Ui::MusicTransformWidget *ui;

    void initControlParameter() const;
    bool processTransform(const QString &para) const;
    void setCheckedControl(bool enable) const;

    QProcess *m_process;
    QMovie *m_movie;
    QStringList m_path;

};

#endif // MUSICTRANSFROMWIDGET_H
