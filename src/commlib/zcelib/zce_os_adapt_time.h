/*!
* @copyright  2004-2013  Apache License, Version 2.0 FULLSAIL
* @filename   zce_os_adapt_time.h
* @author     Sailzeng <sailerzeng@gmail.com>
* @version
* @date       2011年5月1日
* @brief      时间操作的适配器层，主要还是向LINUX下靠拢
* 
* @details
* 
* @note       timeval在整套库内部使用较多，本来打算彻底用他标识内部的时间，但后来还是整理了自己的Time_Value类，
*             可能是因为最后写代码时，timeval的很多操作不封装写起来还是繁琐。
*/

#ifndef ZCE_LIB_OS_ADAPT_TIME_H_
#define ZCE_LIB_OS_ADAPT_TIME_H_

#include "zce_os_adapt_predefine.h"

//为什么不让我用ACE，卫生棉！，卫生棉！！！！！卫生棉卫生棉卫生棉！！！！！！！！

//一个小时的时间
static const time_t ONE_HOUR_SECONDS      = 3600;
//
static const time_t ONE_QUARTER_SECONDS   = 15 * 60;

static const time_t FIVE_MINUTE_SECONDS   = 5 * 60;

//一天的秒数
static const time_t ONE_DAY_SECONDS       = 86400;

#if defined ZCE_OS_WINDOWS

struct timezone
{
    //minutes west of Greenwich
    int tz_minuteswest;
    // type of DST correction ，夏令时，这个值在LINUX下都不再使用了，就饶了我吧
    //int tz_dsttime;
};

#endif

namespace ZCE_OS
{

/*!
* @brief      非标准函数，得到服务器启动到现在的时间，这个时间是个绝对递增的值，不会调整
* @detail     原来模拟的函数是gethrtime的行为，但其返回单位比较怪，类似得到CPU从启动后到现在的TICK的时间，
*             后来pascal改了改了函数名称，也好行为不一样，不如这样
* @return     const timeval
* @note       WINDOWS (不支持WIN2008的环境上，包括编译) 使用的是GetTickCount 函数，这个函数返回毫秒整数，
*             49天就会溢出，内部为了上一次调用的时间，用了static 变量，又为了保护static，给了锁，，所以你要
*             保证调用频度问题。如果两次调用之间的时间如果过长，超过49天，我无法保证你得到准确的值
*             你老不要49天就只调用一次这个函数呀，那样我保证不了你的TICK的效果，你老至少每天调用一次吧。
*             LINUX 下，测试后发现很多系统没有gethrtime函数，用POSIX的新函数clock_gettime代替。
*/
const timeval  get_uptime(void);

/*!
* @brief      取得当前的时间，（墙上时钟）
* @return     inline int  ==0表示成功
* @param      [out]tv     当前的时间
* @param      [out]tz     时区，内部没有处理夏令时问题，LINUX的系统目前也不支持了
* @note       gettimeofday 这个函数在POSIX中已经被抛弃，但是大量的LINUX还会继续使用，所以暂时留下来，
*             POSIX推荐的clock_gettime好像还没有看见多少人呢用
*/
inline int gettimeofday(struct timeval *tv, struct timezone *tz = NULL);

//
/*!
* @brief      取得当前的时钟
* @return     inline const timeval 将当前时间timeval作为结果返回
*/
inline const timeval gettimeofday();

///时间戳字符串的长度，不包括'\0',输入的字符串必须大于这个长度
static const size_t LEN_OF_TIME_STAMP = 26;

///ISO 时间格式化字符串的长度
///输出字符串精度到日期的字符串长度
static const size_t LEN_OF_ISO_DAY_TIMESTRING  = 10;
///输出字符串精度到秒的字符串长度
static const size_t LEN_OF_ISO_SEC_TIMESTRING  = 19;
///输出字符串精度到微秒的字符串长度
static const size_t LEN_OF_ISO_USEC_TIMESTRING = 26;

///US 米国时间格式字符串的长度
///输出字符串精度到日期的字符串长度
static const size_t LEN_OF_US_DAY_TIMESTRING  = 15;
///输出字符串精度到秒的字符串长度
static const size_t LEN_OF_US_SEC_TIMESTRING  = 24;
///输出字符串精度到微秒的字符串长度
static const size_t LEN_OF_US_USEC_TIMESTRING = 31;

///时间格式化输出的格式类型
enum TIME_STR_FORMAT_TYPE
{
    ///打印本地时间
    TIME_STRFFMT_LOCALTIME      = 0x1,
    ///打印UTC时间
    TIME_STRFFMT_UTCTIME        = 0x2,

    ///用ISO的格式进行时间输出
    TIME_STRFFMT_ISO            = 0x10,
    ///用美国的时间格式进行输出
    TIME_STRFFMT_US             = 0x20,

    ///打印时间精度到到天
    TIME_STRFFMT_PRECISION_DAY  = 0x100,
    ///打印时间精度到到秒
    TIME_STRFFMT_PRECISION_SEC  = 0x200,
    ///打印时间精度到微秒
    TIME_STRFFMT_PRECISION_USEC = 0x400,

};

/*!
* @brief      将参数timeval的值作为的时间格格式化后输出打印出来
*             时间戳打印函数,字符串的输出格式是ISO-8601 format. 例子 2010-09-10 10:03:18.100190
*             输出的时间戳格式为2010-09-10 10:03:18.100190       注意末尾还有一个\0
*                            123456789012345678901234567890
* @return     const char*         得到的时间字符串
* @param[out] str_date_time  字符串
* @param[in]  datetime_strlen 字符串长度
* @note
*/
const char *timestamp (const timeval *timeval,
                       char *str_date_time,
                       size_t datetime_strlen);

/*!
* @brief      得到当前的系统时间字符串输出
* @return     const char*
* @param      str_date_time
* @param      datetime_strlen
* @note
*/
const char *timestamp (char *str_date_time,
                       size_t datetime_strlen);

/*!
* @brief      将参数timeval的值作为的时间格格式化后输出打印出来，可以控制各种格式输出
* @return     const char*
* @param[in]  timeval         得到的时间字符串
* @param[out] str_date_time
* @param[in]  datetime_strlen
* @param      fromat_type     参数清参考@ref TIME_STR_FORMAT_TYPE ，可以用|或，可以输出各种长度。毫秒，各种格式
* @note       时间戳打印格式说明,
*             字符串的输出格式是ISO-8601 format. @ref TIME_STRFFMT_ISO
*             输出的时间戳格式为2010-09-10 10:03:18.100190       注意末尾还有一个\0
*                           123456789012345678901234567890
*             如果是美式时间格式，那么其实是 星期-月-日-年 时：分：秒.微秒   @ref TIME_STRFFMT_US
*             输出的时间戳格式为Fri Aug 24 2002 07:43:05.100190       注意末尾还有一个\0
*                            12345678901234567890123456789012
*/
const char *timestamp_ex(const timeval *timeval,
                         char *str_date_time,
                         size_t datetime_strlen,
                         int fromat_type = ZCE_OS::TIME_STRFFMT_LOCALTIME |
                         ZCE_OS::TIME_STRFFMT_ISO | 
                         ZCE_OS::TIME_STRFFMT_PRECISION_USEC);

/*!
* @brief      计算timeval内部总计是多少毫秒
* @return     uint64_t
* @param      tv  要计算的timeval
*/
uint64_t total_milliseconds(const timeval &tv);

/*!
* @brief      计算timeval内部总计是多少微秒
* @return     uint64_t
* @param      tv  要计算的timeval
*/
uint64_t total_microseconds(const timeval &tv);

//扩展timeval的一些函数，没有之间重载操作符号的原因是避免污染整个空间

/*!
* @brief      返回一个0,0的的timeval的结构，
* @return     const timeval 要计算的timeval
*/
const timeval timeval_zero();

/*!
* @brief      将tv修正为0
* @param      tv  清0的timeval
*/
void timeval_clear(timeval &tv);

/*!
* @brief      比较时间是否一致,
* @return     int     如果一致返回0，left大，返回正数，right大返回负数,
* @param      left    左值
* @param      right   右值
* @note       注意他和timercmp是不一样的功能，timercmp主要是利用最后一个函数参数完成比较内容
*/
int timeval_compare(const timeval &left, const timeval &right);

/*!
* @brief      对两个时间进行相加，将结果返回，非标准函数
* @return     const timeval
* @param      left          左值
* @param      right         右值
* @note
*/
const timeval timeval_add(const timeval &left, const timeval &right);

/*!
* @brief      对两个时间进行想减，将结果返回，非标准函数,safe==true如果小于0，返回0
* @return     const timeval 相减的结果
* @param      left          左
* @param      right         右
* @param      safe          是否进行安全保护，如果进行保护，结果小于0时，返回0
* @note
*/
const timeval timeval_sub(const timeval &left, const timeval &right, bool safe = true);

/*!
* @brief      如果你设置的usec 总长度>1s，我帮你调整，，非标准函数
* @return     void
* @param      tv      调整的timeval
*/
void timeval_adjust(timeval &tv);

/*!
* @brief      检查这个TIMEVALUE是否还有剩余的时间，非标准函数
* @return     bool
* @param      tv
*/
bool timeval_havetime(const timeval &tv);

/*!
* @brief      生成timeval这个结构
* @return     const timeval
* @param      sec           秒
* @param      usec          微秒
*/
const timeval make_timeval(time_t sec, time_t usec);

/*!
* @brief      将类型为std::clock_t值 转换得到timeval这个结构
* @return     const timeval 转换后的timeval结果
* @param      clock_value   进行转换的参数
*/
const timeval make_timeval(std::clock_t clock_value);

/*!
* @brief      将timespec结构转换得到timeval结构
* @return     const timeval 转换后的timeval结果
* @param      timespec_val  进行转换的参数
*/
const timeval make_timeval(const ::timespec *timespec_val);

//WINDOWS API常用的几个参数
#if defined (ZCE_OS_WINDOWS)

/*!
* @brief      将FILETIME的参数视为一个时间（绝对时间 如2013-01-01 01:53:29），转换得到timeval
* @return     const timeval
* @param      file_time
*/
const timeval make_timeval(const FILETIME *file_time);

/*!
* @brief      转换SYSTEMTIME到timeval
* @return     const timeval
* @param      system_time
* @note
*/
const timeval make_timeval(const SYSTEMTIME *system_time);

/*!
* @brief      将FILETIME的参数视为一个时长（相对时间 如25s），转换FILETIME到timeval ，
* @return     const timeval
* @param      file_time
*/
const timeval make_timeval2(const FILETIME *file_time);

#endif

//我整体对timespec不想做太多支持，

/*!
* @brief
* @return     int
* @param      clk_id
* @param      ts
*/
int clock_gettime (clockid_t clk_id, timespec *ts);

/*!
* @brief      计算timespec内部总计是多少毫秒
* @return     uint64_t
* @param      ts
*/
uint64_t total_milliseconds(const timespec &ts);

/*!
* @brief      将timespec 结构，转换得到timeval这个结构
* @return     const ::timespec
* @param      timeval_val
*/
const ::timespec make_timespec(const timeval *timeval_val);

//-------------------------------------------------------------------------------
//休息一下，休息一下

/*!
* @brief      SLEEP 秒
* @return     int     0成功，-1失败
* @param      seconds 休息的秒数
* @note
*/
int sleep (uint32_t seconds);

/*!
* @brief      SLEEP timeval的时间
* @return     int  0成功，-1失败
* @param      tv   休息的timeval时间长度的时间
* @note
*/
int sleep (const timeval &tv);

/*!
* @brief      休息微秒，但实际休息时间，肯定长于这个，呵呵
* @return     int   0成功，-1失败
* @param      usec  微秒时间长度
*/
int usleep (unsigned long usec);

//-------------------------------------------------------------------------------
//重入安全的时间转换函数的函数

/*!
* @brief      得到本地时间的tm结构
* @return     inline struct tm*
* @param      timep
* @param      result
* @note       重入安全
*/
inline struct tm *localtime_r(const time_t *timep, struct tm *result);

/*!
* @brief      根据GM时间，得到tm结构
* @return     inline struct tm*
* @param      timep
* @param      result
* @note
*/
inline struct tm *gmtime_r(const time_t *timep, struct tm *result);

/*!
* @brief      打印TM内部的时间信息
* @return     inline char*
* @param      tm
* @param      buf            输出的字符串长度，长度你自己保证至少26个
* @note       输出字符串的格式是Wed Jan 02 02:03:55 1980\n\0.强烈不建议使用,
*             唉，我实在不知道这样的时间输出有啥用处，完全是为了兼容才写这两个函数，
*/
inline char *asctime_r(const struct tm *tm, char *buf);

/*!
* @brief      打印time_t所表示的时间信息
* @return     inline char* 返回的时间字符串
* @param      timep        时间
* @param      buf          输出的字符串长度，长度你自己保证至少26个
* @note       输出字符串的格式是Wed Jan 02 02:03:55 1980\n\0.强烈不建议使用,
*             唉，我实在不知道这样的时间输出有啥用处，完全是为了兼容才写这两个函数，
*/
inline char *ctime_r(const time_t *timep, char *buf);

//-------------------------------------------------------------------------------

//读取TSC，Read Time-Stamp Counter
uint64_t rdtsc();

//-------------------------------------------------------------------------------
};

//兼容LINUX下的gettimeofday
inline int ZCE_OS::gettimeofday(struct timeval *tv, struct timezone *tz)
{
    //
#if defined ZCE_OS_WINDOWS
    //
    if (tv == NULL && tz == NULL)
    {
        return 0;
    }

    //如果输入了时间
    if (tv)
    {
        //得到系统时间,
        //测试比较::GetSystemTime 比这个函数耗时，从字面理解有点怪异
        FILETIME   tfile;
        ::GetSystemTimeAsFileTime (&tfile);

        ULARGE_INTEGER ui;
        ui.LowPart = tfile.dwLowDateTime;
        ui.HighPart = tfile.dwHighDateTime;

        //The FILETIME structure is a 64-bit value representing the number of
        //100-nanosecond intervals since January 1, 1601.

        //得到time_t部分
        tv->tv_sec = static_cast<long>((ui.QuadPart - 116444736000000000) / 10000000);
        //得到微秒部分，FILETIME存放的是100-nanosecond
        tv->tv_usec = static_cast<long>(((ui.QuadPart - 116444736000000000) % 10000000) / 10);
    }

    //得到时区相关数据
    if (tz)
    {
        //得到时区信息
        TIME_ZONE_INFORMATION tzone;
        DWORD  ret = ::GetTimeZoneInformation(&tzone);

        if (ret == TIME_ZONE_ID_INVALID)
        {
            return -1;
        }

        //
        tz->tz_minuteswest = tzone.Bias;
        //夏令时问题回避，LINUX下已经不处理这个问题了

    }

    return 0;
#endif //

    //LINUX下得到时间
#if defined ZCE_OS_LINUX
    //直接掉用系统的
    return ::gettimeofday(tv, tz);
#endif //

}

inline const timeval ZCE_OS::gettimeofday()
{
    timeval now_time;
    ZCE_OS::gettimeofday(&now_time);
    return now_time;
}

//得到本地时间
inline struct tm *ZCE_OS::localtime_r(const time_t *timep, struct tm *result)
{
#if defined (ZCE_OS_WINDOWS)
    //WINDOWS下使用默认的_s 系列的API
    errno_t convert_err =  ::localtime_s(result, timep);

    if (convert_err)
    {
        return NULL;
    }
    else
    {
        return result;
    }

#endif //#if defined (ZCE_OS_WINDOWS)

#if defined (ZCE_OS_LINUX)
    return ::localtime_r(timep, result);
#endif //#if defined (ZCE_OS_LINUX)
}

//GM时间
inline struct tm *ZCE_OS::gmtime_r(const time_t *timep, struct tm *result)
{
#if defined (ZCE_OS_WINDOWS)
    //WINDOWS下使用默认的_s 系列的API
    errno_t convert_err =  ::gmtime_s(result, timep);

    if (convert_err)
    {
        return NULL;
    }
    else
    {
        return result;
    }

#endif //#if defined (ZCE_OS_WINDOWS)

#if defined (ZCE_OS_LINUX)
    return ::gmtime_r(timep, result);
#endif //#if defined (ZCE_OS_LINUX)
}

//打印TM内部的时间信息
inline char *ZCE_OS::asctime_r(const struct tm *tm_data, char *buf)
{
#if defined (ZCE_OS_WINDOWS)

    //你至少要保证buf里面有26个字符,这种写法实属无奈，因为LINUX的API并没有保证真正的安全，只是解决了重入问题
    //很少有的发现WINDOWS的API好于LINUX
    const size_t I_GUESS_BUF_HAVE_ROOM_FOR_AT_LEAST_26_BYTES = 26;

    //WINDOWS下使用默认的_s 系列的API
    errno_t convert_err =  ::asctime_s(buf, I_GUESS_BUF_HAVE_ROOM_FOR_AT_LEAST_26_BYTES, tm_data);

    if (convert_err)
    {
        return NULL;
    }
    else
    {
        return buf;
    }

#endif //#if defined (ZCE_OS_WINDOWS)

#if defined (ZCE_OS_LINUX)
    return ::asctime_r(tm_data, buf);
#endif //#if defined (ZCE_OS_LINUX)
}

//打印time_t锁标识的时间信息
inline char *ctime_r(const time_t *timep, char *buf)
{
#if defined (ZCE_OS_WINDOWS)

    //你至少要保证buf里面有26个字符,，因为LINUX的API并没有保证真正的安全，只是解决了重入问题
    const size_t I_GUESS_BUF_HAVE_ROOM_FOR_AT_LEAST_26_BYTES = 26;

    //WINDOWS下使用默认的_s 系列的API
    errno_t convert_err =  ::ctime_s(buf, I_GUESS_BUF_HAVE_ROOM_FOR_AT_LEAST_26_BYTES, timep);

    if (convert_err)
    {
        return NULL;
    }
    else
    {
        return buf;
    }

#endif //#if defined (ZCE_OS_WINDOWS)

#if defined (ZCE_OS_LINUX)
    return ::ctime_r(timep, buf);
#endif //#if defined (ZCE_OS_LINUX)
}

#endif //ZCE_LIB_SHARE_MEM_PRE_DEFINE_H_

