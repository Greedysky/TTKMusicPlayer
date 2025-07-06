#include "ttkrunobject.h"
#include "ttkversion.h"

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <shellapi.h>
#else
#  include <unistd.h>
#endif

#ifdef __APPLE__
#  include <mach-o/dyld.h>
#endif

namespace TTK 
{
static bool endWith(const TTKString &value, const TTKString &tail)
{
    if(value.empty() || tail.empty() || value.size() < tail.size())
    {
        return false;
    }

    return value.compare(value.length() - tail.size(), tail.size(), tail) == 0;
}
}


void TTKRunObject::run(int argc, char **argv) const
{
    TTKString args;
    for(int i = 0; i < argc; ++i)
    {
        TTKString arg(argv[i]);
        if(!TTK::endWith(arg, TTK_APP_FILE_NAME))
        {
            const size_t pos = arg.find('\"');
            if(pos != TTKString::npos)
            {
               arg.insert(arg.begin() + pos, '\\');
            }

            args.append(TTK_STR_QUOTES(arg));
            args.append(TTK_SPACE);
        }
    }

    char path[TTK_LOW_BUFFER] = {};
    const char * const suffix = TTK_STR_CAT(TTK_SEPARATOR, TTK_VERSION_STR, TTK_SEPARATOR, TTK_SERVICE_RUN_NAME);
#ifdef _WIN32
    GetModuleFileNameA(nullptr, path, TTK_LOW_BUFFER);

    TTKString filePath = path;
    const size_t pos = filePath.find_last_of('\\');
    if(pos != TTKString::npos)
    {
        filePath.erase(pos);
    }

    ShellExecuteA(nullptr, "open", (filePath + suffix).c_str(), args.c_str(), nullptr, SW_HIDE);
#elif defined __linux__
    TTKString filePath = get_current_dir_name();
    if(readlink("/proc/self/exe", path, sizeof(path) - 1) != 0)
    {
        filePath = path;
        const size_t pos = filePath.find_last_of('/');
        if(pos != TTKString::npos)
        {
            filePath.erase(pos);
        }
    }

    system((filePath + suffix + TTK_SPACE + args).c_str());
#elif defined __APPLE__
    // macOS: get executable path
    TTKString filePath("./");
    uint32_t size = sizeof(path);
    if(_NSGetExecutablePath(path, &size) == 0)
    {
        filePath = path;
        const size_t pos = filePath.find_last_of('/');
        if(pos != TTKString::npos)
        {
            filePath.erase(pos);
        }
    }
    else
    {
        // fallback: use current working directory
        char cwd[TTK_LOW_BUFFER] = {};
        if(getcwd(cwd, sizeof(cwd)))
        {
            filePath = cwd;
        }
    }

    system((filePath + suffix + TTK_SPACE + args).c_str());
#endif
}
