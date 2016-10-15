#include "musicuserwindow.h"
#include "ui_musicuserwindow.h"
#include "musicuserdialog.h"
#include "musicusermodel.h"
#include "musicusermanager.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"

#include <QTimer>

MusicUserWindow::MusicUserWindow(QWidget *parent)
   : QStackedWidget(parent),
     ui(new Ui::MusicUserWindow)
{
    ui->setupUi(this);
    ui->userNameL->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    ui->userNameL->setCursor(QCursor(Qt::PointingHandCursor));
    ui->userNameU->setCursor(QCursor(Qt::PointingHandCursor));

    connectDatabase();

    m_userManager = new MusicUserManager(this);
    connect(ui->userIconU, SIGNAL(clicked()), SLOT(musicUserLogin()));
    connect(ui->userNameU, SIGNAL(clicked()), SLOT(musicUserLogin()));
    connect(ui->userIconL, SIGNAL(clicked()), m_userManager, SLOT(exec()));
    connect(ui->userNameL, SIGNAL(clicked()), m_userManager, SLOT(exec()));
    connect(m_userManager, SIGNAL(userStateChanged(QString,QString)),
                           SLOT(userStateChanged(QString,QString)));

    QTimer::singleShot(MT_MS, this, SLOT(checkToAutoLogin()));
}

MusicUserWindow::~MusicUserWindow()
{
    delete m_userManager;
    delete ui;
    disConnectDatabase();
}

QString MusicUserWindow::getClassName()
{
    return staticMetaObject.className();
}

bool MusicUserWindow::isUserLogin() const
{
    return currentIndex() == 1;
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
            data = QSqlDatabase::addDatabase(SQLITE_DATABASE, "user-data");
        }
        data.setDatabaseName(DARABASEPATH_FULL);
        if( !data.isDriverAvailable(SQLITE_DATABASE) )
        {
            throw QString("The driver name is not available!");
        }
        if( !data.isValid() )
        {
            throw QString("The database has not a valid driver!");
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

void MusicUserWindow::userStateChanged(const QString &uid, const QString &icon)
{
    if(uid.isEmpty())
    {
        ui->userIconU->setPixmap(MusicUtils::Widget::pixmapToRound(QPixmap(":/image/lb_player_logo"),
                                                                   QPixmap(":/usermanager/lb_mask"),
                                                                   ui->userIconU->size()));
        ui->userNameU->setText(tr("L|R"));
        setCurrentIndex(0);
    }
    else
    {
        m_userManager->setUserUID(uid);
        ui->userIconL->setPixmap(MusicUtils::Widget::pixmapToRound(QPixmap(icon),
                                                                   QPixmap(":/usermanager/lb_mask"),
                                                                   ui->userIconL->size()));
        ui->userNameL->setText(MusicUtils::Widget::elidedText(font(), uid, Qt::ElideRight, 44));
        ui->userNameL->setToolTip(uid);
        setCurrentIndex(1);
    }
}

void MusicUserWindow::musicUserLogin()
{
    MusicUserDialog dialog;
    connect(&dialog, SIGNAL(userLoginSuccess(QString,QString)),
                     SLOT(userStateChanged(QString,QString)));
    dialog.exec();
}

void MusicUserWindow::musicUserContextLogin()
{
    if(currentIndex() == 1)
    {
        m_userManager->musicUserLogoff();
        return;
    }
    QTimer::singleShot(MT_MS, this, SLOT(musicUserLogin()));
}

void MusicUserWindow::checkToAutoLogin()
{
    MusicUserDialog dialog;
    QString name, icon;
    dialog.checkToAutoLogin(name, icon);
    userStateChanged(name, icon);
}
