#pragma once

#ifdef __WIN32__
#include<libloaderapi.h>
#else
#include<dlfcn.h>
#include<filesystem>
#endif

namespace zlt::dl {
  #ifdef __WIN32__
  using DL = HMODULE;

  #define defRTLD(name) static inline constexpr auto name = 0
  defRTLD(DEEPBIND);
  defRTLD(GLOBAL);
  defRTLD(LAZY);
  defRTLD(LOCAL);
  defRTLD(NODELETE);
  defRTLD(NOLOAD);
  defRTLD(NOW);
  #undef defRTLD

  static inline DL open(const char *file, int flags = 0) {
    return LoadLibraryA(file);
  }

  static inline DL open(const wchar_t *file, int flags = 0) {
    return LoadLibraryW(file);
  }

  template<class C>
  static inline DL open(const std::basic_string<C> &file, int flags = 0) noexcept {
    return open(file.data(), 0);
  }

  static inline int close(DL dl) noexcept {
    FreeLibrary(dl);
    return 0;
  }

  template<class T>
  static inline T *symbol(DL dl, const char *name) noexcept {
    return (T *) GetProcAddress(dl, name);
  }

  template<class R, class ...Args>
  static inline auto funct(DL dl, const char *name) noexcept {
    return (R (*)(Args...)) GetProcAddress(dl, name);
  }
  #else
  using DL = void *;

  #define defRTLD(name) static inline constexpr auto name = RTLD_##name
  defRTLD(DEEPBIND);
  defRTLD(GLOBAL);
  defRTLD(LAZY);
  defRTLD(LOCAL);
  defRTLD(NODELETE);
  defRTLD(NOLOAD);
  defRTLD(NOW);
  #undef defRTLD

  static inline DL open(const char *file, int flags = LAZY | GLOBAL) noexcept {
    return dlopen(file, flags);
  }

  static inline DL open(const wchar_t *file, int flags = LAZY | GLOBAL) noexcept {
    std::filesystem::path path(file);
    return open(path.string().data(), flags);
  }

  template<class C>
  static inline DL open(const std::basic_string<C> &file, int flags = LAZY | GLOBAL) noexcept {
    return open(file.data(), flags);
  }

  static inline int close(DL dl) noexcept {
    return dlclose(dl);
  }

  template<class T>
  static inline T *symbol(DL dl, const char *name) noexcept {
    return (T *) dlsym(dl, name);
  }

  template<class R, class ...Args>
  static inline auto funct(DL dl, const char *name) noexcept {
    return (R (*)(Args...)) dlsym(dl, name);
  }
  #endif

  template<class T>
  static inline auto symbol(DL dl, const std::string &name) noexcept {
    return symbol<T>(dl, name.data());
  }

  template<class R, class ...Args>
  static inline auto funct(DL dl, const std::string &name) noexcept {
    return funct<R, Args...>(dl, name.data());
  }
}
