#ifndef FILTER_H
#define FILTER_H

#include <new>
#include <math.h>

//////////////////////////////////////////////////////////////////////////

#ifndef FILTER_ALLOC
#define FILTER_ALLOC filter::alloc_t
#endif
//////////////////////////////////////////////////////////////////////////

namespace filter
{
    struct alloc_t
    {
        static void* alloc(unsigned int size) { return ::malloc(size); }
        static void  free(void* ptr)         { ::free(ptr); }
    };

    template <typename TypeT, typename AllocT = FILTER_ALLOC>
    class auto_t
    {
    protected:
        TypeT* _point;
        long _size;

    public:
        auto_t() : _point(0), _size(0) {}
        auto_t(long size) :
          _point(::new (AllocT::alloc(sizeof(TypeT) * size)) TypeT[size])
          , _size(size) {}
        virtual ~auto_t()
        {
            if (_point)
            {
                for (long i = 0; i < _size; ++i) _point[i].~TypeT();
                AllocT::free(_point);
            }
        }

        void set(long size)
        {
            this->~auto_t();
            ::new (this) auto_t(size);
        }

        long size() { return _size; }

        TypeT& operator[](long num) { return _point[num]; }
    };

    //////////////////////////////////////////////////////////////////////////

    class bitmap_t
    {
    public:
        typedef unsigned char channel_t;

        struct pixel_t
        {
          channel_t r, g, b;
          pixel_t() : r(0), g(0), b(0) {}
        };

        struct buff_t
        {
          double r, g, b;
          buff_t() : r(0.0), g(0.0), b(0.0) {}
        };

    protected:
        pixel_t* _bits;
        long _w, _h, _size;

    public:
        bitmap_t() : _bits(0), _w(0), _h(0), _size(0) {}

        void set(pixel_t* bits, long w, long h)
        {
          _bits = bits;
          _w = w;
          _h = h;
          _size = 0;
        }

        long w() { return _w; }
        long h() { return _h; }

        long size()
        {
          if (_size) return _size;
          else       return _size = _w * _h;
        }

        pixel_t& operator[](long num) { return _bits[num]; }
    };

    class buffer_t : public auto_t<bitmap_t::buff_t>
    {
    public:
        buffer_t() {}
        buffer_t(bitmap_t& bitmap) :
          auto_t<bitmap_t::buff_t>(bitmap.size()) {}

        void set(bitmap_t& bitmap)
        {
          ::new (this) buffer_t(bitmap);
        }
    };

    class filter_t : public auto_t<double>
    {
    protected:
        long _radius;

    public:
        filter_t() {}
        filter_t(long radius, long size) :
          auto_t<double>(size), _radius(radius) {}

        void set(long radius, long size)
        {
          ::new (this) filter_t(radius, size);
        }

        long radius() { return _radius; }
    };

    static const double PI = 3.141592653589793;

    //////////////////////////////////////////////////////////////////////////

    template <typename T1, typename T2>
    T1 Clamp(T2 n) { return (T1)(n > (T1)~0 ? (T1)~0 : n); }

    template <typename T>
    T Diamet(T r) { return ((r * 2) + 1); }

    template <typename T>
    bool Equal(T n1, T n2) { return (fabs(n1 - n2) < (T)0.000001); }

    template <typename T>
    T Edge(T i, T x, T w)
    {
        T i_k = x + i;
        if (i_k < 0)  i_k = -x;
        else if (i_k >= w) i_k = w - 1 - x;
        else               i_k = i;
        return i_k;
    }

    void Normalization(filter_t& kernel)
    {
        double sum = 0.0;
        for (int n = 0; n < kernel.size(); ++n)
            sum += kernel[n];
        if (Equal(sum, 1.0)) return;
        for (int n = 0; n < kernel.size(); ++n)
            kernel[n] = kernel[n] / sum;
    }

    //////////////////////////////////////////////////////////////////////////

    void Blur1D(bitmap_t& bitmap, filter_t& kernel)
    {
        Normalization(kernel);

        buffer_t buff(bitmap);

        for (long inx = 0, y = 0; y < bitmap.h(); ++y)
        {
          for (long x = 0; x < bitmap.w(); ++x, ++inx)
          {
            for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
            {
              long i_k = Edge(i, x, bitmap.w());
              long inx_k = inx + i_k;
              buff[inx].r += bitmap[inx_k].r * kernel[n];
              buff[inx].g += bitmap[inx_k].g * kernel[n];
              buff[inx].b += bitmap[inx_k].b * kernel[n];
            }
          }
        }

        for (long inx = 0, x = 0; x < bitmap.w(); ++x)
        {
          for (long y = 0; y < bitmap.h(); ++y)
          {
            inx = y * bitmap.w() + x;
            double r = 0.0, g = 0.0, b = 0.0;
            for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
            {
              long i_k = Edge(i, y, bitmap.h());
              long inx_k = inx + i_k * bitmap.w();
              r += buff[inx_k].r * kernel[n];
              g += buff[inx_k].g * kernel[n];
              b += buff[inx_k].b * kernel[n];
            }
            bitmap[inx].r = Clamp<bitmap_t::channel_t>(r);
            bitmap[inx].g = Clamp<bitmap_t::channel_t>(g);
            bitmap[inx].b = Clamp<bitmap_t::channel_t>(b);
          }
        }
    }

    void Blur2D(bitmap_t& bitmap, filter_t& kernel)
    {
        filter_t matrix(kernel.radius(), kernel.size() * kernel.size());
        for (long n = 0, i = 0; i < kernel.size(); ++i)
          for (long j = 0; j < kernel.size(); ++j, ++n)
            matrix[n] = kernel[i] * kernel[j];

        Normalization(matrix);

        for (long inx = 0, y = 0; y < bitmap.h(); ++y)
        {
          for (long x = 0; x < bitmap.w(); ++x, ++inx)
          {
            double r = 0.0, g = 0.0, b = 0.0;
            for (long n = 0, j = -matrix.radius(); j <= matrix.radius(); ++j)
            {
              long j_k = Edge(j, y, bitmap.h());
              for (long i = -matrix.radius(); i <= matrix.radius(); ++i, ++n)
              {
                long i_k = Edge(i, x, bitmap.w());
                long inx_k = inx + j_k * bitmap.w() + i_k;
                r += bitmap[inx_k].r * matrix[n];
                g += bitmap[inx_k].g * matrix[n];
                b += bitmap[inx_k].b * matrix[n];
              }
            }
            bitmap[inx].r = Clamp<bitmap_t::channel_t>(r);
            bitmap[inx].g = Clamp<bitmap_t::channel_t>(g);
            bitmap[inx].b = Clamp<bitmap_t::channel_t>(b);
          }
        }
    }

    //////////////////////////////////////////////////////////////////////////

    void Average(filter_t& kernel, long radius)
    {
        kernel.set(radius, Diamet(radius));

        double average = 1.0 / (double)kernel.size();

        for (long n = 0; n < kernel.size(); ++n)
            kernel[n] = average;
    }

    void Linear(filter_t& kernel, long radius)
    {
        kernel.set(radius, Diamet(radius));

        double b = 2.0 / (double)kernel.size();
        double a = -(b / radius);

        for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
            kernel[n] = a * abs(i) + b;
    }

    void Gauss(filter_t& kernel, long radius)
    {
        kernel.set(radius, Diamet(radius));

        static const double SQRT2PI = sqrt(2.0 * PI);

        double sigma = (double)radius / 3.0;
        double sigma2 = 2.0 * sigma * sigma;
        double sigmap = sigma * SQRT2PI;

        for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
            kernel[n] = exp(-(double)(i * i) / sigma2) / sigmap;
    }

    void Gauss2(filter_t& kernel, long radius)
    {
        long diamet = Diamet(radius);           // (r * 2) + 1
        kernel.set(radius, diamet * diamet);    // kernel size is d * d

        double sigma = (double)radius / 3.0;
        double sigma2 = 2.0 * sigma * sigma;
        double sigmap = sigma2 * PI;

        for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i)
        {
          long i2 = i * i;
          for (long j = -kernel.radius(); j <= kernel.radius(); ++j, ++n)
            kernel[n] = exp(-(double)(i2 + j * j) / sigma2) / sigmap;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    typedef void(*mark_t)(filter_t&, long);
    typedef void(*blur_t)(bitmap_t&, filter_t&);

    bool Filter(mark_t mark, blur_t blur, bitmap_t& bitmap, long radius)
    {
        if (radius < 1) return false;

        filter_t kernel;
        mark(kernel, radius);
        blur(bitmap, kernel);

        return true;
    }

    struct pair_t
    {
        mark_t mark;
        blur_t blur;
    };

    bool Filter(pair_t& pair, bitmap_t& bitmap, long radius)
    {
        return Filter(pair.mark, pair.blur, bitmap, radius);
    }
}

#endif //FILTER_H
