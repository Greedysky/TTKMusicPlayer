#include "musicuserdialog.h"
#include "musicuiobject.h"
#include "ui_musicuserdialog.h"
#include "musicuserlineedit.h"
#include "musicusermodel.h"
#include "musictoastlabel.h"
#include "musicwidgetutils.h"

#include "musicuserrecordwidget.h"
#include "musicdownloadsourcerequest.h"

#include <QValidator>
#include <QStyledItemDelegate>

#define DEFAULT_LOGIN_SERVER    0

MusicUserDialog::MusicUserDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicUserDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    MusicTime::initRandom();
    changeVerificationCode();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    firstStatckWidget();
    secondStatckWidget();
    thirdStatckWidget();

    m_userUID = MusicUserUIDItem(m_ui->userComboBox->currentText(), DEFAULT_LOGIN_SERVER);

    connect(m_ui->userComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(userComboBoxChanged(QString)));
    connect(m_ui->userComboBox, SIGNAL(editTextChanged(QString)), SLOT(userEditTextChanged(QString)));

    readFromUserConfig();
}

MusicUserDialog::~MusicUserDialog()
{
    delete m_ui;
}

bool MusicUserDialog::checkToAutoLogin()
{
    for(const MusicUserRecord &record  : qAsConst(m_records))
    {
        if(record.m_autoFlag)
        {
            m_userUID = MusicUserUIDItem(record.m_uid, record.m_server);
            MusicUtils::Widget::setComboBoxText(m_ui->userComboBox, record.m_uid);
            readFromUserSettings();
            break;
        }
    }

    bool result = false;
    if(m_ui->automaticLogin->isChecked() && m_ui->rememberPwd->isChecked())
    {
        checkUserLogin();
        result = true;
    }

    return result;
}

void MusicUserDialog::setUserModel(MusicUserModel *model)
{
    m_userModel = model;
    m_ui->userComboBox->addItems(m_userModel->getAllUsers());
}

void MusicUserDialog::userLogin()
{
    windowRectChanged(0, 251);
}

void MusicUserDialog::checkUserLogin()
{
    const MusicUserUIDItem uid(m_ui->userComboBox->currentText(), DEFAULT_LOGIN_SERVER);
    const QString &pwd = m_ui->passwLineEdit->text();

    if(!m_ui->rememberPwd->isChecked() || pwd != m_userModel->getUserPWDMD5(m_userUID))
    {
        if(!m_userModel->passwordCheck(uid, pwd))
        {
            MusicToastLabel::popup(tr("You passwd is incorrect or user is not exist"));
            return;
        }
    }

    if(uid.m_uid.trimmed().isEmpty() || pwd.trimmed().isEmpty())
    {
        MusicToastLabel::popup(tr("You entered is incorrect"));
        return;
    }

    writeToUserConfig();

    m_userUID = uid;
    Q_EMIT userLoginSuccess(m_userUID, m_userModel->getUserIcon(m_userUID));
    close();
}

void MusicUserDialog::registerUser()
{
    windowRectChanged(1, 400);
}

void MusicUserDialog::checkRegisterUser()
{
    if(m_ui->registerUserLine->getStrStatus() && m_ui->registerMailLine->getMailStatus() &&
       m_ui->registerPwdLine->getStrStatus() && m_ui->registerPwdCLine->getStrStatus())
    {
        if(m_ui->registerPwdLine->text() != m_ui->registerPwdCLine->text())
        {
            MusicToastLabel::popup(tr("The two passwords do not match"));
            return;
        }
        if(!m_ui->agreementCheckBox->isChecked())
        {
            MusicToastLabel::popup(tr("The agreement does not tick"));
            return;
        }
        if(!m_userModel->addUser(MusicUserUIDItem(m_ui->registerUserLine->text(), DEFAULT_LOGIN_SERVER),
                                                  m_ui->registerPwdLine->text(), m_ui->registerMailLine->text()))
        {
            MusicToastLabel::popup(tr("The username is existed"));
            return;
        }

        MusicToastLabel::popup(tr("The register successfully"));
        userLogin();
    }
    else
    {
        MusicToastLabel::popup(tr("You entered is incorrect"));
    }
}

void MusicUserDialog::userForgotPasswd()
{
    windowRectChanged(2, 330);
}

void MusicUserDialog::checkUserForgotPasswd()
{
     const MusicUserUIDItem user(m_ui->userLineEdit->text(), DEFAULT_LOGIN_SERVER);
     const QString &mail = m_ui->mailLineEdit->text();

     if(user.m_uid.trimmed().isEmpty() || mail.trimmed().isEmpty())
     {
         MusicToastLabel::popup(tr("You entered is incorrect"));
         return;
     }
     if(!m_userModel->mailCheck(user, mail))
     {
         MusicToastLabel::popup(tr("You mail is incorrect or user is not exist"));
         return;
     }
     if(!m_ui->pwdLineEdit->getStrStatus())
     {
         MusicToastLabel::popup(tr("You passwd is incorrect"));
         return;
     }
     if(m_ui->verificationCode->getCheckCode() != m_ui->verificationCodeEdit->text().trimmed())
     {
         MusicToastLabel::popup(tr("You verificationCode is incorrect"));
         return;
     }
     if(m_userModel->updateUser(user, m_ui->pwdLineEdit->text(), mail, QString(), QString()))
     {
         MusicToastLabel::popup(tr("Change password successfully"));
     }
     userLogin();
}

void MusicUserDialog::changeVerificationCode()
{
    m_ui->verificationCode->setCodeCount(6);
    m_ui->verificationCode->setNoisyPointCount(100);
    m_ui->verificationCode->renderPicture();
}

void MusicUserDialog::userComboBoxChanged(const QString &uid)
{
    m_userUID = MusicUserUIDItem(uid, DEFAULT_LOGIN_SERVER);
    readFromUserSettings();
}

void MusicUserDialog::userEditTextChanged(const QString &uid)
{
    if(m_userModel->getAllUsers().contains(uid))
    {
        m_userUID = MusicUserUIDItem(uid, DEFAULT_LOGIN_SERVER);
        readFromUserSettings();
    }
    else
    {
        m_ui->automaticLogin->setChecked(false);
        m_ui->rememberPwd->setChecked(false);
        m_ui->passwLineEdit->clear();
    }
}

void MusicUserDialog::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);

    const QString &path = MusicUserRecordWidget::avatarPixmapRender(m_userUID, pix);
    m_userModel->updateUserIcon(m_userUID, path);
    Q_EMIT userLoginSuccess(m_userUID, m_userModel->getUserIcon(m_userUID));
}

int MusicUserDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicUserDialog::firstStatckWidget()
{
    m_ui->userComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->userComboBox));
    m_ui->userComboBox->setStyleSheet(MusicUIObject::MQSSComboBoxStyle01 + MusicUIObject::MQSSItemView01);
    m_ui->userComboBox->view()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    m_ui->passwLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->loginButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->forgotPwdButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle05);
    m_ui->registerButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle05);
    m_ui->rememberPwd->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->automaticLogin->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

    m_ui->registerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->forgotPwdButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->registerButton, SIGNAL(clicked()), SLOT(registerUser()));
    connect(m_ui->forgotPwdButton, SIGNAL(clicked()), SLOT(userForgotPasswd()));
    connect(m_ui->loginButton, SIGNAL(clicked()), SLOT(checkUserLogin()));
#ifdef Q_OS_UNIX
    m_ui->registerButton->setFocusPolicy(Qt::NoFocus);
    m_ui->forgotPwdButton->setFocusPolicy(Qt::NoFocus);
    m_ui->loginButton->setFocusPolicy(Qt::NoFocus);
    m_ui->rememberPwd->setFocusPolicy(Qt::NoFocus);
    m_ui->automaticLogin->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicUserDialog::secondStatckWidget()
{
    m_ui->registerUserLine->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->registerMailLine->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->registerPwdLine->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->registerPwdCLine->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->confirmButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->agreementButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle05);
    m_ui->agreementCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);

#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
    m_ui->agreementButton->setFocusPolicy(Qt::NoFocus);
    m_ui->agreementCheckBox->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(checkRegisterUser()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    m_ui->registerUserLine->setValidator(valid);
    m_ui->registerMailLine->setValidator(valid);
    m_ui->registerPwdLine->setValidator(valid);
    m_ui->registerPwdCLine->setValidator(valid);

    m_ui->registerUserLine->setLabel(MusicUserLineEdit::User, m_ui->registerUserLineR, m_ui->labelRight);
    m_ui->registerMailLine->setLabel(MusicUserLineEdit::Mail, m_ui->registerMailLineR, m_ui->labelRigh_2);
    m_ui->registerPwdLine->setLabel(MusicUserLineEdit::Passwd, m_ui->registerPwdLineR, m_ui->labelRigh_3);
    m_ui->registerPwdCLine->setLabel(MusicUserLineEdit::PasswdConfirm, m_ui->registerPwdCLineR, m_ui->labelRigh_4);
}

void MusicUserDialog::thirdStatckWidget()
{
    m_ui->userLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->pwdLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->mailLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->verificationCodeEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->confirmButton_2->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->cancelButton_2->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->confirmButton_2->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton_2->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->confirmButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->confirmButton_2, SIGNAL(clicked()), SLOT(checkUserForgotPasswd()));
    connect(m_ui->cancelButton_2, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    m_ui->userLineEdit->setValidator(valid);
    m_ui->pwdLineEdit->setValidator(valid);
    m_ui->mailLineEdit->setValidator(valid);

    m_ui->pwdLineEdit->setLabel(MusicUserLineEdit::Passwd, m_ui->pwdLineEditR, m_ui->labelRigh_5);
    connect(m_ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCode()));
}

void MusicUserDialog::clearOriginData()
{
    m_ui->registerUserLineR->hide();
    m_ui->registerMailLineR->hide();
    m_ui->registerPwdLineR->hide();
    m_ui->registerPwdCLineR->hide();
    m_ui->labelRight->hide();
    m_ui->labelRigh_2->hide();
    m_ui->labelRigh_3->hide();
    m_ui->labelRigh_4->hide();
    m_ui->pwdLineEditR->hide();
    m_ui->labelRigh_5->hide();

    m_ui->passwLineEdit->clear();
    m_ui->registerUserLine->clear();
    m_ui->registerMailLine->clear();
    m_ui->registerPwdLine->clear();
    m_ui->registerPwdCLine->clear();
    m_ui->userLineEdit->clear();
    m_ui->pwdLineEdit->clear();
    m_ui->mailLineEdit->clear();
    m_ui->verificationCodeEdit->clear();

    m_ui->automaticLogin->setChecked(false);
    m_ui->rememberPwd->setChecked(false);
#ifdef TTK_GREATER_NEW
    m_ui->userComboBox->setCurrentText(QString());
#else
    m_ui->userComboBox->setCurrentIndex(-1);
#endif
}

void MusicUserDialog::readFromUserConfig()
{
    MusicUserConfigManager xml;
    if(!xml.readConfig())
    {
        return;
    }
    xml.readUserData(m_records);
    readFromUserSettings();
}

int MusicUserDialog::findUserNameIndex(const MusicUserUIDItem &uid)
{
    for(int i=0; i<m_records.count(); ++i)
    {
        if(m_records[i].m_uid == uid.m_uid)
        {
            return i;
        }
    }
    return -1;
}

void MusicUserDialog::readFromUserSettings()
{
    const int index = findUserNameIndex(m_userUID);
    if(index != -1)
    {
        m_ui->automaticLogin->setChecked(m_records[index].m_autoFlag);
        m_ui->rememberPwd->setChecked(m_records[index].m_rememberFlag);
        m_ui->passwLineEdit->setText(m_records[index].m_password);
    }
}

void MusicUserDialog::writeToUserConfig()
{
    MusicUserConfigManager xml;
    writeToUserSettings();
    xml.writeUserData(m_records);
}

void MusicUserDialog::writeToUserSettings()
{
    if(m_ui->automaticLogin->isChecked())
    {
        for(int i=0; i<m_records.count(); ++i)
        {
            m_records[i].m_autoFlag = false;
        }
    }

    const int index = findUserNameIndex(m_userUID);
    if(index != -1)
    {
        m_records[index].m_autoFlag = m_ui->automaticLogin->isChecked();
        m_records[index].m_rememberFlag = m_ui->rememberPwd->isChecked();
        m_records[index].m_password = m_ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userUID) : QString();
    }
    else
    {
        MusicUserRecord record;
        record.m_uid = m_userUID.m_uid;
        record.m_server = m_userUID.m_server;
        record.m_autoFlag = m_ui->automaticLogin->isChecked();
        record.m_rememberFlag = m_ui->rememberPwd->isChecked();
        record.m_password = m_ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userUID) : QString();
        m_records << record;
    }
}

void MusicUserDialog::windowRectChanged(int index, int height)
{
    clearOriginData();
    m_ui->stackedWidget->setCurrentIndex(index);
    m_ui->stackedWidget->setGeometry(QRect(4, 29, 381, height));

    QRect other = geometry();
    other.setHeight(height + 33);
    setGeometry(other);
}
