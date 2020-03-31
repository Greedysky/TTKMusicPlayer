#include "file.h"

sS32 file::copy(file &src, sS32 cnt)
{
	sS32 copied=0;
	sS32 maxlen=src.size()-src.tell();
	if (cnt>0 && cnt<maxlen)
		maxlen=cnt;

	sU8 *buf=new sU8[65536];

	while (maxlen)
	{
		sS32 bsize=(maxlen<65536)?maxlen:65536;
		sS32 bread=src.read(buf, bsize);
		sS32 bwritten=write(buf, bread);
		maxlen-=bread;
		copied+=bwritten;

		if (!bread || bwritten!=bread)
			break;
	}
  
  delete[] buf;

	return copied;
}


