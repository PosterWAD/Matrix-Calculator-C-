# Faza 1 — `Matrix` i podstawowa arytmetyka

Rób tylko tę fazę. Fazy 2–5 zależą od tej klasy, więc niedbała `Matrix` utrudni później szukanie błędów w eliminacji Gaussa i w parserze.

## Po co jest ta faza

Każda późniejsza funkcja to „weź jedną albo dwie macierze i wyprodukuj wartość”. Jeśli zapis, indeksowanie albo `+ - *` są złe, to `inv`, `solve` i `2 * A + trans(B)` też będą złe, a nie odróżnisz błędu macierzy od błędu parsera.

Macierz to prostokątna tabela liczb plus reguły łączenia tabel. Klasa jest właścicielem tabeli. Operatory to tylko sprawdzone sposoby zbudowania nowej tabeli.

## Pliki do utworzenia

- `src/core/Matrix.hpp`
- `src/core/Matrix.cpp`
- `src/main.cpp` — wyłącznie program testowy tej fazy

## Decyzje projektowe i powody

**Trzymaj elementy w jednym `std::vector<double>`, wiersz po wierszu.** Wektor wektorów też działa, ale płaski bufor sprawia, że późniejsza zamiana wierszy w eliminacji Gaussa jest jedną zamianą zakresu i zostaje jedna alokacja. Indeks wiersza `r` i kolumny `c` to `r * cols + c`.

**Wymiary są ustalone po konstrukcji.** Dodawanie i mnożenie zwracają nową macierz. Nie zmieniają rozmiaru `*this`. Dzięki temu operator nie zostawia wpół zaktualizowanego obiektu, gdy późniejsze sprawdzenie rzuci wyjątek.

**`at(row, col)` sprawdza zakres i rzuca wyjątek.** `operator[]` wektora nie sprawdza zakresu tak, żeby w komunikacie podać rozmiar macierzy. Komunikat w rodzaju „indeks (2, 0) jest poza macierzą 2x3” przyda się, gdy parser źle policzy pozycję.

**Rzucaj własny typ wyjątku** pochodny od `std::runtime_error`, na przykład `MatrixError`. Wywołujący złapie błąd matematyczny bez łapania każdego możliwego `std::exception`. Tę małą klasę umieść w nagłówku macierzy albo w `src/core/MatrixError.hpp`.

**Wewnątrz `Matrix` pozwól na do 16 w każdym wymiarze.** Aplikacja odrzuci dane użytkownika powyżej 8x8 w interfejsie. Faza 2 i tak potrzebuje macierzy rozszerzonej 8x16. Jeśli na sztywno ograniczysz klasę do 8, odwrotności nie da się zbudować. W konstruktorze odrzucaj 0 i wszystko powyżej 16.

**Równość używa małej tolerancji** (około `1e-9`). Liczby zmiennoprzecinkowe sprawiają, że `0.1 + 0.2` nie ma czystego wzorca bitowego. Dokładne `==` na każdym elemencie sprawi, że późniejsze testy będą niestabilne.

## Publiczny interfejs do zaimplementowania

Prywatne funkcje pomocnicze wybierasz sam. Publiczna powierzchnia ma umieć to wszystko:

```cpp
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols, double fill = 0.0);
    Matrix(std::initializer_list<std::initializer_list<double>> rows);

    std::size_t rows() const;
    std::size_t cols() const;

    double& at(std::size_t row, std::size_t col);
    double  at(std::size_t row, std::size_t col) const;

    Matrix transpose() const;

    Matrix operator-() const;
    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;   // macierz * macierz
    Matrix operator*(double scalar) const;       // macierz * skalar

    bool operator==(const Matrix& rhs) const;
};

Matrix operator*(double scalar, const Matrix& matrix);  // skalar * macierz
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
```

`operator*` nie może być metodą dla `skalar * macierz`, bo lewy argument to `double`. Metoda `operator*(double)` obsługuje `macierz * skalar`. Wolna funkcja może ją wywołać.

## Zadania

1. **Konstruktor `(rows, cols, fill)`.** Odrzucaj wiersze albo kolumny spoza 1..16. Alokuj `rows * cols` elementów ustawionych na `fill`.
2. **Konstruktor z listy inicjalizacyjnej.** `Matrix{{1, 2, 3}, {4, 5, 6}}` ma być 2x3. Odrzucaj pustą listę, pusty wiersz albo wiersze różnej długości. Istnieje po to, żeby `main.cpp` mógł zapisać oczekiwane wyniki bez pętli.
3. **`at`.** Oblicz płaski indeks. Jeśli `row >= rows` albo `col >= cols`, rzuć `MatrixError` zanim dotkniesz wektora.
4. **`transpose`.** Zbuduj macierz `cols` na `rows`. Element `(c, r)` wyniku to element `(r, c)` tej macierzy. Nie zmieniaj `*this`.
5. **`operator+` i `operator-`.** Oba wymagają tego samego kształtu. Jeśli go nie ma, rzuć wyjątek i podaj oba rozmiary w komunikacie. W przeciwnym razie zbuduj wynik i dodawaj albo odejmuj element po elemencie.
6. **`operator*(const Matrix&)`.** Legalne tylko gdy `this->cols() == rhs.rows()`. Rozmiar wyniku to `this->rows()` na `rhs.cols()`. Element `(r, c)` to iloczyn skalarny wiersza `r` lewej macierzy i kolumny `c` prawej. Ten element zaczyna od `0`.
7. **Mnożenie przez skalar z obu stron oraz minus jednoargumentowy.** Minus jednoargumentowy to mnożenie przez `-1`. Nie pisz drugiej pętli, jeśli możesz użyć ponownie `operator*(double)`.
8. **`operator==`.** Różne kształty nie są równe. Ten sam kształt jest równy, gdy każda para elementów różni się co najwyżej o `1e-9`.
9. **`operator<<`.** Wypisuj jeden wiersz na linię, żeby było widać błędy. Stała precyzja, na przykład 4 miejsca po przecinku, wystarczy. Dokładne odstępy nie mają znaczenia.

## Czym ta faza nie jest

Bez wyznacznika, odwrotności, parsera i menu. Jeśli dodasz je teraz, będziesz debugować dwie warstwy naraz.

## Sprawdzenia w `main.cpp`

Napisz `main`, które buduje macierze listami inicjalizacyjnymi, wypisuje każdy wynik i jasną linię PASS/FAIL. Proponowane przypadki:

| Przypadek | Wejście | Oczekiwane |
|---|---|---|
| Dodawanie | `[[1,2,3],[4,5,6]] + [[7,8,9],[10,11,12]]` | `[[8,10,12],[14,16,18]]` |
| Odejmowanie | druga minus pierwsza | `[[6,6,6],[6,6,6]]` |
| Skalowanie z lewej | `2.5 * [[1,2,3],[4,5,6]]` | `[[2.5,5,7.5],[10,12.5,15]]` |
| Mnożenie | ta macierz 2x3 razy `[[1,2],[3,4],[5,6]]` | `[[22,28],[49,64]]` |
| Transpozycja | transpozycja macierzy 2x3 | `[[1,4],[2,5],[3,6]]` |
| Złe dodawanie | 2x3 + 3x2 | rzuca wyjątek; program idzie dalej, jeśli go złapiesz |
| Złe mnożenie | 2x3 * 2x3 | rzuca wyjątek |
| Zły indeks | `at(9, 0)` na macierzy 2x3 | rzuca wyjątek |

Wypisz też jedną macierz przez `std::cout`, żeby dało się ją przeczytać wzrokiem.

Kompilacja:

```
g++ -std=c++23 -Wall -Wextra -Wpedantic -Isrc -o MatrixCalculator.exe src/main.cpp src/core/Matrix.cpp
```

Uruchom `MatrixCalculator.exe`. Każda linia arytmetyki ma zgadzać się z tabelą. Każde niedozwolone wywołanie ma być złapane i zgłoszone, a nie przerwać proces nieobsłużonym wyjątkiem.

## Błędy, których warto szukać, zanim uznasz fazę za skończoną

- Pomyłka o jeden w `r * cols + c` (użycie `rows` zamiast `cols`).
- `operator*` porównuje złe wymiary (`rows == rows` zamiast `left.cols == right.rows`).
- Wewnętrzna pętla mnożenia zapisuje do lewej macierzy zamiast do wyniku.
- `operator==` używa `!=` na surowych `double`.
- Konstruktor z listy przyjmuje `{{1,2},{3}}`.
- Konstruktor sprawdza `rows > 8` i zablokuje macierz rozszerzoną w fazie 2.

Gdy te sprawdzenia przejdą, napisz o tym i zaczynamy fazę 2. Jeśli któreś nie wyjdzie, wklej `main.cpp`, źródła macierzy i wynik.
