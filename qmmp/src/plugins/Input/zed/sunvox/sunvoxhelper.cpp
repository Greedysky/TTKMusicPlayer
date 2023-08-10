#include "sunvoxhelper.h"

#define SV_INIT_FLAG_NO_DEBUG_OUTPUT 		( 1 << 0 )
#define SV_INIT_FLAG_USER_AUDIO_CALLBACK 	( 1 << 1 ) /* Offline mode: */
                               /* system-dependent audio stream will not be created; */
                               /* user calls sv_audio_callback() to get the next piece of sound stream */
#define SV_INIT_FLAG_OFFLINE			( 1 << 1 ) /* Same as SV_INIT_FLAG_USER_AUDIO_CALLBACK */
#define SV_INIT_FLAG_AUDIO_INT16 		( 1 << 2 ) /* Desired sample type of the output sound stream : int16_t */
#define SV_INIT_FLAG_AUDIO_FLOAT32 		( 1 << 3 ) /* Desired sample type of the output sound stream : float */
                               /* The actual sample type may be different, if SV_INIT_FLAG_USER_AUDIO_CALLBACK is not set */
#define SV_INIT_FLAG_ONE_THREAD			( 1 << 4 ) /* Audio callback and song modification are in single thread */
                               /* Use it with SV_INIT_FLAG_USER_AUDIO_CALLBACK only */

#define SV_GET_MODULE_XY(in_xy, out_x, out_y) \
    out_x = in_xy & 0xFFFF; \
    if(out_x & 0x8000) \
    out_x -= 0x10000; \
    out_y = ( in_xy >> 16 ) & 0xFFFF; \
    if(out_y & 0x8000) \
    out_y -= 0x10000;

#define SV_GET_MODULE_FINETUNE(in_finetune, out_finetune, out_relative_note) \
    out_finetune = in_finetune & 0xFFFF; \
    if(out_finetune & 0x8000) \
    out_finetune -= 0x10000; \
    out_relative_note = (in_finetune >> 16) & 0xFFFF; \
    if(out_relative_note & 0x8000) \
    out_relative_note -= 0x10000;

#ifdef Q_OS_WIN
#  ifdef __GNUC__
#    define SUNVOX_CALL __attribute__((stdcall))
#  else
#    define SUNVOX_CALL __stdcall
#  endif
#  define LIBRARY_NAME  Qmmp::pluginPath() + "/../sunvox.dll"
#else
#  define SUNVOX_CALL
#  define LIBRARY_NAME  "sunvox.so"
#endif
#define LIBRARY_CNAME Qmmp::pluginPath() + "/../sunvox.bak"

typedef int (SUNVOX_CALL *_sv_audio_callback)(void* buf, int frames, int latency, uint32_t out_time);
typedef int (SUNVOX_CALL *_sv_init)(const char *config, int freq, int channels, uint32_t flags);
typedef int (SUNVOX_CALL *_sv_open_slot)(int slot);
typedef int (SUNVOX_CALL *_sv_load)(int slot, const char *name);
typedef int (SUNVOX_CALL *_sv_volume)(int slot, int vol);
typedef int (SUNVOX_CALL *_sv_play)(int slot);
typedef int (SUNVOX_CALL *_sv_stop)(int slot);
typedef int (SUNVOX_CALL *_sv_close_slot)(int slot);
typedef int (SUNVOX_CALL *_sv_deinit)(void);
typedef int (SUNVOX_CALL *_sv_rewind)(int slot, int t);
typedef int (SUNVOX_CALL *_sv_end_of_song)(int slot);
typedef int (SUNVOX_CALL *_sv_set_autostop)(int slot, int autostop);
typedef uint32_t (SUNVOX_CALL *_sv_get_ticks)(void);
typedef uint32_t (SUNVOX_CALL *_sv_get_song_length_frames)(int slot);
typedef uint32_t (SUNVOX_CALL *_sv_get_song_length_lines)(int slot);
typedef const char* (SUNVOX_CALL *_sv_get_song_name)(int slot);
typedef int (SUNVOX_CALL *_sv_get_song_bpm)(int slot);
typedef int (SUNVOX_CALL *_sv_get_song_tpl)(int slot);
typedef int (SUNVOX_CALL *_sv_get_number_of_modules)(int slot);
typedef const char* (SUNVOX_CALL *_sv_get_module_name)(int slot, int mod_num);
typedef uint32_t (SUNVOX_CALL *_sv_get_module_xy)(int slot, int mod_num);
typedef int (SUNVOX_CALL *_sv_get_module_color)(int slot, int mod_num);
typedef uint32_t (SUNVOX_CALL *_sv_get_module_finetune)(int slot, int mod_num);
typedef int (SUNVOX_CALL *_sv_get_number_of_module_ctls)(int slot, int mod_num);
typedef const char* (SUNVOX_CALL *_sv_get_module_ctl_name)(int slot, int mod_num, int ctl_num);
typedef int (SUNVOX_CALL *_sv_get_number_of_patterns)(int slot);
typedef const char* (SUNVOX_CALL *_sv_get_pattern_name)(int slot, int pat_num);
typedef int (SUNVOX_CALL *_sv_get_pattern_x)(int slot, int pat_num);
typedef int (SUNVOX_CALL *_sv_get_pattern_y)(int slot, int pat_num);
typedef int (SUNVOX_CALL *_sv_get_pattern_tracks)(int slot, int pat_num);
typedef int (SUNVOX_CALL *_sv_get_pattern_lines)(int slot, int pat_num);
//
#define CREATE_SV_MODULE(Module) ((_##Module)m_instance->resolve(#Module))
//
#define sv_audio_callback            CREATE_SV_MODULE(sv_audio_callback)
#define sv_init                      CREATE_SV_MODULE(sv_init)
#define sv_open_slot                 CREATE_SV_MODULE(sv_open_slot)
#define sv_load                      CREATE_SV_MODULE(sv_load)
#define sv_volume                    CREATE_SV_MODULE(sv_volume)
#define sv_play                      CREATE_SV_MODULE(sv_play)
#define sv_stop                      CREATE_SV_MODULE(sv_stop)
#define sv_close_slot                CREATE_SV_MODULE(sv_close_slot)
#define sv_deinit                    CREATE_SV_MODULE(sv_deinit)
#define sv_rewind                    CREATE_SV_MODULE(sv_rewind)
#define sv_end_of_song               CREATE_SV_MODULE(sv_end_of_song)
#define sv_set_autostop              CREATE_SV_MODULE(sv_set_autostop)
#define sv_get_ticks                 CREATE_SV_MODULE(sv_get_ticks)
#define sv_get_song_length_frames    CREATE_SV_MODULE(sv_get_song_length_frames)
#define sv_get_song_length_lines     CREATE_SV_MODULE(sv_get_song_length_lines)
#define sv_get_song_name             CREATE_SV_MODULE(sv_get_song_name)
#define sv_get_song_bpm              CREATE_SV_MODULE(sv_get_song_bpm)
#define sv_get_song_tpl              CREATE_SV_MODULE(sv_get_song_tpl)
#define sv_get_number_of_modules     CREATE_SV_MODULE(sv_get_number_of_modules)
#define sv_get_module_name           CREATE_SV_MODULE(sv_get_module_name)
#define sv_get_module_xy             CREATE_SV_MODULE(sv_get_module_xy)
#define sv_get_module_color          CREATE_SV_MODULE(sv_get_module_color)
#define sv_get_module_finetune       CREATE_SV_MODULE(sv_get_module_finetune)
#define sv_get_number_of_module_ctls CREATE_SV_MODULE(sv_get_number_of_module_ctls)
#define sv_get_module_ctl_name       CREATE_SV_MODULE(sv_get_module_ctl_name)
#define sv_get_number_of_patterns    CREATE_SV_MODULE(sv_get_number_of_patterns)
#define sv_get_pattern_name          CREATE_SV_MODULE(sv_get_pattern_name)
#define sv_get_pattern_x             CREATE_SV_MODULE(sv_get_pattern_x)
#define sv_get_pattern_y             CREATE_SV_MODULE(sv_get_pattern_y)
#define sv_get_pattern_tracks        CREATE_SV_MODULE(sv_get_pattern_tracks)
#define sv_get_pattern_lines         CREATE_SV_MODULE(sv_get_pattern_lines)

#define SV_SLOT 0
#define INPUT_BUFFER_SIZE   1024

SunVoxHelper::SunVoxHelper(const QString &path)
   : m_path(path)
{
    m_instance = new QLibrary;
}

SunVoxHelper::~SunVoxHelper()
{
    deinit();
    delete m_instance;
}

void SunVoxHelper::deinit()
{
    if(!m_instance->isLoaded())
    {
        return;
    }

    sv_stop(SV_SLOT);
    sv_close_slot(SV_SLOT);

#if defined Q_OS_WIN && defined __GNUC__
    sv_deinit();
#endif
    m_instance->unload();

    if(m_copyMode)
    {
        QFile::remove(LIBRARY_CNAME);
    }
}

bool SunVoxHelper::initialize(bool copy)
{
    QString library = LIBRARY_NAME;
    m_copyMode = copy;

    if(m_copyMode)
    {
        if(!QFile::exists(LIBRARY_CNAME))
        {
            QFile::copy(LIBRARY_NAME, LIBRARY_CNAME);
        }
        library = LIBRARY_CNAME;
    }

    m_instance->setFileName(library);
    if(!m_instance->load())
    {
        qWarning("SunVoxHelper: load plugin failed");
        return false;
    }

    const int flag = SV_INIT_FLAG_OFFLINE | SV_INIT_FLAG_USER_AUDIO_CALLBACK | SV_INIT_FLAG_ONE_THREAD | SV_INIT_FLAG_AUDIO_INT16;
    if(sv_init(0, 44100, 2, flag) < 0)
    {
        qWarning("SunVoxHelper: init failed");
        return false;
    }

    sv_open_slot(SV_SLOT);
    sv_load(SV_SLOT, QmmpPrintable(m_path));
    sv_set_autostop(SV_SLOT, 1);
    sv_volume(SV_SLOT, 256);
    sv_play(SV_SLOT);

    m_title = sv_get_song_name(SV_SLOT);
    m_beatsPerMinute = sv_get_song_bpm(SV_SLOT);
    m_ticksPerLine = sv_get_song_tpl(SV_SLOT);
    m_moduleCount = sv_get_number_of_modules(SV_SLOT);
    m_patternCount = sv_get_number_of_patterns(SV_SLOT);

    for(int i = 0; i < m_moduleCount; ++i)
    {
        const int color = sv_get_module_color(SV_SLOT, i);
        const int xy = sv_get_module_xy(SV_SLOT, i);
        const int ft = sv_get_module_finetune(SV_SLOT, i);

        int x, y, finetune, relnote;
        SV_GET_MODULE_XY(xy, x, y);
        SV_GET_MODULE_FINETUNE(ft, finetune, relnote);
        m_modules += QString("Module %1: %2; (x=%3, y=%4); color=(%5, %6, %7); finetune=%8; rel.note=%9;\n")
                    .arg(i + 1)
                    .arg(sv_get_module_name(SV_SLOT, i))
                    .arg(x)
                    .arg(y)
                    .arg(((color) & 0xff))
                    .arg(((color >> 8) & 0xff))
                    .arg(((color >> 16) & 0xff))
                    .arg(finetune)
                    .arg(relnote);

        QStringList v;
        const int ctls = sv_get_number_of_module_ctls(SV_SLOT, i);
        for(int j = 0; j < ctls; ++j)
        {
            v << sv_get_module_ctl_name(SV_SLOT, i, j);
        }

        m_modules += QString("Module ctls: %1; names: %2\n\n")
                    .arg(sv_get_number_of_module_ctls(SV_SLOT, i))
                    .arg(v.join(","));
    }

    for(int i = 0; i < m_patternCount; ++i)
    {
        m_patterns += QString("Pattern %1: %2; (x=%3, y=%4); tracks=%5; lines=%6;\n")
                     .arg(i + 1)
                     .arg(sv_get_pattern_name(SV_SLOT, i))
                     .arg(sv_get_pattern_x(SV_SLOT, i))
                     .arg(sv_get_pattern_y(SV_SLOT, i))
                     .arg(sv_get_pattern_tracks(SV_SLOT, i))
                     .arg(sv_get_pattern_lines(SV_SLOT, i));
    }
    return true;
}

void SunVoxHelper::seek(qint64 time)
{
    const qint64 t = totalTime();
    const qint64 l = sv_get_song_length_lines(SV_SLOT);
    if(t != 0 && l != 0)
    {
        sv_rewind(0, time * 1.0 / t * l);
    }
}

qint64 SunVoxHelper::totalTime() const
{
    return sv_get_song_length_frames(SV_SLOT) / sampleRate() * 1000;
}

qint64 SunVoxHelper::read(unsigned char *data, qint64)
{
    if(sv_end_of_song(SV_SLOT))
    {
        return 0;
    }

    return sv_audio_callback(data, INPUT_BUFFER_SIZE, 0, sv_get_ticks()) ? INPUT_BUFFER_SIZE * 4 : 0;
}
