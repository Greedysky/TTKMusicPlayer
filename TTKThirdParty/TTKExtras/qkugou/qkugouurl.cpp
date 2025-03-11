#include "qkugouurl.h"

QString QKugouUrl::makeSongYuekuUrl() noexcept
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/default.html?ver=10053";
}

QString QKugouUrl::makeSongRecommendUrl() noexcept
{
    return "http://www2.kugou.kugou.com/yueku/v9/html/home.html?ver=10053";
}

QString QKugouUrl::makeRadioPublicUrl() noexcept
{
    return "http://www2.kugou.kugou.com/mv/v9/html/mvradio.html?ver=10053";
}

QString QKugouUrl::makeMovieRecommendUrl() noexcept
{
    return "http://www2.kugou.kugou.com/mv/v9/mtv/index/getData.js?cdn=cdn&ver=10053";
}

QString QKugouUrl::makeKuiSheUrl() noexcept
{
    return "http://huodong.kugou.com/2014/html/kuishe.html?ver=10053";
}
