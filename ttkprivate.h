#ifndef TTKPRIVATE_H
#define TTKPRIVATE_H

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


#define TTK_DECLARE_PRIVATE(Class) \
    friend class Class##Private; \
    TTKPrivateInterface<Class, Class##Private> ttk_d;

#define TTK_DECLARE_PUBLIC(Class) \
    friend class Class;

#define TTK_INIT_PRIVATE \
    ttk_d.setPublic(this);

#define TTK_D(Class) Class##Private *const d = ttk_d()
#define TTK_Q(Class) Class *const q = ttk_q()

template <typename PUB>
/*! @brief The class of the ttk private base.
 * @author Greedysky <greedysky@163.com>
 */
class TTKPrivate
{
public:
    virtual ~TTKPrivate() { }
    inline void setPublic(PUB* pub) { ttk_q_ptr = pub; }

protected:
    inline PUB *ttk_q() const { return ttk_q_ptr; }

private:
    PUB* ttk_q_ptr;

};

template <typename PUB, typename PVT>
/*! @brief The class of the ttk private interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTKPrivateInterface
{
    friend class TTKPrivate<PUB>;
public:
    TTKPrivateInterface() { pvt = new PVT; }
    ~TTKPrivateInterface() { delete pvt; }

    inline void setPublic(PUB* pub) { pvt->setPublic(pub); }
    inline PVT *operator()() const { return static_cast<PVT*>(pvt); }

private:
    TTKPrivateInterface(const TTKPrivateInterface&) { }
    TTKPrivateInterface& operator=(const TTKPrivateInterface&) { }
    TTKPrivate<PUB>* pvt;

};


#endif // TTKPRIVATE_H
