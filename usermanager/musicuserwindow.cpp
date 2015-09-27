#include "musicuserwindow.h"
#include "musicuiobject.h"
#include "ui_musicuserwindow.h"
#include "musicuserdialog.h"
#include "musicusermodel.h"
#include "musicusermanager.h"
#include "musicmessagebox.h"

#include <QTimer>

MusicUserWindow::MusicUserWindow(QWidget *parent)
   : QStackedWidget(parent), ui(new Ui::MusicUserWindow)
{
    ui->setupUi(this);
    ui->userLogin->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    ui->userName->setStyleSheet(MusicUIObject::MPushButtonStyle11);
    ui->userLogin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->userName->setCursor(QCursor(Qt::PointingHandCursor));

    ui->userIcon->setStyleSheet(MusicUIObject::MCustomStyle28);
    connectDatabase();

    m_userManager = new MusicUserManager(this);
    connect(ui->userLogin,SIGNAL(clicked()),SLOT(musicUserLogin()));
    connect(ui->userName,SIGNAL(clicked()),m_userManager,SLOT(exec()));
    connect(m_userManager,SIGNAL(userStateChanged(QString)),SLOT(userStateChanged(QString)));
}

MusicUserWindow::~MusicUserWindow()
{
    disConnectDatabase();
    delete m_userManager;
    delete ui;
}

bool MusicUserWindow::disConnectDatabase()
{
    QString connectionName;
    {
        QSqlDatabase data = QSqlDatabase::database("user-data");
        connectionName = data.connectionName();
        if( data.isValid() )
        {
            data.close();
        }
        else
        {
            return false;
        }
    }
    QSqlDatabase::removeDatabase( connectionName );
    return true;
}

bool MusicUserWindow::connectDatabase()
{
    try
    {
        QSqlDatabase data;
        if(QSqlDatabase::contains("user-data"))
        {
            data = QSqlDatabase::database("user-data");
        }
        else
        {
            data = QSqlDatabase::addDatabase(DATABASETYPE, "user-data");
        }
        data.setDatabaseName(DARABASEPATH);
        if( !data.isDriverAvailable(DATABASETYPE) )
        {
            throw QString("The driver name is not available!");
        }
        if( !data.isValid() )
        {
            throw QString("The database has not a vaild driver!");
        }
        if (!data.isOpen() && !data.open() )
        {
            throw QString("Can not open database connection!");
        }
        if(!data.tables().contains("MusicUser"))
        {
            QSqlQuery query(data);
            QString musicUserSql = QString("create table MusicUser (USERID vchar(%1) PRIMARY KEY,"
                    "PASSWD vchar(%2),EMAIL vchar(%3),USERNAME vchar(%4),LOGINTIME vchar(%5))")
                    .arg(USERID).arg(PASSWD).arg(EMAIL).arg(USERNAME).arg(LOGINTIME);
            query.exec(musicUserSql);
        }
    }
    catch(QString exception)
    {
        MusicMessageBox message;
        message.setText( exception );
        message.exec();
        return false;
    }
    return true;
}

void MusicUserWindow::userStateChanged(const QString &name)
{
    ui->userName->setText(QFontMetrics(font())
                .elidedText(name, Qt::ElideRight,44));

    if(currentIndex() == 1)
    {
        setCurrentIndex(0);
    }
    else
    {
        m_userManager->setUserName(name);
        setCurrentIndex(1);
    }
}

void MusicUserWindow::musicUserLogin()
{
    MusicUserDialog dialog;
    connect(&dialog, SIGNAL(userLoginSuccess(QString)), SLOT(userStateChanged(QString)));
    dialog.exec();
}

void MusicUserWindow::musicUserContextLogin()
{
    if(currentIndex() == 1)
    {
        setCurrentIndex(0);
    }
    QTimer::singleShot(1, this, SLOT(musicUserLogin()));
}
