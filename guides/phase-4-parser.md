# Faza 4 — shunting-yard i ewaluator

Otwieraj to dopiero wtedy, gdy zrzuty tokenów z fazy 3 się zgadzają.

## Po co jest ta faza

Tokeny nadal są w kolejności, w której wpisał je użytkownik. `2 * A + B` musi najpierw pomnożyć, a potem dodać, a `inv(A) * B` musi najpierw odwrócić, a potem pomnożyć. Algorytm shunting-yard Dijkstry zamienia listę tokenów na odwrotną notację polską (ONP, ang. RPN), w której każdy operator stoi **po** swoich argumentach. Stos może wtedy liczyć ONP od lewej do prawej bez dodatkowej gramatyki.

Przykład: `2 * A + trans(B)` staje się `2 A * B trans +`.

Typ wartości ewaluatora to `std::variant<double, Matrix>`. Skalary (`det`, `rank`, `trace`, literały liczbowe) i macierze jadą na jednym stosie. Próba dodania skalara do macierzy rzuca wyjątek. To błąd typu, a nie błąd parsowania.

## Pliki do dodania

- `src/parser/ShuntingYard.hpp` i `.cpp`
- `src/parser/Evaluator.hpp` i `.cpp`

Mały `ExpressionEngine` może wywołać tokenizer, potem shunting-yard, potem ewaluację i jest jedynym wejściem parsera, którego potrzebuje interfejs. Zostaw te trzy kroki wywoływalne osobno, żeby dało się wypisać ONP, gdy wynik wygląda źle.

## Priorytety

Wyższa liczba znaczy „zrób to najpierw”.

| Token | Priorytet | Łączność |
|---|---|---|
| `+` `-` (dwuargumentowe) | 1 | lewa |
| `*` | 2 | lewa |
| `^` | 3 | prawa (`2^3^2` to `2^(3^2)`) |
| minus jednoargumentowy | 4 | prawa |
| `^T` | 5 | przyrostkowy, wiąże najciaśniej |

Funkcje siedzą na stosie operatorów i przechodzą na wyjście, gdy znajdzie się zamykający `)`. `solve` zdejmuje dwie wartości (najpierw prawy argument). Każda inna funkcja zdejmuje jedną.

Nawiasy nigdy nie pojawiają się w wyjściu ONP. Przecinek zdejmuje operatory aż do pasującego `(`, a rzuca wyjątek, gdy `(` nie ma.

Zostawiony na końcu `(` oznacza brakującego `)`.

## Zadania ewaluatora

1. Idź po ONP. Odkładaj liczby jako `double`. Odkładaj `A` i `B` jako kopie macierzy przekazanych przez wywołującego.
2. Dwuargumentowe `+` i `-`: skalar ze skalarem albo macierz z macierzą. Typy mieszane rzucają wyjątek.
3. `*`: skalar/skalar, skalar/macierz, macierz/skalar albo macierz/macierz (z fazy 1).
4. `^`: skalar/skalar używa `std::pow`. Macierz/skalar jest legalna tylko wtedy, gdy skalar jest liczbą całkowitą; wołaj `power` z fazy 2.
5. Minus jednoargumentowy neguje ten typ, który leży na stosie.
6. `^T` i `trans` transponują. `inv` i `rref` wymagają macierzy. `det`, `rank` i `tr` wymagają macierzy i odkładają `double` (`rank` jako double, żeby stos został wariantem dwóch typów).
7. `solve` wymaga dwóch macierzy.
8. Przekazuj ten sam `StepLogger` do każdej operacji, która go przyjmuje, i dopisuj też własną krótką linię na każdy krok ONP („mnożenie macierzy 2x2 * macierzy 2x2”). Jeśli stos nie kończy się dokładnie jedną wartością, rzuć wyjątek.

Zapisz napis ONP przed obliczeniem. Gdy wynik jest zły, widać, czy winny jest shunting-yard, czy stos.

## Sprawdzenia

Użyj `A = [[1,2],[3,4]]`, `B = [[5,6],[7,8]]`.

| Wyrażenie | Oczekiwane |
|---|---|
| `2 * A + trans(B)` | `[[7,11],[12,16]]` |
| `inv(A) * B` | ta sama macierz co przy własnym wywołaniu `inv` i `operator*` |
| `det(A)` | `-2` |
| `solve(A, B)` | to samo co `solve(A, B)` z fazy 2 |
| `A^2` | `A * A` |
| `A^T` | transpozycja `A` |
| `A + 1` | rzuca wyjątek |
| `(A + B` | rzuca wyjątek (niedopasowanie) |
| `inv(2)` | rzuca wyjątek |

Wypisz dziennik kroków dla `inv(A) * B`. Powinieneś zobaczyć zarówno linię ONP, jak i kroki wierszowe Gaussa z fazy 2. Jeśli kroków Gaussa brakuje, ewaluator wywołał `inv` bez loggera.

Zatrzymaj się tutaj, aż te wyrażenia się zgadzają. Faza 5 tylko wyświetla ten silnik.
