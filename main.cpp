#include <iostream>
#include "computer.h"

template<typename ... Arg>
struct JP2GMD {
};

using wrong_instructions = Program<
        Mov<Mem<Num<0>>, Num<42>>,
        JP2GMD<Num<0>>>;

using wrong_arg = Program<
        Mov<Mem<Num<0>>, Num<42>>,
        Jmp<Id("0")>,
        //Add<JP2GMD<int>, Num<32>>,
        //Not<JP2GMD<int>>,
        Label<Id("0")>>;

using tmpasm_move = Program<
        Mov<Mem<Num<0>>, Num<42>>>;

using tmpasm_data = Program<
        Inc<Mem<Lea<Id("c")>>>,
        D<Id("a"), Num<0>>,
        D<Id("b"), Num<2>>,
        D<Id("c"), Num<3>>>;

using back_jump = Program<
        Inc<Mem<Num<0>>>,
        Jmp<Id("0")>,
        Label<Id("stop")>,
        Inc<Mem<Num<0>>>,
        Jmp<Id("break")>,
        Label<Id("0")>,
        Jmp<Id("stop")>,
        Label<Id("break")>>;

using inf_loop = Program<
        Dec<Mem<Num<0>>>,
        Label<Id("stop")>,
        Inc<Mem<Num<0>>>,
        Jmp<Id("stop")>>;

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

using tmpasm_jump = Program<
        Inc<Mem<Num<0>>>,
        Jmp<Id("stop")>,
        Inc<Mem<Num<0>>>,
        Label<Id("stop")>>;

using empty_program = Program<>;

using no_program = Inc<Mem<Num<0>>>;

template<class T, std::size_t N>
constexpr bool compare(std::array<T, N> const& arg1, std::array<T, N> const& arg2) {
    for (size_t i = 0; i < N; ++i)
        if (arg1[i] != arg2[i]) return false;
    return true;
}

int main() {
    static_assert(compare(
            Computer<1, int8_t>::boot<tmpasm_move>(),
            std::array<int8_t, 1>({42})),
                  "Failed [tmpasp_move].");

    static_assert(compare(
            Computer<4, uint32_t>::boot<tmpasm_data>(),
           std::array<uint32_t, 4>({0, 2, 4, 0})),
           "Failed [tmpasp_data].");

    static_assert(compare(
            Computer<11, char>::boot<tmpasm_helloworld>(),
            std::array<char, 11>({'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'})),
                  "Failed [tmpasm_helloworld].");

    static_assert(compare(
            Computer<1, int>::boot<tmpasm_jump>(),
            std::array<int, 1>({1})),
                  "Failed [tmpasp_jump].");

    static_assert(compare(
            Computer<1, int>::boot<back_jump>(),
            std::array<int, 1>({2})),
                  "Failed [tmpasp_jump].");

    using test_machine = Computer<4, bool>;

    using mem_test = Program<D<Id("J"), Num<0>>, D<Id("P"), Num<0>>, D<Id("2"), Num<1>>, D<Id("GMD"), Num<0>>, Inc<Mem<Lea<Id("J")>>>, Dec<Mem<Lea<Id("2")>>>, Inc<Mem<Num<3>>>>;

    static_assert(compare(test_machine::boot<mem_test>(), std::array<bool, 4>{1,0,0, 1}));

    //static_assert(compare(test_machine::boot<wrong_arg>(), std::array<bool, 4>{1,0,0, 1}));

    //Computer<5, uint_fast64_t>::boot<wrong_instructions>();

    constexpr auto X = Computer<1,int>::boot<empty_program>();
    //constexpr auto Y = Computer<1,int>::boot<no_program>();


    //Nie powinno się kompilować bo nieskończona pętla.
    //constexpr std::array<int, 1> X =  Computer<1,int>::boot<inf_loop>();
    //constexpr std::array<bool, 4> X = test_machine::boot<mem_test>();


    return 0;
}
