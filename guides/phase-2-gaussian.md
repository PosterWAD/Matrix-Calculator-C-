# Faza 2 — eliminacja Gaussa i dziennik kroków

Otwieraj to dopiero po przejściu sprawdzeń fazy 1.

## Po co jest ta faza

`det`, `rank`, `inv`, `rref` i `solve(A, B)` to ten sam algorytm z innymi warunkami stopu. Jeśli napiszesz pięć niezwiązanych wzorów, błąd odwrotności nie będzie tym samym błędem co błąd wyznacznika, a dziennik kroków nie będzie miał czego uczciwie wypisać.

Eliminacja Gaussa przepisuje macierz trzema dozwolonymi ruchami:

- zamiana dwóch wierszy: `R1 <-> R2`
- skalowanie wiersza: `R2 = (k) * R2`
- dodanie wielokrotności jednego wiersza do drugiego: `R2 = R2 - (k)*R1`

Te ruchy nie zmieniają zbioru rozwiązań układu liniowego. Zmieniają wyznacznik o znany czynnik (zamiana odwraca znak; skalowanie mnoży wyznacznik przez ten współczynnik). Rząd to liczba pozostałych niezerowych wierszy. Odwrotność to „zrób te same ruchy na `I`”. Rozwiązanie `AX = B` to „zrób te same ruchy na `B`”.

## Pliki do dodania

- `src/core/StepLogger.hpp` i `src/core/StepLogger.cpp`
- `src/core/Operations.hpp` i `src/core/Operations.cpp`

Zostaw `Matrix` jako pojemnik. Te operacje to wolne funkcje, które *używają* macierzy. Dzięki temu wypisywanie, przechowywanie i algorytmy są rozdzielone, czego parser będzie potrzebował: woła `inv(A)` bez wiedzy, jak działa eliminacja.

## `StepLogger`

Każdy krok to zdanie dla człowieka plus opcjonalny zrzut macierzy po tym kroku.

```cpp
struct Step {
    std::string description;
    std::optional<Matrix> snapshot;
};

class StepLogger {
public:
    void log(std::string description, std::optional<Matrix> snapshot = std::nullopt);
    void clear();
    const std::vector<Step>& steps() const;
    std::string toString() const;
};
```

`toString` ma numerować kroki i wypisywać zrzut pod zdaniem, gdy zrzut został zapisany. Każda publiczna operacja poniżej przyjmuje `StepLogger* logger = nullptr`. Gdy wskaźnik jest pusty, licz i nic nie zapisuj. Gdy nie jest pusty, zapisuj każdy ruch na wierszach i macierz po tym ruchu. Wskaźnik (zamiast obowiązkowej referencji) pozwala wołać `det(A)` tam, gdzie kroki nikogo nie obchodzą.

## Funkcje do napisania

```cpp
Matrix rref(const Matrix& matrix, StepLogger* logger = nullptr);
double det(const Matrix& matrix, StepLogger* logger = nullptr);
std::size_t rank(const Matrix& matrix, StepLogger* logger = nullptr);
Matrix inv(const Matrix& matrix, StepLogger* logger = nullptr);
Matrix solve(const Matrix& a, const Matrix& b, StepLogger* logger = nullptr);
double trace(const Matrix& matrix, StepLogger* logger = nullptr);
Matrix power(const Matrix& matrix, int exponent, StepLogger* logger = nullptr);
```

## Jak podejść do algorytmu

Pracuj na **kopii**. Nigdy nie zmieniaj macierzy wywołującego.

Użyj małego epsilona (około `1e-10`), żeby uznać element główny za zero. Dokładne `== 0.0` zawodzi po kilku odejmowaniach.

**`rref` (zredukowana postać schodkowa).** Idź po kolumnach od lewej do prawej. W bieżącej kolumnie wybierz wiersz na wysokości bieżącego wiersza albo poniżej, z największą wartością bezwzględną. Jeśli ta wartość jest ~0, kolumna nie ma elementu głównego; przejdź do następnej kolumny. W przeciwnym razie zamień ten wiersz w górę, przeskaluj go tak, by element główny stał się 1, a potem wyzeruj tę kolumnę w **każdym innym wierszu**, powyżej i poniżej. Zapisuj każdą zamianę, skalowanie i eliminację.

**`det`.** Tylko macierze kwadratowe; inaczej rzuć wyjątek. To samo szukanie elementu głównego, ale zeruj tylko **poniżej** niego (zredukowana postać nie jest potrzebna). Każda zamiana mnoży znak przez -1. Wyznacznik to `znak * iloczyn przekątnej` po sprowadzeniu macierzy do postaci górnotrójkątnej. Brak elementu głównego oznacza wyznacznik 0. Nie skaluj elementu głównego do 1 w `det`, bo musiałbyś ten współczynnik potem odjąć z powrotem; prościej zostawić element główny bez zmian.

**`rank`.** Uruchom `rref` i policz wiersze, które nadal zawierają niezerowy element.

**`inv`.** Tylko kwadratowe. Zbuduj `[A | I]`: `n` wierszy i `2n` kolumn, `I` w prawej połowie. Uruchom na tej szerokiej macierzy tę samą redukcję co w `rref`. Lewa połowa musi stać się `I`. Jeśli nie, `A` jest osobliwa: rzuć wyjątek. Prawa połowa to odwrotność. Zapisz macierz rozszerzoną przed redukcją i po niej.

**`solve(A, B)`.** `A` musi być kwadratowa i mieć tyle wierszy co `B`. `B` może mieć kilka kolumn (kilka prawych stron). Zbuduj `[A | B]`, zredukuj, wymagaj, by lewy blok był `I`, i zwróć prawy blok jako `X`. Jeśli lewy blok nie jest `I`, rzuć „brak jednoznacznego rozwiązania”. W tym projekcie obejmuje to zarówno macierz osobliwą, jak i układ sprzeczny; przypadku nieskończenie wielu rozwiązań jeszcze nie opisujesz.

**`trace`.** Suma głównej przekątnej. Macierz niekwadratowa rzuca wyjątek. Zapisz sumę.

**`power`.** Tylko kwadratowe. Wykładnik `0` zwraca macierz jednostkową. Ujemny wykładnik to `power(inv(A), -exponent)` i rzuci wyjątek, gdy `A` jest osobliwa. Dodatni wykładnik może być wielokrotnym mnożeniem. Zapisz macierz końcową.

## Sprawdzenia do dopisania w `main.cpp`

Użyj `A = [[1,2],[3,4]]`.

| Wywołanie | Oczekiwane |
|---|---|
| `det(A)` | `-2` |
| `trace(A)` | `5` |
| `rank(A)` | `2` |
| `rref(A)` | macierz jednostkowa 2x2 |
| `inv(A) * A` | macierz jednostkowa, w granicy twojej tolerancji równości |
| `solve(A, [[5],[11]])` | `[[1],[2]]` |
| `det([[1,2],[2,4]])` | `0` |
| `rank` tej macierzy osobliwej | `1` |
| `inv` i `solve` na niej | oba rzucają wyjątek |
| `det` macierzy 2x3 | rzuca wyjątek |
| `power([[2,0],[0,3]], 2)` | `[[4,0],[0,9]]` |
| logger przekazany do `det` albo `rref` | co najmniej jeden krok, a tekst zawiera operację na wierszu |

Wypisz raz `logger.toString()` i przeczytaj. Powinieneś dać radę odtworzyć rachunek na kartce.

Kompiluj, dopisując `src/core/StepLogger.cpp` i `src/core/Operations.cpp` do polecenia z fazy 1.

## Pułapki projektowe

- Częściowy wybór elementu głównego: zawsze wstawiaj na pozycję główną największy pozostały element. Pominięcie tego psuje `det` przy zwykłej zamianie wierszy i czyni eliminację niestabilną.
- Zapomnienie zmiany znaku przy zamianie wierszy, przez co `det([[0,1],[1,0]])` zwraca `1` zamiast `-1`.
- Zerowanie tylko poniżej elementu głównego i nazywanie tego `rref`. To jest postać schodkowa. `rref` zeruje też powyżej elementu głównego i skaluje elementy główne do 1.
- Budowanie `[A | I]`, gdy ograniczenie 8x8 nadal siedzi wewnątrz `Matrix`.
- Zapisanie kroku po nieudanej operacji, która połknęła błąd. Rzuć wyjątek po zapisaniu stanu porażki albo rzuć i pozwól wywołującemu zapisać log; nie zwracaj macierzy zerowej.

Zatrzymaj się tutaj, aż te sprawdzenia przejdą.
