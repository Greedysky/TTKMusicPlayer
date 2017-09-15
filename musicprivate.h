#ifndef MUSICPRIVATE_H
#define MUSICPRIVATE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/


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
