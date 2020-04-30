// Simple smart pointer class -- Copyright (C) Michael Schwendt
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

#ifndef SMARTPTR_H
#define SMARTPTR_H

typedef unsigned long int ulong;

template <class T> class smartPtrBase {
 public:
    smartPtrBase(T* buffer, ulong bufferLen, bool bufOwner = false) : dummy(0) {
        doFree = bufOwner;
        if ( bufferLen >= 1 ) {
            pBufCurrent = ( bufBegin = buffer );
            bufEnd = bufBegin + bufferLen;
            bufLen = bufferLen;
            status = true;
        }
        else {
            pBufCurrent = ( bufBegin = ( bufEnd = 0 ));
            bufLen = 0;
            status = false;
        }
    }

    virtual ~smartPtrBase() {
        if ( doFree && (bufBegin != 0) ) {
#if defined(_MSC_VER)
            delete[] (void*)bufBegin;
#else
            delete[] bufBegin;
#endif
        }
    }
    
    virtual T* tellBegin()  { return bufBegin; }
    virtual ulong tellLength()  { return bufLen; }
    virtual ulong tellPos()  { return (ulong)(pBufCurrent-bufBegin); }

    virtual bool checkIndex(ulong index) {
        return ((pBufCurrent+index)<bufEnd);
    }
    
    virtual bool reset() {
        if ( bufLen >= 1 ) {
            pBufCurrent = bufBegin;
            return (status = true);
        }
        else {
            return (status = false);
        }
    }

    virtual bool good() {
        return (pBufCurrent<bufEnd);
    }
    
    virtual bool fail() {
        return (pBufCurrent==bufEnd);
    }
    
    virtual void operator++() {
        if ( good() ) {
            pBufCurrent++;
        }
        else {
            status = false;
        }
    }
    
    virtual void operator++(int) {
        if ( good() ) {
            pBufCurrent++;
        }
        else {
            status = false;
        }
    }
    
    virtual void operator--() {
        if ( !fail() ) {
            pBufCurrent--;
        }
        else {
            status = false;
        }
    }
    
    virtual void operator--(int) {
        if ( !fail() ) {
            pBufCurrent--;
        }
        else {
            status = false;
        }
    }
    
    virtual void operator+=(ulong offset) {
        if (checkIndex(offset)) {
            pBufCurrent += offset;
        }
        else {
            status = false;
        }
    }
    
    virtual void operator-=(ulong offset) {
        if ((pBufCurrent-offset) >= bufBegin) {
            pBufCurrent -= offset;
        }
        else {
            status = false;
        }
    }
    
    T operator*() {
        if ( good() ) {
            return *pBufCurrent;
        }
        else {
            status = false;
            return dummy;
        }
    }

    T& operator[](ulong index) {
        if (checkIndex(index)) {
            return pBufCurrent[index];
        }
        else {
            status = false;
            return dummy;
        }
    }

    virtual operator bool()  { return status; }
    
 protected:
    T* bufBegin;
    T* bufEnd;
    T* pBufCurrent;
    ulong bufLen;
    bool status;
    bool doFree;
    T dummy;
};


template <class T> class smartPtr : public smartPtrBase<T> {
 public:
    smartPtr(T* buffer, ulong bufferLen, bool bufOwner = false)
        : smartPtrBase<T>(buffer, bufferLen, bufOwner)
    {
    }
    
    smartPtr()
        : smartPtrBase<T>(0,0)
    {
    }

    void setBuffer(T* buffer, ulong bufferLen) {
        if ( bufferLen >= 1 ) {
            this->pBufCurrent = ( this->bufBegin = buffer );
            this->bufEnd = this->bufBegin + bufferLen;
            this->bufLen = bufferLen;
            this->status = true;
        }
        else {
            this->pBufCurrent = this->bufBegin = this->bufEnd = 0;
            this->bufLen = 0;
            this->status = false;
        }
    }
};

#endif  // SMARTPTR_H
