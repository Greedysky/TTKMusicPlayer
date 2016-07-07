#ifndef QNMAINWINDOW_H
#define QNMAINWINDOW_H

#include <QMainWindow>

class QAction;
class QNetworkAccessManager;
class QNSimpleUploadData;
class QNSimpleListData;

class QNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QNMainWindow(QWidget *parent = 0);
    ~QNMainWindow();

    QNetworkAccessManager *networkManager;

public slots:
    void simpleUploadDataSlot();
    void rsStatSlot();

private:
    //upload menu
    QAction *simpleUploadDataAction;
    //resource management
    QAction *rsStatAction;

    //methods
    void createMenus();
    void createWidgets();

    //objects
    QNSimpleUploadData *simpleUploadData;
    QNSimpleListData *rsStatWidget;


};

#endif // QNMAINWINDOW_H
