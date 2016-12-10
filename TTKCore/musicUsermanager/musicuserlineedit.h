#ifndef MUSICUSERLINEEDIT_H
#define MUSICUSERLINEEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include <QLabel>

#include "musicglobaldefine.h"

/*! @brief The class of the user lineedit.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    enum LabelType
    {
        User,       ///*label type is user*/
        Passwd,     ///*label type is password*/
        PasswdNew,  ///*label type is new password*/
        PwdConfirm, ///*label type is confirm password*/
        Mail        ///*label type is mail*/
    };

    explicit MusicUserLineEdit(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setLabel(LabelType ty, QLabel *t, QLabel *s);
    /*!
     * Set label and label type.
     */
    inline bool getMailStatus() const { return m_mailContains;}
    /*!
     * Check the mail type valid.
     */
    inline bool getStrStatus() const { return m_strContains;}
    /*!
     * Check the string type valid.
     */

Q_SIGNALS:
    void checkPwdStrength(int code);
    /*!
     * Check the new password strength.
     * -1 empty string, 0 weak, 1 middle, 2 strong
     */

protected Q_SLOTS:
    void checkPwdStrength();
    /*!
     * Check the new password strength.
     */

protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    /*!
     * Override the widget event.
     */
    void checkTheInput();
    /*!
     * Check the input valied when focus out.
     */
    void showLabel();
    /*!
     * Show mail label.
     */
    void showLabel(int s, int e);
    /*!
     * Show string label.
     */
    void labelCheck(bool check);
    /*!
     * Show label info that means you input is valid or not.
     */

    QLabel *m_tipsLabel;
    QLabel *m_statusLabel;
    LabelType m_type;
    bool m_mailContains;
    bool m_strContains;

};

#endif // MUSICUSERLINEEDIT_H
