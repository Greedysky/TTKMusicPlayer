#ifndef MUSICPRIVATE_H
#define MUSICPRIVATE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/


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


#endif // MUSICPRIVATE_H
