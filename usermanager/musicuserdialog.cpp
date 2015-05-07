#include "musicuserdialog.h"
#include "../core/musicobject.h"
#include "ui_musicuserdialog.h"
#include "musicuserlineedit.h"
#include "musicuserModel.h"
#include "musicuserconfigmanager.h"
#include <QValidator>
#include <QMessageBox>
#include <time.h>
#include <QTimer>

MusicUserDialog::MusicUserDialog(QWidget *parent) :
    MusicMoveDialogAbstract(parent),ui(new Ui::MusicUserDialog)
{
    ui->setupUi(this);

    m_userModel = new MusicUserModel(this);
    qsrand(time(NULL));
    changeVerificationCode();

    ui->topTitleCloseButton->setIcon(QIcon(QPixmap(":/share/searchclosed")));
    ui->topTitleCloseButton->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    firstStatckWidget();
    secondStatckWidget();
    thirdStatckWidget();
    drawRoundedRect();
    ui->userComboBox->addItems(m_userModel->getAllUsers());
    connect(ui->userComboBox,SIGNAL(currentIndexChanged(QString)),
            SLOT(userComboBoxChanged(QString)));
    m_userComboIndex = ui->userComboBox->currentText();
    readFromUserConfig();

    QTimer::singleShot(1, this, SLOT(checkToAutoLogin()));
}

MusicUserDialog::~MusicUserDialog()
{
    writeToUserConfig();
    delete m_userModel;
    delete ui;
}

void MusicUserDialog::readFromUserConfig()
{
    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
        return;
    xml.readUserConfig(m_parameter);
    readFromUserSettings();
}

void MusicUserDialog::writeToUserConfig()
{
    MusicUserConfigManager xml;
    writeToUserSettings();
    xml.writeUserXMLConfig(m_parameter);
}

void MusicUserDialog::firstStatckWidget()
{
    ui->userComboBox->setStyleSheet(MusicObject::MusicLoginComboboxEditStyle);
    ui->passwLineEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->loginButton->setStyleSheet(MusicObject::MusicLoginPushButtonStyle);
    ui->forgotPwdButton->setStyleSheet(MusicObject::MusicPushButtonTextStyle);
    ui->registerButton->setStyleSheet(MusicObject::MusicPushButtonTextStyle);

    ui->registerButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->forgotPwdButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->registerButton,SIGNAL(clicked()),SLOT(registerUser()));
    connect(ui->forgotPwdButton,SIGNAL(clicked()),SLOT(userForgotPasswd()));
    connect(ui->loginButton,SIGNAL(clicked()),SLOT(checkUserLogin()));
}

void MusicUserDialog::secondStatckWidget()
{
    ui->registerUserLine->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->registerMailLine->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->registerPwdLine->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->registerPwdCLine->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->confirmButton->setStyleSheet(MusicObject::MusicLoginPushButtonStyle);
    ui->cancelButton->setStyleSheet(MusicObject::MusicLoginPushButtonStyle);
    ui->agreementButton->setStyleSheet(MusicObject::MusicPushButtonTextStyle);

    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton,SIGNAL(clicked()),SLOT(checkRegisterUser()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx,this);
    ui->registerUserLine->setValidator(valid);
    ui->registerMailLine->setValidator(valid);
    ui->registerPwdLine->setValidator(valid);
    ui->registerPwdCLine->setValidator(valid);

    ui->registerUserLine->setLabel(User,ui->registerUserLineR,ui->labelRight);
    ui->registerMailLine->setLabel(Mail,ui->registerMailLineR,ui->labelRigh_2);
    ui->registerPwdLine->setLabel(Passwd,ui->registerPwdLineR,ui->labelRigh_3);
    ui->registerPwdCLine->setLabel(PasswdC,ui->registerPwdCLineR,ui->labelRigh_4);
}

void MusicUserDialog::thirdStatckWidget()
{
    ui->userLineEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->pwdLineEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->mailLineEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->verificationCodeEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->confirmButton_2->setStyleSheet(MusicObject::MusicLoginPushButtonStyle);
    ui->cancelButton_2->setStyleSheet(MusicObject::MusicLoginPushButtonStyle);

    ui->confirmButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton_2,SIGNAL(clicked()),SLOT(checkUserForgotPasswd()));
    connect(ui->cancelButton_2,SIGNAL(clicked()),SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx,this);
    ui->userLineEdit->setValidator(valid);
    ui->pwdLineEdit->setValidator(valid);
    ui->mailLineEdit->setValidator(valid);

    ui->pwdLineEdit->setLabel(Passwd,ui->pwdLineEditR,ui->labelRigh_5);
    ui->verificationCode->setStyleSheet(MusicObject::MusicVCodePushButtonStyle);
    connect(ui->verificationCode,SIGNAL(clicked()),
                                 SLOT(changeVerificationCode()));
}

void MusicUserDialog::clearOriginData()
{
    ui->registerUserLineR->hide();
    ui->registerMailLineR->hide();
    ui->registerPwdLineR->hide();
    ui->registerPwdCLineR->hide();
    ui->labelRight->hide();
    ui->labelRigh_2->hide();
    ui->labelRigh_3->hide();
    ui->labelRigh_4->hide();
    ui->pwdLineEditR->hide();
    ui->labelRigh_5->hide();

//    ui->userComboBox->clear();
    ui->passwLineEdit->clear();
    ui->registerUserLine->clear();
    ui->registerMailLine->clear();
    ui->registerPwdLine->clear();
    ui->registerPwdCLine->clear();
    ui->userLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->mailLineEdit->clear();
    ui->verificationCodeEdit->clear();
}

void MusicUserDialog::changeVerificationCode()
{
    ui->verificationCode->setText(QString::number(qrand()));
}

void MusicUserDialog::drawRoundedRect()
{
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius
}

void MusicUserDialog::userLogin()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setGeometry(1,35,329,180);
    QRect rect = geometry();
    rect.setHeight(216);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::registerUser()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setGeometry(1,35,329,299);
    QRect rect = geometry();
    rect.setHeight(335);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::userForgotPasswd()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget->setGeometry(1,35,329,250);
    QRect rect = geometry();
    rect.setHeight(286);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::checkUserLogin()
{
    QString user = ui->userComboBox->currentText();
    QString pwd = ui->passwLineEdit->text();

    if(!ui->rememberPwd->isChecked() ||
       pwd != m_userModel->getUserPWDMD5(m_userComboIndex) )
    {
        if( user.trimmed() == "" || pwd.trimmed() == "" )
        {
            QMessageBox::warning(this,tr("QMusicPlayer"),
                                 tr("You entered is incorrect"));
            return;
        }
        if( !m_userModel->checkUser(user,pwd) )
        {
            QMessageBox::warning(this,tr("QMusicPlayer"),
                         tr("You passwd is incorrect or user is not exist"));
            return;
        }
    }
    emit userLoginSuccess(user);
    close();
}

void MusicUserDialog::checkRegisterUser()
{
    if( ui->registerUserLine->getStrStatus() &&
        ui->registerMailLine->getMailStatus() &&
        ui->registerPwdLine->getStrStatus() &&
        ui->registerPwdCLine->getStrStatus() )
    {
        if( ui->registerPwdLine->text() !=
            ui->registerPwdCLine->text() )
        {
            QMessageBox::warning(this,tr("QMusicPlayer"),
                                 tr("The two passwords do not match"));
            return;
        }
        if( !ui->agreementCheckBox->isChecked() )
        {
            QMessageBox::warning(this,tr("QMusicPlayer"),
                                 tr("The agreement does not tick"));
            return;
        }
        if( !m_userModel->addUser(ui->registerUserLine->text(),
                                 ui->registerPwdLine->text(),
                                 ui->registerMailLine->text()) )
        {
            QMessageBox::warning(this,tr("QMusicPlayer"),
                                 tr("The username is existed"));
            return;
        }
        QMessageBox::information(this,tr("QMusicPlayer"),
                             tr("The register successfully"));
        userLogin();
    }
    else
        QMessageBox::warning(this,tr("QMusicPlayer"),
                             tr("You entered is incorrect"));
}

void MusicUserDialog::checkUserForgotPasswd()
{
     QString user = ui->userLineEdit->text();
     QString mail = ui->mailLineEdit->text();
     if( user.trimmed() == "" || mail.trimmed() == "" )
     {
         QMessageBox::warning(this,tr("QMusicPlayer"),
                              tr("You entered is incorrect"));
         return;
     }
     if( !m_userModel->mailCheck(user,mail) )
     {
         QMessageBox::warning(this,tr("QMusicPlayer"),
                      tr("You mail is incorrect or user is not exist"));
         return;
     }
     if( !ui->pwdLineEdit->getStrStatus() )
     {
         QMessageBox::warning(this,tr("QMusicPlayer"),
                                   tr("You passwd is incorrect"));
         return;
     }
     if( ui->verificationCode->text() !=
         ui->verificationCodeEdit->text().trimmed() )
     {
         QMessageBox::warning(this,tr("QMusicPlayer"),
                                   tr("You verificationCode is incorrect"));
         return;
     }
     if( m_userModel->updateUser(user,ui->pwdLineEdit->text(),mail,"","") )
         QMessageBox::warning(this,tr("QMusicPlayer"),
                                   tr("Change password successfully"));
     userLogin();
}

void MusicUserDialog::readFromUserSettings()
{
    QMapIterator<QString, QStringList> p(m_parameter);
    bool flag;
    while(p.hasNext())
    {
        p.next();
        if(p.key() == m_userComboIndex)
        {
            p.value()[0] == "0" ? flag = false : flag = true;
            ui->automaticLogon->setChecked(flag);
            p.value()[1] == "0" ? flag = false : flag = true;
            ui->rememberPwd->setChecked(flag);
            ui->passwLineEdit->setText(p.value()[2]);
            break;
        }
    }
}

void MusicUserDialog::writeToUserSettings()
{
    m_parameter.insert(m_userComboIndex,QStringList()
                        <<(ui->automaticLogon->isChecked() ? "1" : "0")
                        <<(ui->rememberPwd->isChecked() ? "1" : "0")
    <<(ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userComboIndex)
                                    : QString() ));
}

void MusicUserDialog::userComboBoxChanged(QString index)
{
    writeToUserSettings();
    m_userComboIndex = index;
    readFromUserSettings();
}

void MusicUserDialog::checkToAutoLogin()
{
    if(ui->automaticLogon->isChecked() &&
       ui->rememberPwd->isChecked() &&
       ui->passwLineEdit->text() == m_userModel->getUserPWDMD5(m_userComboIndex))
    {
        emit userLoginSuccess(m_userComboIndex);
        close();
    }
}
