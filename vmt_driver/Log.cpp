#include "Log.h"
namespace VMTDriver {
    IVRDriverLog* Log::handle = nullptr;

    void Log::Open(IVRDriverLog* hnd)
    {
        handle = hnd;
    }
    void Log::Close()
    {
        handle = nullptr;
    }
    void Log::Output(char* msg)
    {
        if (handle != nullptr) {
            handle->Log(msg);
        }
    }

    int Log::printf(const char* fmt, ...)
    {
        if (handle == nullptr) {
            return -1;
        }

        const int size = 8192;
        char output[size] = { 0 };

        //���O�o��(�󋵂ɍ��킹�Đ؂�ւ�����悤��)
        va_list arg;
        va_start(arg, fmt);
        int ret = vsnprintf(output, size, fmt, arg);
        va_end(arg);

        output[size - 1] = '\0';
        handle->Log(output);

        return ret;
    }
}