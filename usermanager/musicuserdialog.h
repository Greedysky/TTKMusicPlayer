#ifndef MUSICUSERDIALOG_H
#define MUSICUSERDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicabstractmovedialog.h"

class MusicUserModel;

namespace Ui {
class MusicUserDialog;
}

class MUSIC_USER_EXPORT MusicUserDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicUserDialog(QWidget *parent = 0);
    ~MusicUserDialog();

signals:
    void userLoginSuccess(const QString &name);

public slots:
    void userLogin();
    void checkUserLogin();
    void registerUser();
    void checkRegisterUser();
    void userForgotPasswd();
    void checkUserForgotPasswd();
    void changeVerificationCode();
    void userComboBoxChanged(const QString &index);
    void userEditTextChanged(const QString &index);
    void checkToAutoLogin();
    void buttonClicked(int index);

protected:
    void firstStatckWidget();
    void secondStatckWidget();
    void thirdStatckWidget();
    void clearOriginData();
    void readFromUserConfig();
    void readFromUserSettings();
    void writeToUserConfig();
    void writeToUserSettings();
    void windowRectChanged(int index, const QRect &rect);

    Ui::MusicUserDialog *ui;
    MusicUserModel *m_userModel;
    MStringsListMap m_parameter;
    QString m_userComboIndex;

};

#endif // MUSICUSERDIALOG_H
