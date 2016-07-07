#include "QNMainWindow.h"
#include "QNSimpleUploadData.h"
#include "QNSimpleListData.h"
#include "QNConf.h"

#include <QAction>
#include <QMenuBar>
#include <QNetworkAccessManager>

QNMainWindow::QNMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
   setWindowTitle(tr("Qiniu Qt SDK & Tools"));
   setUnifiedTitleAndToolBarOnMac(true);
   networkManager=new QNetworkAccessManager(this);

   createMenus();
   createWidgets();
   this->setMinimumWidth(500);
   this->setMinimumHeight(200);
}

void QNMainWindow::createMenus()
{
    //upload menu
    simpleUploadDataAction=new QAction(tr("Simple Upload Data"),this);
    QMenu *uploadMenu=menuBar()->addMenu(tr("Upload"));
    uploadMenu->addAction(simpleUploadDataAction);
    connect(simpleUploadDataAction,SIGNAL(triggered()),this,SLOT(simpleUploadDataSlot()));

    //resource management
    rsStatAction=new QAction(tr("Stat"),this);
    QMenu *statMenu=menuBar()->addMenu(tr("statMenu"));
    statMenu->addAction(rsStatAction);
    connect(rsStatAction,SIGNAL(triggered()),this,SLOT(rsStatSlot()));
}

void QNMainWindow::createWidgets()
{
    //init
    simpleUploadData=0;
    rsStatWidget=0;
    QNConf::ACCESS_KEY="L2GGQ-ttIlTScXVtXOdwPF2ftQAEiVK1qor5KCu3";
    QNConf::SECRET_KEY="FXiQ8EWibo-9tIlWRS3UAJOqv94pM1QViU2Gw25y";
}

QNMainWindow::~QNMainWindow()
{

}

void QNMainWindow::simpleUploadDataSlot()
{
    if (simpleUploadData==0)
    {
        simpleUploadData=new QNSimpleUploadData(networkManager);
        QByteArray data="sdffffffffffffffffffff";
        QString bucket="store";
        QString key="dd.t";
        simpleUploadData->uploadDataToServer(data, bucket, key, key);

    }
}

void QNMainWindow::rsStatSlot()
{
    if(rsStatWidget==0)
    {
        rsStatWidget=new QNSimpleListData(networkManager);
        rsStatWidget->listDataToServer("store");
    }
}
