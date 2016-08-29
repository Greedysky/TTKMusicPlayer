#ifndef MUSICIDENTIFYSONGSWIDGET_H
#define MUSICIDENTIFYSONGSWIDGET_H

#include <QLabel>
#include <QPushButton>

class QMovie;
class QStackedWidget;

class MusicIdentifySongsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicIdentifySongsWidget(QWidget *parent = 0);
    ~MusicIdentifySongsWidget();

public Q_SLOTS:
    void detectedButtonClicked();

protected:
    void createDetectedWidget();
    void createDetectedSuccessedWidget();
    void createDetectedFailedWidget();

    QStackedWidget *m_mainWindow;
    QLabel *m_detectedLabel;
    QMovie *m_detectedMovie;
    QPushButton *m_detectedButton;

};

#endif // MUSICIDENTIFYSONGSWIDGET_H
