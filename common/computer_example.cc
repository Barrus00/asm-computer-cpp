#include "computer.h"
#include <array>

// Operator == dla std::array jest constexpr dopiero od C++20.
template<class T, std::size_t N>
constexpr bool compare(std::array<T, N> const& arg1, std::array<T, N> const& arg2) {
    for (size_t i = 0; i < N; ++i)
        if (arg1[i] != arg2[i]) return false;
    return true;
}

using tmpasm_move = Program<
        Mov<Mem<Num<0>>, Num<42>>>;

using tmpasm_jump = Program<
        Inc<Mem<Num<0>>>,
        Jmp<Id("stop")>,
        Inc<Mem<Num<0>>>,
        Label<Id("stop")>>;

using tmpasm_data = Program<
        Inc<Mem<Lea<Id("a")>>>,
        D<Id("a"), Num<0>>,
        D<Id("b"), Num<2>>,
        D<Id("c"), Num<3>>>;

using tmpasm_operations = Program<
        D<Id("a"), Num<4>>,
        D<Id("b"), Num<3>>,
        D<Id("c"), Num<2>>,
        D<Id("d"), Num<1>>,
        Add<Mem<Lea<Id("a")>>, Mem<Lea<Id("c")>>>,
        Sub<Mem<Lea<Id("b")>>, Mem<Lea<Id("d")>>>,
        Mov<Mem<Lea<Id("c")>>, Num<0>>,
        Mov<Mem<Lea<Id("d")>>, Num<0>>>;

using tmpasm_helloworld = Program<
        Mov<Mem<Mem<Num<10>>>, Num<'h'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'e'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'l'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'l'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'o'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<' '>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'w'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'o'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'r'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'l'>>,
        Inc<Mem<Num<10>>>,
        Mov<Mem<Mem<Num<10>>>, Num<'d'>>>;

int main() {
    static_assert(compare(
            Computer<1, int8_t>::boot<tmpasm_move>(),
            std::array<int8_t, 1>({42})),
            "Failed [tmpasp_move].");

    static_assert(compare(
            Computer<1, int>::boot<tmpasm_jump>(),
            std::array<int, 1>({1})),
            "Failed [tmpasp_jump].");

    static_assert(compare(
            Computer<4, uint32_t>::boot<tmpasm_data>(),
            std::array<uint32_t, 4>({1, 2, 3, 0})),
            "Failed [tmpasp_data].");

    static_assert(compare(
            Computer<5, int64_t>::boot<tmpasm_operations>(),
            std::array<int64_t, 5>({6, 2, 0, 0, 0})),
            "Failed [tmpasp_operations].");

    static_assert(compare(
            Computer<11, char>::boot<tmpasm_helloworld>(),
            std::array<char, 11>({'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'})),
            "Failed [tmpasm_helloworld].");
}
