#ifdef USE_VULKAN
#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_TYPESAFE_CONVERSION

#ifndef NDEBUG
#define VERIFY(x) assert(x)
#else
#define VERIFY(x) ((void)(x))
#endif

#include <windows.h>

#define ERR_EXIT(err_msg, err_class)                        \
    do                                                      \
    {                                                       \
        MessageBox(nullptr, err_msg, err_class, MB_OK);     \
        exit(1);                                            \
    } while (0)


//need to make a callback for debugging

#endif // USE_VULKAN
