#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSlider;
class MusicPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setVolume(int value);
    void setTimeValue(qint64 value);
    void setSeek(int value);

private:
    Ui::MainWindow *ui;
    MusicPlayer *m_sound;
    QList<QSlider*> m_sliders;


};

#endif
