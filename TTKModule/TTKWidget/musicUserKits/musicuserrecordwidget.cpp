#include "musicuserrecordwidget.h"
#include "ui_musicuserrecordwidget.h"
#include "musicuiobject.h"
#include "musicusermodel.h"
#include "musicmessagebox.h"
#include "musicfileutils.h"
#include "musicwidgetutils.h"
#include "musicalgorithmutils.h"

#include <QStyledItemDelegate>

MusicUserRecordWidget::MusicUserRecordWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicUserRecordWidget)
{
    m_ui->setupUi(this);

#ifdef Q_OS_UNIX
    MusicUtils::Widget::setLabelFontSize(m_ui->label_5T, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->label_6T, 9);
#endif
    m_ui->tabWidget->setStyleSheet(MusicUIObject::MTabWidgetStyle01);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicUserRecordWidget::~MusicUserRecordWidget()
{
    delete m_ui;
}

QString MusicUserRecordWidget::avatarPixmapRender(const MusicUserUIDItem &item, const QPixmap &pix)
{
    QPixmap p(pix);
    const QByteArray name(MusicUtils::Algorithm::md5(QString("%1%2%3").arg(item.m_uid).arg(TTK_STR_SPLITER).arg(item.m_server).toUtf8()));
    const QString &path = QString("%1%2").arg(AVATAR_DIR_FULL).arg(QString(name.toHex().toUpper()));
    p.save(path + JPG_FILE);
    QFile::rename(path + JPG_FILE, path);

    return path;
}

void MusicUserRecordWidget::setUserModel(MusicUserModel *model, const MusicUserUIDItem &uid)
{
    m_userModel = model;
    m_userUID = uid;
    m_ui->userIDLabel_F->setText(uid.m_uid);

    ///first tab
    initTabF();
    ///second tab
    initTabS();
    ///third tab
    initTabT();

#ifdef Q_OS_UNIX
    m_ui->maleRadioButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->femaleRadioButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton_F->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton_S->setFocusPolicy(Qt::NoFocus);
    m_ui->saveFileButton_S->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton_T->setFocusPolicy(Qt::NoFocus);
#endif

}

void MusicUserRecordWidget::initTabF()
{
    m_ui->cityComboBox_F->setItemDelegate(new QStyledItemDelegate(m_ui->cityComboBox_F));
    m_ui->cityComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->countryComboBox_F->setItemDelegate(new QStyledItemDelegate(m_ui->countryComboBox_F));
    m_ui->countryComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->cityComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->countryComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->signatureEdit_F->setStyleSheet(MusicUIObject::MTextEditStyle01);
    connect(m_ui->cityComboBox_F, SIGNAL(currentIndexChanged(QString)), SLOT(cityComboBoxIndexChanged(QString)));

    m_ui->cityComboBox_F->addItems(m_userModel->getAllCities());

    m_ui->userTimeLabel_F->setText(m_userModel->getUserLogTime(m_userUID));
    m_ui->nicknameEdit->setText(m_userModel->getUserName(m_userUID));
    m_ui->userIDLabel_F->setText(m_userUID.m_uid);

    m_ui->nicknameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->birthDateEdit_F->setStyleSheet(MusicUIObject::MDateEditStyle01);

    QString string(m_userModel->getUserSex(m_userUID));
    m_ui->maleRadioButton_F->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->femaleRadioButton_F->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->maleRadioButton_F->setChecked(string.isEmpty() || string == "0");
    m_ui->femaleRadioButton_F->setChecked(string == "1");

    string = m_userModel->getUserBirthday(m_userUID);
    m_ui->birthDateEdit_F->setDisplayFormat(QString("yyyy-MM-dd"));
    m_ui->birthDateEdit_F->setDate(string.isEmpty() ? QDate::currentDate() : QDate::fromString(string, QString("yyyy-MM-dd")));

    string = m_userModel->getUserCity(m_userUID);
    if(!string.isEmpty())
    {
        MusicUtils::Widget::setComboBoxText(m_ui->cityComboBox_F, string);
    }

    string = m_userModel->getUserCountry(m_userUID);
    if(!string.isEmpty())
    {
        MusicUtils::Widget::setComboBoxText(m_ui->countryComboBox_F, string);
    }
    m_ui->signatureEdit_F->setText(m_userModel->getUserSignature(m_userUID));
    m_ui->confirmButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle06);

    connect(m_ui->confirmButton_F, SIGNAL(clicked()), SLOT(confirmButtonClickedF()));
}

void MusicUserRecordWidget::initTabS()
{
    const QString &path = m_userModel->getUserIcon(m_userUID);
    m_ui->smlPixmapLabel_S->setPixmap(QPixmap(path).scaled(m_ui->smlPixmapLabel_S->size()));
    m_ui->openFileButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    m_ui->saveFileButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    connect(m_ui->openFileButton_S, SIGNAL(clicked()), SLOT(openFileButtonClickedS()));
    connect(m_ui->saveFileButton_S, SIGNAL(clicked()), SLOT(saveFileButtonClickedS()));
    connect(m_ui->bigPixmapLabel_S, SIGNAL(intersectedPixmap(QPixmap)), SLOT(intersectedPixmap(QPixmap)));
}

void MusicUserRecordWidget::initTabT()
{
    m_ui->tab_3->setStyleSheet(MusicUIObject::MLineEditStyle01 + "QWidget{ color:#666666;}");
    m_ui->labelRighT1->hide();
    m_ui->labelRighT2->hide();
    m_ui->labelRighT3->hide();
    m_ui->originPwdEdit_T->setLabel(MusicUserLineEdit::PasswdConfirm, nullptr, m_ui->labelRighT1);
    m_ui->newPwdEdit_T->setLabel(MusicUserLineEdit::PasswdNew, nullptr, m_ui->labelRighT2);
    m_ui->newCPwdEdit_T->setLabel(MusicUserLineEdit::PasswdConfirm, nullptr, m_ui->labelRighT3);
    connect(m_ui->newPwdEdit_T, SIGNAL(checkPwdStrength(int)), SLOT(checkPwdStrength(int)));

    changeVerificationCodeT();
    m_ui->confirmButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    connect(m_ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCodeT()));
    connect(m_ui->confirmButton_T, SIGNAL(clicked()), SLOT(confirmButtonClickedT()));
}

void MusicUserRecordWidget::cityComboBoxIndexChanged(const QString &city)
{
    const QStringList &country = m_userModel->getAllCountries(city);
    m_ui->countryComboBox_F->clear();
    m_ui->countryComboBox_F->addItems(country);
}

void MusicUserRecordWidget::confirmButtonClickedF()
{
    const QString nickname(m_ui->nicknameEdit->text());
    if(nickname != m_userModel->getUserName(m_userUID))
    {
        emit resetUserName(m_ui->nicknameEdit->text());
    }
    m_userModel->updateUser(m_userUID, nickname, m_ui->maleRadioButton_F->isChecked() ? "0" : "1",
                            m_ui->birthDateEdit_F->text(), m_ui->cityComboBox_F->currentText(),
                            m_ui->countryComboBox_F->currentText(), m_ui->signatureEdit_F->toPlainText());
    close();
}

void MusicUserRecordWidget::openFileButtonClickedS()
{
    m_iconLocalPath = MusicUtils::File::getOpenFileDialog(this);
    if(m_iconLocalPath.isEmpty())
    {
        return;
    }
    m_ui->bigPixmapLabel_S->setImagePath(m_iconLocalPath);
    m_ui->smlPixmapLabel_S->setPixmap(QPixmap( m_iconLocalPath ).scaled(m_ui->smlPixmapLabel_S->size()));
}

void MusicUserRecordWidget::saveFileButtonClickedS()
{
    if(m_iconLocalPath.isEmpty() || !QFile::exists(m_iconLocalPath))
    {
        return;
    }

    const QString &path = avatarPixmapRender(m_userUID, m_ui->bigPixmapLabel_S->pixmap());
    m_userModel->updateUserIcon(m_userUID, path);

    emit userIconChanged(m_userUID, path);
}

void MusicUserRecordWidget::intersectedPixmap(const QPixmap &pix)
{
    m_ui->smlPixmapLabel_S->setPixmap(pix.scaled(100, 100));
}

void MusicUserRecordWidget::changeVerificationCodeT()
{
    m_ui->verificationCode->setCodeCount(6);
    m_ui->verificationCode->setNoisyPointCount(100);
    m_ui->verificationCode->renderPicture();
}

void MusicUserRecordWidget::confirmButtonClickedT()
{
    if(m_ui->originPwdEdit_T->getStrStatus() && m_ui->newPwdEdit_T->getStrStatus() && m_ui->newCPwdEdit_T->getStrStatus())
    {
        if(m_ui->verificationCode->getCheckCode() != m_ui->verificationCodeEdit_T->text().trimmed())
        {
            MusicMessageBox message;
            message.setText(tr("You verificationCode is incorrect"));
            message.exec();
            return;
        }
        if(!m_userModel->passwordCheck(m_userUID, m_ui->originPwdEdit_T->text()))
        {
            MusicMessageBox message;
            message.setText(tr("The origin password does not match"));
            message.exec();
            return;
        }
        if(m_ui->newPwdEdit_T->text() != m_ui->newCPwdEdit_T->text())
        {
            MusicMessageBox message;
            message.setText(tr("The two passwords do not match"));
            message.exec();
            return;
        }
        if(!m_userModel->updateUserPwd(m_userUID, m_ui->newPwdEdit_T->text()))
        {
            MusicMessageBox message;
            message.setText(tr("The modity password failed"));
            message.exec();
            return;
        }

        emit userIconChanged(MusicUserUIDItem(), QString());
        close();

        MusicMessageBox message;
        message.setText(tr("The modity password successfully, please relogin"));
        message.exec();
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
    }
}

void MusicUserRecordWidget::checkPwdStrength(int code)
{
    m_ui->pwdStrengthT1->setStyleSheet(code != -1 ? MusicUIObject::MBackgroundStyle14 : MusicUIObject::MBackgroundStyle15);
    m_ui->pwdStrengthT2->setStyleSheet(MusicUIObject::MBackgroundStyle15);
    m_ui->pwdStrengthT3->setStyleSheet(MusicUIObject::MBackgroundStyle15);

    switch(code)
    {
        case  1:
            m_ui->pwdStrengthT2->setStyleSheet(MusicUIObject::MBackgroundStyle14);
            break;
        case  2:
            m_ui->pwdStrengthT2->setStyleSheet(MusicUIObject::MBackgroundStyle14);
            m_ui->pwdStrengthT3->setStyleSheet(MusicUIObject::MBackgroundStyle14);
            break;
        default: break;
    }
}

int MusicUserRecordWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
