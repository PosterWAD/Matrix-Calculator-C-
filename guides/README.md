# Kalkulator macierzy — jak będziemy to budować

Kod piszesz ty. Te przewodniki mówią, co zbudować, po co to istnieje i skąd wiedzieć, że działa. Nie zawierają gotowych implementacji.

Pracuj po jednej fazie:

1. Czytaj tylko przewodnik bieżącej fazy.
2. Utwórz pliki, które wymienia, i sam napisz ciała funkcji.
3. Skompiluj i uruchom sprawdzenia z końca tego przewodnika.
4. Jeśli coś nie wyjdzie, wklej błąd kompilatora albo zły wynik i zapytaj. Debugujemy tę fazę, zanim pójdziemy dalej.
5. Napisz, gdy sprawdzenia fazy przechodzą. Następny przewodnik zostaje zamknięty do tego momentu.

## Układ, do którego dorośnie projekt

```
src/
  core/       Matrix, później StepLogger i operacje Gaussa
  parser/     tokenizer, potem shunting-yard i ewaluator
  ui/         dochodzi w fazie 5, po wyborze interfejsu
  main.cpp    mały program konsolowy tylko do testu bieżącej fazy
```

Matematyka i parser nie mogą włączać żadnych nagłówków interfejsu. Faza 5 jest jedynym miejscem, które wolno im wywoływać te warstwy.

## Zasady na każdą fazę

- C++23. Tylko biblioteka standardowa. Bez Eigena i innych bibliotek matematycznych.
- Jedna klasa (albo jedna grupa wolnych funkcji) na parę nagłówek/plik źródłowy.
- Błędne wymiary i niemożliwa matematyka rzucają wyjątek. Nie zwracaj w takich przypadkach cichego zera ani pustej macierzy.
- Macierze użytkownika mają rozmiar od 1x1 do 8x8. Tę granicę trzymaj przy wczytywaniu danych. Sam typ `Matrix` musi pomieścić szerszą macierz tymczasową, bo faza 2 buduje `[A | I]`, czyli 8 wierszy na 16 kolumn.
- Nazwy mają mówić, czym wartość jest: `rows`, `cols`, `pivotRow`, a nie `x` i `tmp`, chyba że zakres to jedna linia.

## Proponowana kompilacja

Z folderu projektu, gdy pliki danej fazy już istnieją:

```
g++ -std=c++23 -Wall -Wextra -Wpedantic -Isrc -o MatrixCalculator.exe src/main.cpp src/core/Matrix.cpp
```

Dopisuj każdy nowy plik `.cpp` do tego polecenia w miarę ich powstawania. Każde ostrzeżenie traktuj jak błąd.
