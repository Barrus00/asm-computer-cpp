# Assembler simulation in C++
This repo contains an implementation of C++ computer simulation. It has it's own memory, and is able to execute assembler language commands. <br>
It uses c++ templates and metaprogramming. <br>
Instructions:

* Id(const char*) - Creates a label.

* Numerical values examples: <br>
  Num<13>, Num<0>, Num<-50>.

* Mem<Addr> - Access the memory with addres "Addr", which must be a valid p-value.<br>
  Examples: <br>
  Mem<Num<0>>, Mem<Lea<Id("a")>>.

* Lea<Id> - returns the addres of variable named with id "Id".<br>
  Examples:<br>
  Lea<Id("A")>, Lea<Id("a")>.

* l-values: Mem.

* r-alues Mem, Num, Lea.

*  D<Id, Value> - declares a variable with id "Id" and value of "Value"<br>
  Examples:<br>
  D<Id("A"), Num<5>>.

* Mov<Dst, Src> - copies value of Src into Dst, Dst must be a valid l-value, and Src must be a valid r-value<br>
  Examples:<br>
  Mov<Mem<Num<0>>, Num<13>>, Mov<Mem<Lea<Id("abc")>>, Mem<Num<0>>>.

* Arithmetic operation:<br>
  Add<Arg1, Arg2> -Adds Arg2 to Arg1.<br>
 Sub<Arg1, Arg2> -Substracts from Arg1, Arg2.bstracts from Arg1, Arg2.<br>
  Inc<Arg> - Increments Arg<br>
  Dec<Arg> - Decrements Arg<br>
  Flags (works like in ASM):<br>
  - ZF (zero flag)<br>
  - SF (sign flag)<br>
  Examples:<br>
  Add<Mem<Num<0>>, Num<1>>, Inc<Mem<Lea<Id("a")>>>.<br>

* Logic operators:<br>
  And<Arg1, Arg2> <br>
  Or<Arg1, Arg2> <br>
  Result is stored in Arg1.<br>
  Not<Arg><br>
  Flags:<br>
  - ZF (zero flag)<br>
  Examples:<br>
  And<Mem<Num<0>>, Num<1>>, Not<Mem<Lea<Id("a")>>>.

* Cmp<Arg1, Arg2> works like Sub<x,y> but doesn't change the Arg1, only sets flags.<br>
  Example: Cmp<Mem<Num<0>>, Num<1>>.

* Label<Id("label")> - creates a Label, that can be accesed later.

* Conditional jumps<br>
  Jmp<Label> - jumps to a label<br>
  Jz<Label>  - jumps if Label ZF is set to 1.<br>
  Js<Label>  - jumps if Label SF is set to 1.<br>
  Example:<br>
  Jmp<Id("label")>, Jz<Id("stop")>.

<br> Example usages are listed in common folder.
