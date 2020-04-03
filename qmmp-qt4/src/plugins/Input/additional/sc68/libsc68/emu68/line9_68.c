#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef EMU68_MONOLITIC
# error "EMU68_MONOLITIC should NOT be defined when compiling this file."
#else
# include "lines68.h"
# include "lines/line9.c"
#endif
