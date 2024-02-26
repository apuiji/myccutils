#pragma once

#include<string>

namespace zlt {
  template<class C, C ...S>
  struct ConstBasicString {
    static const std::basic_string<C> value;
  };

  template<class C, C ...S>
  const std::basic_string<C> ConstBasicString<C, S...>::value({ S... });

  template<class C, C ...S>
  static inline const std::basic_string<C> &constBasicString = ConstBasicString<C, S...>::value;
  template<char ...S>
  static inline const std::string &constString = constBasicString<char, S...>;
  template<wchar_t ...S>
  static inline const std::wstring &constWstring = constBasicString<wchar_t, S...>;
}
