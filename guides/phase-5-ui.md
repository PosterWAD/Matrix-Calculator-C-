# Faza 5 — interfejs z dwiema zakładkami

Otwieraj to dopiero wtedy, gdy wyrażenia z fazy 4 się zgadzają. Zanim napiszesz kod interfejsu, wybierz jeden front:

- Dear ImGui (okno pulpitu)
- FTXUI (interfejs w terminalu)
- mała strona internetowa
- menu konsolowe bez dodatkowych bibliotek (działa z `g++`, które już masz; CMake nie jest zainstalowany na tej maszynie)

Silnik się od tego wyboru nie zmienia. `src/ui/` to jedyny nowy folder i jedyny kod, któremu wolno czytać klawiaturę albo mysz.

## Dlaczego interfejs jest na końcu

Zakładki, siatki i pola tekstowe psują się z zwykłych powodów (fokus, zmiana rozmiaru, złe dane). Jeśli `inv` nadal jest złe, nie odróżnisz błędu interfejsu od błędu matematyki. Faza 4 już udowodniła matematykę w konsoli.

## Co muszą pokazać dwie zakładki

**Zakładka 1 — dane**

- Ustawianie wierszy i kolumn A oraz B, każda wartość od 1 do 8.
- Edycja każdego elementu.
- Pole tekstowe na jedno wyrażenie.
- Ostatni wynik albo tekst błędu, jeśli obliczenie rzuciło wyjątek.

**Zakładka 2 — dziennik**

- `StepLogger::toString()` dla ostatniego obliczenia.
- Zmiana A albo B nie kasuje dziennika, dopóki użytkownik nie uruchomi nowego wyrażenia.

## Zachowanie do zaimplementowania, niezależnie od biblioteki

1. Trzymaj `Matrix A`, `Matrix B`, `StepLogger` i `ExpressionEngine` (albo swój odpowiednik) jako pola typu interfejsu.
2. Przy „uruchom” wywołaj `logger.clear()`, a potem `engine.evaluate(text, A, B, logger)` wewnątrz `try/catch`. Przy sukcesie zapisz postać do wypisania z `variant` (tekst skalara albo tekst `Matrix`). Przy porażce zapisz `ex.what()` i zostaw dziennik czytelny.
3. Odrzucaj rozmiary spoza 1..8 w interfejsie, zanim zbudujesz macierz użytkownika. Nie polegaj na szerszym limicie wewnętrznym.
4. Odczyt wiersza, który nie zawiera dokładnie `cols` liczb, zgłasza błąd i zostawia poprzednią macierz bez zmian.

## Gotowe, gdy

- Z interfejsu możesz zmienić A na `[[1,2],[3,4]]` i B na `[[5],[11]]` (zmień rozmiar B na 2x1).
- `solve(A, B)` pokazuje `[[1],[2]]` na zakładce 1.
- Zakładka 2 pokazuje kroki macierzy rozszerzonej, a nie tylko macierz końcową.
- `A + 1` pokazuje błąd na zakładce 1 i nie wywala programu.
- Wyjście albo zamknięcie okna nie przenosi wpół edytowanej macierzy do następnego uruchomienia; proces po prostu się kończy.

Przynieś błędy kompilatora, zły wynik albo stan interfejsu, do którego nie możesz dojść, a poprawimy tylko tę fazę.
