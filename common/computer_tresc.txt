Celem zadania jest stworzenie prostej symulacji komputera z pamięcią,
obsługującej język typu asembler. Symulację należy zaimplementować,
używając metaprogramowania i szablonów C++.

Szablon klasy Computer powinien mieć następujące parametry:
* wielkość pamięci – dodatnia wartość określająca liczbę komórek pamięci
  w słowach;
* typ słowa – typ całkowitoliczbowy reprezentujący podstawową jednostkę
  pamięci.

Dodatkowo klasa Computer powinna mieć metodę klasową boot, która załaduje
oraz wykona przekazany program w języku TMPAsm (Template Metaprogramming
Assembler) w czasie kompilowania programu.

Program w języku TMPAsm składa się z ciągu instrukcji. Podczas ładowania
programu pamięć komputera jest inicjowana zerami. Następnie wszystkie zmienne
są kopiowane do pamięci komputera zgodnie z kolejnością deklaracji,
a później wykonywane są pozostałe instrukcje.

Podstawowe elementy języka TMPAsm:

* Identyfikatory zmiennej lub etykiety Id
  Id(const char*) – identyfikator; identyfikator tworzony jest na
  podstawie ciągu znaków o długości od 1 do 6 włącznie, zawierającego małe
  i wielkie litery alfabetu angielskiego (a-zA-Z) oraz cyfry (0-9);
  małe i wielkie litery nie są rozróżniane.
  Przykłady poprawnych identyfikatorów: Id("A"), Id("01234"), Id("Cdefg").

* Literały numeryczne Num
  Literały całkowitoliczbowe.
  Przykłady poprawnych literałów:
  Num<13>, Num<0>, Num<-50>.

* Pamięć Mem
  Mem<Addr> – dostęp do komórki pamięci pod adresem Addr; Addr musi być
  poprawną p-wartością. Odwołania poza dostępną pamięć są wykrywane
  i zgłaszane jako błąd. Zakres adresacji jest zgodny z wersją unsigned
  typu słowa zdefiniowanego dla danego komputera.
  Przykłady poprawnych odwołań do pamięci:
  Mem<Num<0>>, Mem<Lea<Id("a")>>.

* Pobranie efektywnego adresu zmiennej Lea
  Lea<Id> – zwraca wartość efektywnego adresu zmiennej Id.
  Przykłady poprawnych pobrań adresu zmiennej:
  Lea<Id("A")>, Lea<Id("a")>.

* Poprawna lewa wartość (l-wartość) w TMPAsm to Mem.

* Poprawne prawe wartości (p-wartość) w TMPAsm to Mem, Num, Lea.

TMPAsm wspiera następujące instrukcje:

* Deklaracja zmiennej D
  D<Id, Value> – deklaruje zmienną o identyfikatorze Id oraz wartości
  numerycznej Value.
  Przykład poprawnej deklaracji zmiennej:
  D<Id("A"), Num<5>>.

* Operacja kopiowania Mov
  Mov<Dst, Src> – kopiuje wartość Src do Dst; Dst musi być poprawną
  l-wartością, natomiast Src musi być poprawną p-wartością.
  Przykłady poprawnych instrukcji:
  Mov<Mem<Num<0>>, Num<13>>, Mov<Mem<Lea<Id("abc")>>, Mem<Num<0>>>.

* Operacje arytmetyczne Add, Sub, Inc, Dec
  Add<Arg1, Arg2> – dodawanie
  Sub<Arg1, Arg2> – odejmowanie
  Wynik powyższych operacji jest umieszczany w Arg1.
  Arg1 musi być poprawną l-wartością, natomiast Arg2 musi być poprawną
  p-wartością.
  Inc<Arg> – specjalizacja dodawania, zwiększająca wartość Arg o 1
  Dec<Arg> – specjalizacja odejmowania, zmniejszająca wartość Arg o 1
  Arg musi być poprawną l-wartością.
  Wszystkie operacje arytmetyczne ustawiają:
  - flagę ZF (zero flag) procesora na 1, jeśli wynik jest 0,
    a na 0 w przeciwnym przypadku.
  - flagę SF (sign flag) procesora na 1, jeśli wynik jest ujemny,
    a na 0 w przeciwnym przypadku.
  Operacje arytmetyczne są wykonywane zgodnie z typem słowa zdefiniowanym
  dla danego komputera.
  Przykłady poprawnych operacji:
  Add<Mem<Num<0>>, Num<1>>, Inc<Mem<Lea<Id("a")>>>.

* Operacje logiczne And, Or, Not
  And<Arg1, Arg2> – bitowa operacja and
  Or<Arg1, Arg2> – bitowa operacja or
  Wynik powyższych operacji jest umieszczany w Arg1.
  Arg1 musi być poprawną l-wartością, natomiast Arg2 musi być poprawną
  p-wartością.
  Not<Arg> – bitowa negacja
  Arg musi być poprawną l-wartością.
  Wszystkie operacje logiczne ustawiają:
  - flagę ZF (zero flag) procesora na 1, jeśli wynik jest 0,
    a na 0 w przeciwnym przypadku.
  Przykłady poprawnych operacji:
  And<Mem<Num<0>>, Num<1>>, Not<Mem<Lea<Id("a")>>>.

* Operację porównania Cmp
  Cmp<Arg1, Arg2> – działa jak operacja odejmowania, ale nie zapisuje wyniku,
  a tylko ustawia flagi.
  Arg1 oraz Arg2 muszą być poprawnymi p-wartościami.
  Przykład poprawnej operacji: Cmp<Mem<Num<0>>, Num<1>>.

* Oznaczenie etykiety Label
  Label<Id> – ustawienie etykiety o identyfikatorze Id.
  Przykład poprawnej etykiety:
  Label<Id("label")>.

* Instrukcje skoków Jmp, Jz, Js
  Jmp<Label> – skok bezwarunkowy do etykiety o identyfikatorze Label
  Jz<Label>  – skok warunkowy do Label w przypadku gdy flaga ZF jest ustawiona na 1
  Js<Label>  – skok warunkowy do Label w przypadku gdy flaga SF jest ustawiona na 1
  Przykłady poprawnych skoków:
  Jmp<Id("label")>, Jz<Id("stop")>.

Wszystkie obliczenia powinny być wykonane podczas kompilacji.

Wymagania formalne

Rozwiązanie będzie kompilowane za pomocą polecenia

clang -Wall -Wextra -std=c++17 -O2 -lstdc++

Przykład użycia znajduje się w pliku computer_example.cc.

Rozwiązanie powinno zawierać plik computer.h.
Plik należy umieścić w repozytorium w katalogu

grupaN/zadanie4/ab123456+cd123456

lub

grupaN/zadanie4/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami
członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie
powinien zawierać innych plików, ale może zawierać podkatalog prywatne, gdzie
można umieszczać różne pliki, np. swoje testy. Pliki umieszczone w tym
podkatalogu nie będą oceniane. Nie wolno umieszczać w repozytorium plików
dużych, binarnych, tymczasowych (np. *.o) ani innych zbędnych.
