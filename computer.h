#ifndef ZADANIE4_COMPUTER_H
#define ZADANIE4_COMPUTER_H

#include <array>
#include <cstdint>
#include <stdexcept>
#include <type_traits>


namespace header_private {

    using id_t = uint_fast32_t;

    // identificator not possible to acquire
    // through `Id` function calls.
    constexpr id_t empty_id = 0;

    constexpr bool is_lower(const char c) {
        return 'a' <= c and c <= 'z';
    }

    constexpr bool is_upper(const char c) {
        return 'A' <= c and c <= 'Z';
    }

    constexpr bool is_digit(const char c) {
        return '0' <= c and c <= '9';
    }

    constexpr bool is_proper_char(const char c) {
        return is_lower(c) or is_upper(c) or is_digit(c);
    }

    constexpr int get_val(const char c) {
        if (is_lower(c)) {
            return (c - 'a') + 1;
        }
        else if (is_upper(c)) {
            return (c - 'A') + 1;
        }
        else if (is_digit(c)) {
            return (c - '0') + 1 + ('z' - 'a' + 1);
        }
        else {
            throw std::invalid_argument("Argument out of [0-9a-zA-Z] range.");
        }
    }

    // constexpr version of std::strlen
    constexpr size_t strsize(const char *str) {
        size_t size = 0;

        while (str[size] != '\0') {
            ++size;
        }

        return size;
    }

    constexpr header_private::id_t bijection(const char *name) {
        constexpr header_private::id_t base = ('z' - 'a' + 1) + ('9' - '0' + 1) + 1;
        header_private::id_t id = 0;

        for (size_t i = 0; i < strsize(name); i++) {
            id = id * base + get_val(name[i]);
        }

        return id;
    }

}

constexpr header_private::id_t Id(const char *name) {
    if (header_private::strsize(name) == 0) {
        throw std::invalid_argument("Empty identificator.");
    }

    if (header_private::strsize(name) > 6) {
        throw std::invalid_argument("Identificator longer than 6 characters.");
    }

    for (size_t i = 0; i < header_private::strsize(name); ++i) {
        if (not header_private::is_proper_char(name[i])) {
            throw std::invalid_argument("Invalid character in identificator.");
        }
    }

    header_private::id_t id = header_private::bijection(name);

    if (id == header_private::empty_id) {
        throw std::logic_error("Got empty identificator for non-empty name.");
    }

    return id;
}

struct Instruction {};

struct RValue {};

// Inheritance used so is_base_of(RValue, lvalue_type) is true.
struct LValue : RValue {};

// Inheritance used so is_base_of(RValue, literal_type) is true.
struct LiteralValue : RValue {};

template<typename... Ins>
struct Program {};

template<typename Address>
struct Mem : LValue {};

template<auto value>
struct Num : LiteralValue {};

template<header_private::id_t id>
struct Lea : RValue {};

template<typename Lhs, typename Rhs>
struct Add : Instruction {
    static_assert(std::is_base_of_v<LValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<typename Lhs, typename Rhs>
struct Sub : Instruction {
    static_assert(std::is_base_of_v<LValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<typename Arg>
using Inc = Add<Arg, Num<1>>;

template<typename Arg>
using Dec = Sub<Arg, Num<1>>;

template<typename Lhs, typename Rhs>
struct Mov : Instruction {
    static_assert(std::is_base_of_v<LValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<header_private::id_t id, typename InitValue>
struct D : Instruction {
    static_assert(std::is_base_of_v<LiteralValue, InitValue>);
};

template<header_private::id_t id>
struct Label : Instruction {};

template<typename Lhs, typename Rhs>
struct Cmp : Instruction {
    static_assert(std::is_base_of_v<RValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<header_private::id_t id>
struct Jmp : Instruction {};

template<header_private::id_t id>
struct Jz : Instruction {};

template<header_private::id_t id>
struct Js : Instruction {};

template<typename Lhs, typename Rhs>
struct And : Instruction {
    static_assert(std::is_base_of_v<LValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<typename Lhs, typename Rhs>
struct Or : Instruction {
    static_assert(std::is_base_of_v<LValue, Lhs>);
    static_assert(std::is_base_of_v<RValue, Rhs>);
};

template<typename Arg>
struct Not : Instruction {
    static_assert(std::is_base_of_v<LValue, Arg>);
};

template<size_t memory_size, typename memory_cell_t>
class Computer {
    static_assert(std::is_integral<memory_cell_t>(), "Computer memory type not integral.");

    using memory_t = std::array<memory_cell_t, memory_size>;
    using aliases_t = std::array<header_private::id_t, memory_size>;

    struct State {
        memory_t memory;
        aliases_t aliases;
        bool SF;
        bool ZF;
        size_t next_decl_addr;
        header_private::id_t pending_jump_label;
    };

    static constexpr void change_ZF(State &state, memory_cell_t val) {
        state.ZF = (val == 0);
    }

    static constexpr void change_SF(State &state, memory_cell_t val) {
        state.SF = (val < 0);
    }

    static constexpr void change_flags(State &state, memory_cell_t val) {
        change_ZF(state, val);
        change_SF(state, val);
    }

    static constexpr size_t find_alias(header_private::id_t alias, aliases_t &aliases) {
        for (size_t i = 0; i < memory_size; i++) {
            if (aliases[i] == alias) {
                return i;
            }
        }

        throw std::invalid_argument("Definition for identificator not found.");
    }

    // Unmatched template doesn't have any methods, so trying to resolve r_val, l_val or num_val won't work.
    template<typename Arg>
    struct Resolve {};

    template<auto Arg>
    struct Resolve<Num<Arg>> {
        static constexpr auto num_val() {
            static_assert(std::is_integral_v<decltype(Arg)>,
                          "No valid integral Num type provided.");

            return Arg;
        }

        static constexpr auto r_val([[maybe_unused]] State &state) {
            return num_val();
        }
    };

    template<typename Arg>
    struct Resolve<Mem<Arg>> {
    private:
        static constexpr size_t get_index(State &state) {
            auto arg = Resolve<Arg>::r_val(state);
            if (arg < 0) {
                throw std::out_of_range("Negative memory address.");
            }
            return arg;
        }

    public:
        static constexpr memory_cell_t r_val(State &state) {
            return state.memory.at(get_index(state));
        }

        static constexpr memory_cell_t &l_val(State &state) {
            return state.memory.at(get_index(state));
        }
    };


    template<header_private::id_t id>
    struct Resolve<Lea<id>> {
        static constexpr size_t r_val(State &state) {
            return find_alias(id, state.aliases);
        }
    };

    template<typename Arg>
    struct Evaluate {
        static constexpr void _([[maybe_unused]] State &state) {
            static_assert(std::is_same<Arg, Program<>>::value);
        }
    };

    template<typename InsHead, typename... InsTail>
    struct Evaluate<Program<InsHead, InsTail...>> {
        static constexpr void _(State &state) {
            Evaluate<Program<InsTail...>>::_(state);
        }
    };

    template<typename Arg>
    struct Declare {
        static constexpr void _([[maybe_unused]] State &state) {
            static_assert(std::is_same<Arg, Program<>>::value);
        }
    };

    template<typename InsHead, typename... InsTail>
    struct Declare<Program<InsHead, InsTail...>> {
        static constexpr void _(State &state) {
            Declare<Program<InsTail...>>::_(state);
        }
    };

    template<header_private::id_t id, typename InitValue, typename... InsTail>
    struct Declare<Program<D<id, InitValue>, InsTail...>> {
        static constexpr void _(State &state) {
            if (state.next_decl_addr == memory_size) {
                throw std::length_error(
                        "Ran out of available memory addresses for requested variables."
                );
            }

            state.aliases[state.next_decl_addr] = id;
            state.memory[state.next_decl_addr] = Resolve<InitValue>::num_val();
            ++state.next_decl_addr;

            Declare<Program<InsTail...>>::_(state);
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<Add<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            memory_cell_t &val = Resolve<Lhs>::l_val(state);
            val += Resolve<Rhs>::r_val(state);

            change_flags(state, val);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<Sub<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            memory_cell_t &val = Resolve<Lhs>::l_val(state);
            val -= Resolve<Rhs>::r_val(state);

            change_flags(state, val);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<Cmp<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            change_flags(state, Resolve<Lhs>::r_val(state) - Resolve<Rhs>::r_val(state));

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<Mov<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            Resolve<Lhs>::l_val(state) = Resolve<Rhs>::r_val(state);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<header_private::id_t id, typename... Tail>
    struct Evaluate<Program<Jmp<id>, Tail...>> {
        static constexpr void _(State &state) {
            state.pending_jump_label = id;
        }
    };

    template<header_private::id_t id, typename... Tail>
    struct Evaluate<Program<Jz<id>, Tail...>> {
        static constexpr void _(State &state) {
            if (state.ZF) {
                state.pending_jump_label = id;
            }
            else {
                Evaluate<Program<Tail...>>::_(state);
            }
        }
    };

    template<header_private::id_t id, typename... Tail>
    struct Evaluate<Program<Js<id>, Tail...>> {
        static constexpr void _(State &state) {
            if (state.SF) {
                state.pending_jump_label = id;
            }
            else {
                Evaluate<Program<Tail...>>::_(state);
            }
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<And<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            memory_cell_t &val = Resolve<Lhs>::l_val(state);
            val &= Resolve<Rhs>::r_val(state);

            change_ZF(state, val);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Lhs, typename Rhs, typename... Tail>
    struct Evaluate<Program<Or<Lhs, Rhs>, Tail...>> {
        static constexpr void _(State &state) {
            memory_cell_t &val = Resolve<Lhs>::l_val(state);
            val |= Resolve<Rhs>::r_val(state);

            change_ZF(state, val);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Arg, typename... Tail>
    struct Evaluate<Program<Not<Arg>, Tail...>> {
        static constexpr void _(State &state) {
            memory_cell_t &val = Resolve<Arg>::l_val(state);
            val = ~val;

            change_ZF(state, val);

            Evaluate<Program<Tail...>>::_(state);
        }
    };

    template<typename Arg>
    struct Jump {
        static constexpr void _([[maybe_unused]] State &state, [[maybe_unused]] header_private::id_t id) {
            static_assert(std::is_same<Arg, Program<>>::value);
        }
    };

    template<typename InsHead, typename... InsTail>
    struct Jump<Program<InsHead, InsTail...>> {
        static constexpr void _(State &state, header_private::id_t id) {
            Jump<Program<InsTail...>>::_(state, id);
        }
    };

    template<header_private::id_t head_id, typename... InsTail>
    struct Jump<Program<Label<head_id>, InsTail...>> {
        static constexpr void _(State &state, header_private::id_t id) {
            if (head_id == id) {
                Evaluate<Program<InsTail...>>::_(state);
            }
            else {
                Jump<Program<InsTail...>>::_(state, id);
            }
        }
    };

    template<typename Arg>
    struct Check {};

    template<typename ... Ins>
    struct Check<Program<Ins...>> {
        static constexpr void _() {
            static_assert((std::is_base_of_v<Instruction, Ins> and ...),
                          "Provided instructions are not valid.");
        }
    };

public:
    template<typename Program>
    static constexpr memory_t boot() {
        State state{memory_t({0}), aliases_t({0}), false, false, 0, 0};

        Check<Program>::_();

        Declare<Program>::_(state);

        Evaluate<Program>::_(state);

        // pending_jump_label == header_private::empty_id means program has finished, otherwise execution
        // has to be continued from label saved in pending_jump_label
        while (state.pending_jump_label != header_private::empty_id) {
            header_private::id_t jump_id = state.pending_jump_label;
            state.pending_jump_label = header_private::empty_id;
            Jump<Program>::_(state, jump_id);
        }

        return state.memory;
    }
};

#endif // ZADANIE4_COMPUTER_H
