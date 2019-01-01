#ifndef MUSICUSERLINEEDIT_H
#define MUSICUSERLINEEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

/*! @brief The class of the user lineedit.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserLineEdit : public QLineEdit
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicUserLineEdit)
public:
    enum LabelType
    {
        User,           /*!< label type is user*/
        Passwd,         /*!< label type is password*/
        PasswdNew,      /*!< label type is new password*/
        PasswdConfirm,  /*!< label type is confirm password*/
        Mail            /*!< label type is mail*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicUserLineEdit(QWidget *parent = nullptr);

    /*!
     * Set label and label type.
     */
    void setLabel(LabelType ty, QLabel *t, QLabel *s);
    /*!
     * Check the mail type valid.
     */
    inline bool getMailStatus() const { return m_mailContains;}
    /*!
     * Check the string type valid.
     */
    inline bool getStrStatus() const { return m_strContains;}

Q_SIGNALS:
    /*!
     * Check the new password strength.
     * -1 empty string, 0 weak, 1 middle, 2 strong
     */
    void checkPwdStrength(int code);

protected Q_SLOTS:
    /*!
     * Check the new password strength.
     */
    void checkPwdStrength();

protected:
    /*!
     * Override the widget event.
     */
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    /*!
     * Check the input valied when focus out.
     */
    void checkTheInput();
    /*!
     * Show mail label.
     */
    void showLabel();
    /*!
     * Show string label.
     */
    void showLabel(int s, int e);
    /*!
     * Show label info that means you input is valid or not.
     */
    void labelCheck(bool check);

    QLabel *m_tipsLabel;
    QLabel *m_statusLabel;
    LabelType m_type;
    bool m_mailContains;
    bool m_strContains;

};

#endif // MUSICUSERLINEEDIT_H
