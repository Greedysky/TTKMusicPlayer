/***************************************************************************
 *   Copyright (C) 2008 by Deryabin Andrew                                 *
 *   andrew@it-optima.ru                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef AYFLYSTRING_H_
#define AYFLYSTRING_H_

//minimal string class for ayfly library (unix and windows builds)

class CayflyString
{
public:
    CayflyString();
    ~CayflyString();
    CayflyString(const AY_CHAR *str);
    CayflyString(const CayflyString &str);
#ifdef AYFLY_UNICODE
    CayflyString(const char *str);
#endif
    CayflyString& operator =(const CayflyString &str);
    CayflyString& operator =(const AY_CHAR *str);
#ifdef AYFLY_UNICODE
    CayflyString& operator =(const char *str);
#endif
    size_t length();
    const AY_CHAR *c_str() const;
    void toLower();
    int compare(const CayflyString &str);
    int compare(const AY_CHAR *str);
    int rcompare(const CayflyString &str);
    int rcompare(const AY_CHAR *str);
    
private:
    AY_CHAR *m_str;
};

#endif /* AYFLYSTRING_H_ */
