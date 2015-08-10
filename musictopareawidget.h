#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

#include <QWidget>

namespace Ui {
    class MusicApplication;
}

class MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTopAreaWidget(QWidget *parent = 0);

    void setupUi(Ui::MusicApplication* ui) { m_ui = ui;}

signals:
public slots:

protected:
    Ui::MusicApplication *m_ui;

};

#endif // MUSICTOPAREAWIDGET_H
