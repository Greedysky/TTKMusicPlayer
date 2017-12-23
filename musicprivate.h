#ifndef MUSICPRIVATE_H
#define MUSICPRIVATE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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


#define MUSIC_DECLARE_PRIVATE(Class) \
    friend class Class##Private; \
    MusicPrivateInterface<Class, Class##Private> music_d;

#define MUSIC_DECLARE_PUBLIC(Class) \
    friend class Class;

#define MUSIC_INIT_PRIVATE \
    music_d.setPublic(this);

#define MUSIC_D(Class) Class##Private *const d = music_d()
#define MUSIC_Q(Class) Class *const q = music_q()

template <typename PUB>
/*! @brief The class of the music private base.
 * @author Greedysky <greedysky@163.com>
 */
class MusicPrivate
{
public:
    virtual ~MusicPrivate() { }
    inline void setPublic(PUB* pub) { music_q_ptr = pub; }

protected:
    inline PUB *music_q() const { return music_q_ptr; }

private:
    PUB* music_q_ptr;

};

template <typename PUB, typename PVT>
/*! @brief The class of the ttk private interface.
 * @author Greedysky <greedysky@163.com>
 */
class MusicPrivateInterface
{
    friend class MusicPrivate<PUB>;
public:
    MusicPrivateInterface() { pvt = new PVT; }
    ~MusicPrivateInterface() { delete pvt; }

    inline void setPublic(PUB* pub) { pvt->setPublic(pub); }
    inline PVT *operator()() const { return static_cast<PVT*>(pvt); }

private:
    MusicPrivateInterface(const MusicPrivateInterface&) { }
    MusicPrivateInterface& operator=(const MusicPrivateInterface&) { }
    MusicPrivate<PUB>* pvt;

};


#endif // MUSICPRIVATE_H
