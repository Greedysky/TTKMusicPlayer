#include "musicuserwindow.h"
#include "ui_musicuserwindow.h"
#include "musicuserdialog.h"
#include "musicusermodel.h"
#include "musicusermanagerdialog.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"
#include "musicotherdefine.h"

#include <QTimer>
#include <QSqlQuery>

MusicUserWindow::MusicUserWindow(QWidget *parent)
   : QStackedWidget(parent),
     m_ui(new Ui::MusicUserWindow)
{
    m_ui->setupUi(this);

    m_ui->userNameL->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->userNameU->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->userNameL->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->userNameU->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->userNameL->setFocusPolicy(Qt::NoFocus);
    m_ui->userNameU->setFocusPolicy(Qt::NoFocus);
#endif

    connectDatabase();

    m_userModel = new MusicUserModel(this);
    m_userManager = new MusicUserManagerDialog(this);
    m_userManager->setUserModel(m_userModel);

    connect(m_ui->userIconU, SIGNAL(clicked()), SLOT(musicUserLogin()));
    connect(m_ui->userNameU, SIGNAL(clicked()), SLOT(musicUserLogin()));
    connect(m_ui->userIconL, SIGNAL(clicked()), m_userManager, SLOT(exec()));
    connect(m_ui->userNameL, SIGNAL(clicked()), m_userManager, SLOT(exec()));
    connect(m_userManager, SIGNAL(userStateChanged(MusicUserUIDItem,QString)), SLOT(userStateChanged(MusicUserUIDItem,QString)));

    QTimer::singleShot(MT_MS, this, SLOT(checkToAutoLogin()));
}

MusicUserWindow::~MusicUserWindow()
{
    delete m_userManager;
    delete m_userModel;
    delete m_ui;
    disConnectDatabase();
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
        if(data.isValid())
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
            data = QSqlDatabase::addDatabase(DB_SQLITE_DATABASE, "user-data");
        }
        data.setDatabaseName(DARABASEPATH_FULL);
        if(!data.isDriverAvailable(DB_SQLITE_DATABASE))
        {
            throw QString("The driver name is not available!");
        }
        if(!data.isValid())
        {
            throw QString("The database has not a valid driver!");
        }
        if(!data.isOpen() && !data.open())
        {
            throw QString("Can not open database connection!");
        }
        if(!data.tables().contains("MusicUser"))
        {
            QSqlQuery query(data);
            QString musicUserSql = QString("create table MusicUser (USERID vchar(%1),SERVER vchar(%2),"
                    "PASSWD vchar(%3),EMAIL vchar(%4),USERNAME vchar(%5),LOGINTIME vchar(%6),"
                    "SEX vchar(%7),ICON TEXT,BIRTHDAY Date,CITY TEXT,COUNTRY TEXT,SIGNATURE TEXT)")
                    .arg(DB_USERID).arg(DB_SERVER).arg(DB_PASSWD).arg(DB_EMAIL).arg(DB_USERNAME).arg(DB_LOGINTIME).arg(DB_SEX);
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

void MusicUserWindow::userStateChanged(const MusicUserUIDItem &uid, const QString &icon)
{
    if(uid.m_uid.isEmpty())
    {
        m_ui->userIconU->setPixmap(MusicUtils::Widget::pixmapToRound(QPixmap(":/image/lb_app_logo"), QPixmap(":/usermanager/lb_mask_30"), m_ui->userIconU->size()));
        m_ui->userNameU->setText(tr("L|R"));
        setCurrentIndex(USER_WINDOW_INDEX_0);
    }
    else
    {
        m_userManager->setUserUID(uid);
        m_ui->userIconL->setPixmap(MusicUtils::Widget::pixmapToRound(QPixmap(icon), QPixmap(":/usermanager/lb_mask_30"), m_ui->userIconL->size()));
        m_ui->userNameL->setToolTip(m_userModel->getUserName(uid));
        m_ui->userNameL->setText(MusicUtils::Widget::elidedText(font(), m_ui->userNameL->toolTip(), Qt::ElideRight, 44));
        setCurrentIndex(USER_WINDOW_INDEX_1);
    }
}

void MusicUserWindow::musicUserLogin()
{
    MusicUserDialog dialog;
    dialog.setUserModel(m_userModel);
    connect(&dialog, SIGNAL(userLoginSuccess(MusicUserUIDItem,QString)), SLOT(userStateChanged(MusicUserUIDItem,QString)));
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
    userStateChanged(MusicUserUIDItem(), QString());

    MusicUserDialog dialog;
    connect(&dialog, SIGNAL(userLoginSuccess(MusicUserUIDItem,QString)), SLOT(userStateChanged(MusicUserUIDItem,QString)));

    dialog.setUserModel(m_userModel);
    if(dialog.checkToAutoLogin())
    {
        dialog.exec();
    }
}
