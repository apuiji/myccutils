#pragma once

#include<string>
#include<utility>

namespace zlt {
  template<class C, C ...S>
  struct ConstrToIntSeq {
    template<size_t N>
    consteval auto operator ()(const C (&s)[N], std::index_sequence<>) {
      return std::integer_sequence<C, S...>();
    }
    template<size_t N, size_t I, size_t ...J>
    consteval auto operator ()(const C (&s)[N], std::index_sequence<I, J...>) {
      constexpr C c = s[I];
      return ConstrToIntSeq<C, S..., c> {}(s, std::index_sequence<J...>());
    }
  };

  template<class C, size_t N>
  static consteval auto constrToIntSeq(const C (&s)[N]) {
    return ConstrToIntSeq<C> {}(s, std::make_index_sequence<N>());
  }

  template<class C, C ...S>
  struct Constr {
    static const std::basic_string<C> value;
  };

  template<class C, C ...S>
  const std::basic_string<C> Constr<C, S...>::value(S...);

  template<class C, C ...S>
  static consteval auto &constr(std::integer_sequence<C, S...>) {
    return Constr<C, S...>::value;
  }

  template<class C, size_t N>
  static consteval auto &constr(const C (&s)[N]) {
    return constr(constrToIntSeq(s));
  }
}
