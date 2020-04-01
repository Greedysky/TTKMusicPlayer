#ifndef __file_h_
#define __file_h_

#include <string.h>
#include "types.h"
#include "stdio_file.h"

#define GETPUTMETHODS(T) virtual T get##T() { T t; read(&t, sizeof(T)); return t; } \
						 virtual sS32 put##T(T t) { return write(&t, sizeof(T)); }

#define GETPUTFUNCS(T)	static inline T get##T(file &f) { return f.get##T(); }; \
						static inline sS32 put##T(file &f, T t) { return f.put##T(t); };

class  file
{
public:
  file()            { }
  virtual ~file()   { }

  virtual void      close()=0;

  virtual sS32      read(void *buf, sS32 cnt)=0;
  virtual sS32      write(const void *buf, sS32 cnt)=0;

  virtual sS32      seek(sS32 pos=0)=0;
  virtual sS32      seekcur(sS32 pos)   { return seek(tell()+pos); }
  virtual sS32      seekend(sS32 pos=0) { return seek(size()+pos); }

  virtual sS32      tell()=0;
  virtual sS32      size()=0;

  virtual file&     operator [](sS32 pos) { seek(pos); return *this; }
  virtual sBool     eof()                 { return tell()==size(); }

  virtual sBool     eread(void *buf, sS32 cnt)  { return read(buf, cnt)==cnt; }
  virtual sBool     ewrite(const void *buf, sS32 cnt) { return write(buf, cnt)==cnt; }

  virtual sS32      copy(file &src, sS32 cnt=-1);

	template<class T> sS32 read(T& buf)   { return read(&buf,sizeof(T)); }
	template<class T> sS32 write(T& buf)  { return write(&buf,sizeof(T)); }

  GETPUTMETHODS(sInt)
  GETPUTMETHODS(sBool)
  GETPUTMETHODS(sS8)
  GETPUTMETHODS(sS16)
  GETPUTMETHODS(sS32)
  GETPUTMETHODS(sS64)
  GETPUTMETHODS(sU8)
  GETPUTMETHODS(sU16)
  GETPUTMETHODS(sU32)
  GETPUTMETHODS(sU64)
  GETPUTMETHODS(sF32)
  GETPUTMETHODS(sF64)

	virtual sS32 puts(const sChar * t) { if (t) return write((void *)t,(sS32)strlen(t)); else return 0; }
};


class fileS: public file
{
private:
    FILE *hnd;

public:
	fileS()   { hnd=NULL; }
  ~fileS()  { close(); }

	enum {
	  rd=0, wr=1, rw=2,
	  ex=0, cr=4, up=8
	};

	virtual sBool     open(const sChar *name, sInt mode=rd|ex);
	virtual sS32      read(void *buf, sS32 cnt);
	virtual sS32      write(const void *buf, sS32 cnt);

    virtual void      close()			      { if (hnd) { stdio_close (hnd); hnd=0; } }

    virtual sS32      seek(sS32 pos)	  { return stdio_seek (hnd, pos, SEEK_SET); }
    virtual sS32      seekcur(sS32 pos) { return stdio_seek (hnd, pos, SEEK_CUR); }
    virtual sS32      seekend(sS32 pos) { return stdio_seek (hnd, pos, SEEK_END); }

    virtual sS32      tell()			      { return stdio_tell (hnd); }
    virtual sS32      size()			      { return stdio_length (hnd); }

	template<class T> sS32 read(T& buf)   { return fread(&buf,1,sizeof(T),hnd); }
	template<class T> sS32 write(T& buf)  { return -1; }
};


class fileA: public file
{
protected:
	file  *fhost;
	sS32  foffset, flen, fpos;
	sBool dhc;

public:
	fileA()
	{
	  fhost=0;
	  foffset=0;
	  flen=0;
	  fpos=0;
	  dhc=0;
	}

  ~fileA()
  {
    close();
  }

	virtual sBool open(file &f, sS32 pos, sS32 len, sBool deleteHostOnClose=sFALSE)
	{
	  sS32 fsize=f.size();
	  fhost=&f;

    pos=(pos<0)?0:(pos>fsize)?fsize:pos;
	  if (len<0)
      len=0;

	  if ((pos+len)>fsize)
      len=fsize-pos;

	  f.seek(pos);
	  foffset=pos;

	  flen=len;
	  dhc=deleteHostOnClose;
	  fpos=0;

	  return (flen) ? sTRUE : sFALSE;
	}

  virtual void close()
	{
	  if (fhost && dhc)
	  {
		  fhost->close();
		  delete fhost;
	  }

	  fhost=0;
	  foffset=0;
	  flen=0;
	  fpos=0;
	}

	virtual sS32 read(void *buf, sS32 cnt)
	{
		if (!buf || !fhost)
			return 0;

		if (cnt<0)
      cnt=0;
		if (cnt>(flen-fpos))
      cnt=flen-fpos;

		cnt=fhost->read(buf,cnt);
		fpos+=cnt;

		return cnt;
	}

	virtual sS32 write(const void *buf, sS32 cnt)
	{
		if (!buf || !fhost)
			return 0;

		if (cnt<0)
      cnt=0;

		if (cnt>(flen-fpos))
      cnt=flen-fpos;

		cnt=fhost->write(buf,cnt);
		fpos+=cnt;

		return cnt;
	}

	virtual sS32 seek(sS32 pos)
	{
		if (pos<0)
      pos=0;

		if (pos>flen)
      pos=flen;

		fhost->seek(foffset+pos);

		return fpos=fhost->tell()-foffset;
	}

	virtual sS32 seekcur(sS32 pos) 
	{
		fhost->seekcur(pos);
		return fpos=fhost->tell()-foffset;
	}

	virtual sS32 tell() { return fpos; }
	virtual sS32 size() { return flen; }

	template<class T> sS32 read(T& buf)   { return read(&buf,sizeof(T)); }
	template<class T> sS32 write(T& buf)  { return write(&buf,sizeof(T)); }
};

#define FILEMTMP_BLOCKSIZE 65536

class fileMTmp: public file
{
protected:
	struct tmblock 
  {
		tmblock()
    {
      next=0; 
    }

		~tmblock() 
		{ 
			if (next)
				delete next; 
		}

		tmblock *next;
		sU8     content[FILEMTMP_BLOCKSIZE];
	};

	tmblock *firstblk, *curblk;
	sS32    bpos, fpos, flen;

public:
	fileMTmp()
	{
		firstblk=0;
	  curblk=0;
		flen=0;
		fpos=0;
		bpos=0;
	}

  ~fileMTmp()
  {
    close();
  }

	virtual sBool open()
	{
		close();
		firstblk=new tmblock;
		seek(0);

		return firstblk ? sTRUE : sFALSE;
	}

	virtual void close()
	{
		if (firstblk)
			delete firstblk;

		firstblk=0;
	  curblk=0;
		flen=0;
		fpos=0;
		bpos=0;
	}

	virtual sS32 read(void *buf, sS32 cnt)
	{
		if (!buf || !curblk)
			return 0;

		if (cnt>(flen-fpos))
      cnt=flen-fpos;

		sU8   *out=(sU8 *) buf;
		sS32  rb=0;

		while (cnt)
		{
			// noch im selben block?
			if (bpos+cnt<FILEMTMP_BLOCKSIZE)
			{
				memcpy(out, curblk->content+bpos, cnt);
				out+=cnt;
				bpos+=cnt;
				rb+=cnt;
				cnt=0;
			}
      else
			{
				// rest vom block übertragen
				sS32 tocopy=FILEMTMP_BLOCKSIZE-bpos;
				memcpy(out, curblk->content+bpos, tocopy);

				out+=tocopy;
				rb+=tocopy;
				cnt-=tocopy;
				bpos=0;
				curblk=curblk->next;

				if (!curblk)
					cnt=0;
			}
		}

		fpos+=rb;
		return rb;
	}

	virtual sS32 write(const void *buf, sS32 cnt)
	{
		if (!buf || !curblk)
			return 0;

		if (cnt<0) 
			cnt=0;

		sU8   *in=(sU8 *) buf;
		sS32  wb=0;

		while (cnt)
		{
			// noch im selben block?
			if (bpos+cnt<FILEMTMP_BLOCKSIZE)
			{
				memcpy(curblk->content+bpos, in, cnt);
				in+=cnt;
				bpos+=cnt;
				wb+=cnt;
				cnt=0;
			}
      else
			{
				// rest vom block übertragen
				sS32 tocopy=FILEMTMP_BLOCKSIZE-bpos;
				memcpy(curblk->content+bpos, in, tocopy);
				in+=tocopy;
				wb+=tocopy;
				cnt-=tocopy;
				bpos=0;
				// nexter block (oder nen neuer)
				if (!curblk->next)
					curblk->next=new tmblock;

				curblk=curblk->next;
				if (!curblk)
					cnt=0;
			}
		}

		fpos+=wb;
		if (fpos>flen) flen=fpos;
		return wb;
	}

	virtual sS32 seek(sS32 pos)
	{
    pos=(pos<0)?0:(pos>flen)?flen:pos;

		sS32 pos2=pos;
		curblk=firstblk;

		while (pos2>=FILEMTMP_BLOCKSIZE && curblk)
		{
			curblk=curblk->next;
			pos2-=FILEMTMP_BLOCKSIZE;
		}
		bpos=pos2;

		return fpos=pos;
	}

	virtual sS32 tell() { return fpos; }
	virtual sS32 size() { return flen; }

	template<class T> sS32 read(T& buf)   { return read(&buf, sizeof(T)); }
	template<class T> sS32 write(T& buf)  { return write(&buf, sizeof(T)); }

};

GETPUTFUNCS(sBool)
GETPUTFUNCS(sInt)
GETPUTFUNCS(sS8)
GETPUTFUNCS(sS16)
GETPUTFUNCS(sS32)
GETPUTFUNCS(sS64)
GETPUTFUNCS(sU8)
GETPUTFUNCS(sU16)
GETPUTFUNCS(sU32)
GETPUTFUNCS(sU64)
GETPUTFUNCS(sF32)
GETPUTFUNCS(sF64)

#define bread(f,v)  (f).eread(&v,sizeof(v))
#define bwrite(f,v) (f).ewrite(&v,sizeof(v))


#endif
