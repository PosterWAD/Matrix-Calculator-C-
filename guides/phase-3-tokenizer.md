# Faza 3 — tokenizer

Otwieraj to dopiero po przejściu sprawdzeń fazy 2.

## Po co jest ta faza

Ewaluator nie może przeglądać surowych znaków. Tokenizer zamienia `"2.5 * A + inv(B) - A^T"` na listę typowanych kawałków. Parsowanie jest potem pętlą po tej liście. Mieszanie „czytaj znaki” i „rób algebrę liniową” w jednej funkcji to sposób, w jaki błędy o jeden chowają się wewnątrz `inv`.

Ta faza produkuje tylko tokeny. Niczego nie mnoży i nie sprawdza, czy nawiasy się zgadzają. Zgodność nawiasów to faza 4, bo to problem gramatyki, a nie pisowni.

## Pliki do dodania

- `src/parser/Token.hpp`
- `src/parser/Tokenizer.hpp`
- `src/parser/Tokenizer.cpp`

`Tokenizer.cpp` może włączyć nagłówek błędu matematycznego. Nie może włączać kodu interfejsu.

## Rodzaje tokenów

Musisz umieć je odróżnić:

- liczba (`2`, `2.5`, `1e-3`)
- nazwa macierzy `A` albo `B` (akceptuj obie wielkości liter)
- operatory `+`, `-`, `*`
- potęga `^` z następującą liczbą (liczba jest osobnym tokenem; `^` jest operatorem)
- transpozycja `^T` jako **jeden** token, a nie `^` plus identyfikator `T`
- funkcje: `inv`, `det`, `trans`, `rref`, `tr`, `rank`, `solve`
- `(` `)` oraz `,` (przecinek jest potrzebny, żeby dało się rozdzielić `solve(A, B)`)
- minus jednoargumentowy, inny niż minus dwuargumentowy

Każdy token przechowuje swój rodzaj, pozycję w źródle, oryginalny tekst oraz wartość liczbową, gdy rodzaj to liczba.

## Zadania

1. **Pomiń białe znaki.** Spacje i tabulatory nie są tokenami.
2. **Skanuj liczbę.** Cyfry, co najwyżej jedna kropka, opcjonalne `e`/`E` z opcjonalnym znakiem i cyframi. `2.5.1` nie jest jedną liczbą; zatrzymaj się po legalnym przedrostku albo rzuć wyjątek, ale bądź konsekwentny i to przetestuj.
3. **Skanuj identyfikator.** Litery tylko dla nazw, które obsługujemy (`A`, `B` i słowa funkcji). Porównuj bez względu na wielkość liter. Cokolwiek innego (`Q`, `invert`) rzuca `ParseError` i podaje pozycję w komunikacie.
4. **Klasyfikuj `-`.** Jest jednoargumentowy, gdy jest pierwszym tokenem albo gdy poprzedni token to operator, `(`, albo `,`. Jest dwuargumentowy po liczbie, macierzy, `)` albo `^T`. Dlatego `-A + 3 * -B` ma dwa minusy jednoargumentowe, a `A - B` jeden minus dwuargumentowy.
5. **Klasyfikuj `^`.** Jeśli następny znak to `T` albo `t`, a znak po nim nie jest literą ani cyfrą, wypuść jeden token transpozycji i skonsumuj oba znaki. W przeciwnym razie wypuść operator potęgi.
6. **Tokeny jednoznakowe.** `+ * ( ) ,` są bezpośrednie. Każdy inny znak rzuca wyjątek na tym indeksie.

`tokenize` zwraca `std::vector<Token>` i niczego nie oblicza.

## Sprawdzenia

Wypisuj jeden token na linię w postaci `rodzaj tekst @ pozycja`.

| Wejście | Teksty tokenów, po kolei |
|---|---|
| `2.5 * A + inv(B) - A^T` | `2.5`, `*`, `A`, `+`, `inv`, `(`, `B`, `)`, `-`, `A`, `^T` |
| `-A + 3 * -B` | jednoargumentowy `-`, `A`, `+`, `3`, `*`, jednoargumentowy `-`, `B` |
| `solve(A, B)` | `solve`, `(`, `A`, `,`, `B`, `)` |
| `2 * Q` | rzuca wyjątek, pozycja wskazuje `Q` |
| `A $ B` | rzuca wyjątek, pozycja wskazuje `$` |

`A^T` nie może wyprodukować macierzy ani identyfikatora o nazwie `T`.

Dopisz `src/parser/Tokenizer.cpp` do polecenia kompilacji. Zostaw testy faz 1 i 2 w `main`, żeby zauważyć, jeśli ten układ nagłówków zepsuł rdzeń.

Zatrzymaj się tutaj, aż zrzuty tokenów się zgadzają.
