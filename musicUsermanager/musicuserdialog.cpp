#include "musicuserdialog.h"
#include "musicuiobject.h"
#include "ui_musicuserdialog.h"
#include "musicuserlineedit.h"
#include "musicuserModel.h"
#include "musicmessagebox.h"
#include "musictime.h"

#include <QValidator>
#include <QButtonGroup>

MusicUserDialog::MusicUserDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicUserDialog)
{
    ui->setupUi(this);

    m_userModel = new MusicUserModel(this);

    MusicTime::timeSRand();
    changeVerificationCode();

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    firstStatckWidget();
    secondStatckWidget();
    thirdStatckWidget();

    ui->userComboBox->addItems(m_userModel->getAllUsers());
    connect(ui->userComboBox, SIGNAL(currentIndexChanged(QString)),
                              SLOT(userComboBoxChanged(QString)));
    connect(ui->userComboBox, SIGNAL(editTextChanged(QString)),
                              SLOT(userEditTextChanged(QString)));
    m_userName = ui->userComboBox->currentText();
    readFromUserConfig();

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->useTencentLogin, 0);
    buttonGroup->addButton(ui->useRenrenLogin, 1);
    buttonGroup->addButton(ui->useWechatLogin, 2);
    buttonGroup->addButton(ui->useSinaLogin, 3);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
}

MusicUserDialog::~MusicUserDialog()
{
    delete m_userModel;
    delete ui;
}

void MusicUserDialog::readFromUserConfig()
{
    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
    {
        return;
    }
    xml.readUserConfig(m_record);
    readFromUserSettings();
}

void MusicUserDialog::writeToUserConfig()
{
    MusicUserConfigManager xml;
    writeToUserSettings();
    xml.writeUserXMLConfig(m_record);
}

void MusicUserDialog::firstStatckWidget()
{
    ui->userComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->userComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->passwLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->loginButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->forgotPwdButton->setStyleSheet(MusicUIObject::MPushButtonStyle09);
    ui->registerButton->setStyleSheet(MusicUIObject::MPushButtonStyle09);

    ui->registerButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->forgotPwdButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->registerButton, SIGNAL(clicked()), SLOT(registerUser()));
    connect(ui->forgotPwdButton, SIGNAL(clicked()), SLOT(userForgotPasswd()));
    connect(ui->loginButton, SIGNAL(clicked()), SLOT(checkUserLogin()));
}

void MusicUserDialog::secondStatckWidget()
{
    ui->registerUserLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->registerMailLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->registerPwdLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->registerPwdCLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->agreementButton->setStyleSheet(MusicUIObject::MPushButtonStyle09);

    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(checkRegisterUser()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    ui->registerUserLine->setValidator(valid);
    ui->registerMailLine->setValidator(valid);
    ui->registerPwdLine->setValidator(valid);
    ui->registerPwdCLine->setValidator(valid);

    ui->registerUserLine->setLabel(MusicUserLineEdit::User, ui->registerUserLineR, ui->labelRight);
    ui->registerMailLine->setLabel(MusicUserLineEdit::Mail, ui->registerMailLineR, ui->labelRigh_2);
    ui->registerPwdLine->setLabel(MusicUserLineEdit::Passwd, ui->registerPwdLineR, ui->labelRigh_3);
    ui->registerPwdCLine->setLabel(MusicUserLineEdit::PwdConfirm, ui->registerPwdCLineR, ui->labelRigh_4);
}

void MusicUserDialog::thirdStatckWidget()
{
    ui->userLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->pwdLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->mailLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->verificationCodeEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->confirmButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->cancelButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle08);

    ui->confirmButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton_2, SIGNAL(clicked()), SLOT(checkUserForgotPasswd()));
    connect(ui->cancelButton_2, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    ui->userLineEdit->setValidator(valid);
    ui->pwdLineEdit->setValidator(valid);
    ui->mailLineEdit->setValidator(valid);

    ui->pwdLineEdit->setLabel(MusicUserLineEdit::Passwd, ui->pwdLineEditR, ui->labelRigh_5);
    ui->verificationCode->setStyleSheet(MusicUIObject::MPushButtonStyle10);
    connect(ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCode()));
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

    ui->passwLineEdit->clear();
    ui->registerUserLine->clear();
    ui->registerMailLine->clear();
    ui->registerPwdLine->clear();
    ui->registerPwdCLine->clear();
    ui->userLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->mailLineEdit->clear();
    ui->verificationCodeEdit->clear();

    ui->automaticLogon->setChecked(false);
    ui->rememberPwd->setChecked(false);
#ifdef MUSIC_QT_5
    ui->userComboBox->setCurrentText(QString());
#else
    ui->userComboBox->setCurrentIndex(-1);
#endif
}

void MusicUserDialog::changeVerificationCode()
{
    ui->verificationCode->setText(QString::number(qrand()).leftJustified(5, '0'));
}

void MusicUserDialog::userLogin()
{
    windowRectChanged(0, QRect(20, 45, 331, 181));
}

void MusicUserDialog::registerUser()
{
    windowRectChanged(1, QRect(20, 45, 331, 300));
}

void MusicUserDialog::userForgotPasswd()
{
    windowRectChanged(2, QRect(20, 45, 331, 251));
}

void MusicUserDialog::windowRectChanged(int index, const QRect &rect)
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(index);
    ui->stackedWidget->setGeometry(rect);
    QRect other = geometry();
    other.setHeight(rect.height() + 61);
    setGeometry(other);
}

void MusicUserDialog::checkUserLogin()
{
    QString user = ui->userComboBox->currentText();
    QString pwd = ui->passwLineEdit->text();

    if(!ui->rememberPwd->isChecked() ||
       pwd != m_userModel->getUserPWDMD5(m_userName) )
    {
        if( !m_userModel->passwordCheck(user, pwd) )
        {
            MusicMessageBox message;
            message.setText(tr("You passwd is incorrect or user is not exist"));
            message.exec();
            return;
        }
    }
    if( user.trimmed() == "" || pwd.trimmed() == "" )
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
        return;
    }
    writeToUserConfig();

    emit userLoginSuccess(user, m_userModel->getUserIcon(user));
    close();
}

void MusicUserDialog::checkRegisterUser()
{
    if( ui->registerUserLine->getStrStatus() &&
        ui->registerMailLine->getMailStatus() &&
        ui->registerPwdLine->getStrStatus() &&
        ui->registerPwdCLine->getStrStatus() )
    {
        if( ui->registerPwdLine->text() != ui->registerPwdCLine->text() )
        {
            MusicMessageBox message;
            message.setText(tr("The two passwords do not match"));
            message.exec();
            return;
        }
        if( !ui->agreementCheckBox->isChecked() )
        {
            MusicMessageBox message;
            message.setText(tr("The agreement does not tick"));
            message.exec();
            return;
        }
        if( !m_userModel->addUser(ui->registerUserLine->text(),
                                  ui->registerPwdLine->text(),
                                  ui->registerMailLine->text()) )
        {
            MusicMessageBox message;
            message.setText(tr("The username is existed"));
            message.exec();
            return;
        }
        MusicMessageBox message;
        message.setText(tr("The register successfully"));
        message.exec();

        userLogin();
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
    }
}

void MusicUserDialog::checkUserForgotPasswd()
{
     QString user = ui->userLineEdit->text();
     QString mail = ui->mailLineEdit->text();
     if( user.trimmed() == "" || mail.trimmed() == "" )
     {
         MusicMessageBox message;
         message.setText(tr("You entered is incorrect"));
         message.exec();
         return;
     }
     if( !m_userModel->mailCheck(user,mail) )
     {
         MusicMessageBox message;
         message.setText(tr("You mail is incorrect or user is not exist"));
         message.exec();
         return;
     }
     if( !ui->pwdLineEdit->getStrStatus() )
     {
         MusicMessageBox message;
         message.setText(tr("You passwd is incorrect"));
         message.exec();
         return;
     }
     if( ui->verificationCode->text() != ui->verificationCodeEdit->text().trimmed() )
     {
         MusicMessageBox message;
         message.setText(tr("You verificationCode is incorrect"));
         message.exec();
         return;
     }
     if( m_userModel->updateUser(user, ui->pwdLineEdit->text(), mail, "", "") )
     {
         MusicMessageBox message;
         message.setText(tr("Change password successfully"));
         message.exec();
     }
     userLogin();
}

void MusicUserDialog::readFromUserSettings()
{
    int index = 0;
    if((index = m_record.m_names.indexOf(m_userName)) != -1)
    {
        ui->automaticLogon->setChecked( !(m_record.m_als[index] == "0") );
        ui->rememberPwd->setChecked( !(m_record.m_rps[index] == "0") );
        ui->passwLineEdit->setText( m_record.m_pwds[index] );
    }
}

void MusicUserDialog::writeToUserSettings()
{
    int index = 0;
    if((index = m_record.m_names.indexOf(m_userName)) != -1)
    {
        m_record.m_als[index] = ui->automaticLogon->isChecked() ? "1" : "0";
        m_record.m_rps[index] = ui->rememberPwd->isChecked() ? "1" : "0";
        m_record.m_pwds[index] = ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userName)
                                                              : QString();
    }
    else
    {
        m_record.m_names << m_userName;
        m_record.m_als << (ui->automaticLogon->isChecked() ? "1" : "0");
        m_record.m_rps << (ui->rememberPwd->isChecked() ? "1" : "0");
        m_record.m_pwds << (ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userName)
                                                         : QString() );
    }
}

void MusicUserDialog::userComboBoxChanged(const QString &name)
{
    m_userName = name;
    readFromUserSettings();
}

void MusicUserDialog::userEditTextChanged(const QString &name)
{
    if(m_userModel->getAllUsers().contains(name))
    {
        m_userName = name;
        readFromUserSettings();
    }
    else
    {
        ui->automaticLogon->setChecked(false);
        ui->rememberPwd->setChecked(false);
        ui->passwLineEdit->clear();
    }
}

void MusicUserDialog::checkToAutoLogin(QString &name, QString &icon)
{
    if(ui->automaticLogon->isChecked() && ui->rememberPwd->isChecked() &&
       ui->passwLineEdit->text() == m_userModel->getUserPWDMD5(m_userName))
    {
        name = m_userName;
        icon = m_userModel->getUserIcon(m_userName);
    }
}

void MusicUserDialog::buttonClicked(int)
{
    MusicMessageBox message;
    message.setText(tr("This way of loading is now not supported"));
    message.exec();
}
