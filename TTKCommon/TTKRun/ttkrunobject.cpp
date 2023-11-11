#include "ttkrunobject.h"
#include "ttkversion.h"
#ifdef _WIN32
#  include <windows.h>
#  include <shellapi.h>
#else
#  include <unistd.h>
#endif

static bool strEndWidth(const TTKString &in, const TTKString &out)
{
    if(in.empty() || out.empty() || in.size() < out.size())
    {
        return false;
    }

   return in.compare(in.length() - out.size(), out.size(), out) == 0;
}


void TTKRunObject::run(int argc, char **argv) const
{
    TTKString args;
    for(int i = 0; i < argc; ++i)
    {
        TTKString arg(argv[i]);
        if(!strEndWidth(arg, TTK_APP_EXE_NAME))
        {
            const size_t pos = arg.find('\"');
            if(pos != TTKString::npos)
            {
               arg.insert(arg.begin() + pos, '\\');
            }

            args.append("\"" + arg + "\"");
            args.append(" ");
        }
    }

#ifdef _WIN32
    const char * const path = TTK_STR_CAT(TTK_VERSION_STR, TTK_RSEPARATOR, TTK_SERVICE_EXE_NAME);
    ShellExecuteA(nullptr, "open", path, args.c_str(), nullptr, SW_HIDE);
#else
    const char * const path = TTK_STR_CAT(TTK_VERSION_STR, TTK_SEPARATOR, TTK_SERVICE_SHL_NAME);
    const TTKString &dir = get_current_dir_name();
    system((dir + "/" + path + args).c_str());
#endif
}
