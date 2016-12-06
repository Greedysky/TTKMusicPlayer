#include "musicuserrecordwidget.h"
#include "ui_musicuserrecordwidget.h"
#include "musicuiobject.h"
#include "musicusermodel.h"
#include "musicmessagebox.h"
#include "musicwidgetutils.h"

#include <QFileDialog>
#include <QCryptographicHash>
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
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicUserRecordWidget::~MusicUserRecordWidget()
{
    delete m_ui;
}

QString MusicUserRecordWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicUserRecordWidget::setUserModel(MusicUserModel *model, const QString &uid)
{
    m_userModel = model;
    m_ui->userIDLabel_F->setText(uid);
    ///first tab
    initTabF();
    ///second tab
    initTabS();
    ///third tab
    initTabT();
}

void MusicUserRecordWidget::initTabF()
{
    QString uid = m_ui->userIDLabel_F->text();
    m_ui->cityComboBox_F->setItemDelegate(new QStyledItemDelegate(m_ui->cityComboBox_F));
    m_ui->cityComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->countryComboBox_F->setItemDelegate(new QStyledItemDelegate(m_ui->countryComboBox_F));
    m_ui->countryComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->cityComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->countryComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->signatureEdit_F->setStyleSheet(MusicUIObject::MTextEditStyle01);
    connect(m_ui->cityComboBox_F, SIGNAL(currentIndexChanged(QString)),
                                SLOT(cityComboBoxIndexChanged(QString)));
    m_ui->cityComboBox_F->addItems(m_userModel->getAllCities());

    m_ui->userTimeLabel_F->setText(m_userModel->getUserLogTime(uid));
    m_ui->nicknameEdit->setText(m_userModel->getUserName(uid));
    m_ui->userIDLabel_F->setText(uid);

    m_ui->nicknameEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->birthDateEdit_F->setStyleSheet(MusicUIObject::MDateEditStyle01);

    QString string(m_userModel->getUserSex(uid));
    m_ui->maleRadioButton_F->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->femaleRadioButton_F->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->maleRadioButton_F->setChecked(string.isEmpty() || string == "0");
    m_ui->femaleRadioButton_F->setChecked(string == "1");

    string = m_userModel->getUserBirthday(uid);
    m_ui->birthDateEdit_F->setDisplayFormat(QString("yyyy-MM-dd"));
    m_ui->birthDateEdit_F->setDate(string.isEmpty() ? QDate::currentDate()
                               : QDate::fromString(string, QString("yyyy-MM-dd")));
    string = m_userModel->getUserCity(uid);
    if(!string.isEmpty())
    {
        MusicUtils::Widget::setComboboxText(m_ui->cityComboBox_F, string);
    }

    string = m_userModel->getUserCountry(uid);
    if(!string.isEmpty())
    {
        MusicUtils::Widget::setComboboxText(m_ui->countryComboBox_F, string);
    }
    m_ui->signatureEdit_F->setText(m_userModel->getUserSignature(uid));
    m_ui->confirmButton_F->setStyleSheet(MusicUIObject::MPushButtonStyle06);

    connect(m_ui->confirmButton_F, SIGNAL(clicked()), SLOT(confirmButtonClickedF()));
}

void MusicUserRecordWidget::initTabS()
{
    QString path = m_userModel->getUserIcon(m_ui->userIDLabel_F->text());
    m_ui->bigPixmapLabel_S->setPixmap(QPixmap(path).scaled(m_ui->bigPixmapLabel_S->size()));
    m_ui->smlPixmapLabel_S->setPixmap(QPixmap(path).scaled(m_ui->smlPixmapLabel_S->size()));
    m_ui->openFileButton_S->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    connect(m_ui->openFileButton_S, SIGNAL(clicked()), SLOT(openFileButtonClickedS()));
}

void MusicUserRecordWidget::initTabT()
{
    m_ui->tab_3->setStyleSheet(MusicUIObject::MLineEditStyle01 + "QWidget{ color:#666666;}");
    m_ui->labelRighT1->hide();
    m_ui->labelRighT2->hide();
    m_ui->labelRighT3->hide();
    m_ui->originPwdEdit_T->setLabel(MusicUserLineEdit::PwdConfirm, nullptr, m_ui->labelRighT1);
    m_ui->newPwdEdit_T->setLabel(MusicUserLineEdit::PasswdNew, nullptr, m_ui->labelRighT2);
    m_ui->newCPwdEdit_T->setLabel(MusicUserLineEdit::PwdConfirm, nullptr, m_ui->labelRighT3);
    connect(m_ui->newPwdEdit_T, SIGNAL(checkPwdStrength(int)), SLOT(checkPwdStrength(int)));

    changeVerificationCodeT();
    m_ui->confirmButton_T->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    connect(m_ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCodeT()));
    connect(m_ui->confirmButton_T, SIGNAL(clicked()), SLOT(confirmButtonClickedT()));
}

void MusicUserRecordWidget::cityComboBoxIndexChanged(const QString &city)
{
    QStringList country = m_userModel->getAllCountries(city);
    m_ui->countryComboBox_F->clear();
    m_ui->countryComboBox_F->addItems(country);
}

void MusicUserRecordWidget::confirmButtonClickedF()
{
    QString nickname(m_ui->nicknameEdit->text());
    if(nickname != m_userModel->getUserName(m_ui->userIDLabel_F->text()))
    {
        emit resetUserName(m_ui->nicknameEdit->text());
    }
    m_userModel->updateUser(m_ui->userIDLabel_F->text(), nickname,
                            m_ui->maleRadioButton_F->isChecked() ? "0" : "1",
                            m_ui->birthDateEdit_F->text(),
                            m_ui->cityComboBox_F->currentText(),
                            m_ui->countryComboBox_F->currentText(),
                            m_ui->signatureEdit_F->toPlainText());
    close();
}

void MusicUserRecordWidget::openFileButtonClickedS()
{
    QString path =  QFileDialog::getOpenFileName(
                              this, QString(), "./", "Images (*.png *.bmp *.jpg)");
    if(path.isEmpty())
    {
        return;
    }
    m_ui->bigPixmapLabel_S->setPixmap(QPixmap( path ).scaled(m_ui->bigPixmapLabel_S->size()));
    m_ui->smlPixmapLabel_S->setPixmap(QPixmap( path ).scaled(m_ui->smlPixmapLabel_S->size()));

    QFile file(path);
    QByteArray name;
    if(file.open(QIODevice::ReadOnly))
    {
        name = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
    }
    path = QString("%1%2").arg(CACHE_DIR_FULL)
                          .arg(QString(name.toHex().toUpper()));
    file.copy( path );
    file.close();

    m_userModel->updateUserIcon(m_ui->userIDLabel_F->text(), path);
    emit userIconChanged(m_ui->userIDLabel_F->text(), path);
}

void MusicUserRecordWidget::changeVerificationCodeT()
{
    m_ui->verificationCode->setCodeCount(6);
    m_ui->verificationCode->setNoisyPointCount(100);
    m_ui->verificationCode->renderPicture();
}

void MusicUserRecordWidget::confirmButtonClickedT()
{
    QString uid = m_ui->userIDLabel_F->text();
    if( m_ui->originPwdEdit_T->getStrStatus() &&
        m_ui->newPwdEdit_T->getStrStatus() &&
        m_ui->newCPwdEdit_T->getStrStatus() )
    {
        if(m_ui->verificationCode->getCheckCode() != m_ui->verificationCodeEdit_T->text().trimmed())
        {
            MusicMessageBox message;
            message.setText(tr("You verificationCode is incorrect"));
            message.exec();
            return;
        }
        if(!m_userModel->passwordCheck(uid, m_ui->originPwdEdit_T->text()))
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
        if(!m_userModel->updateUserPwd(uid, m_ui->newPwdEdit_T->text()))
        {
            MusicMessageBox message;
            message.setText(tr("The modity password failed"));
            message.exec();
            return;
        }

        emit userIconChanged(QString(), QString());
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
    m_ui->pwdStrengthT1->setStyleSheet(code != -1 ? MusicUIObject::MBackgroundStyle14 :
                                                  MusicUIObject::MBackgroundStyle15);
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
    return MusicAbstractMoveDialog::exec();;
}
