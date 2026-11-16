#include "CalculatorPanel.hpp"
#include "../core/MatrixError.hpp"
#include "../parser/ParseError.hpp"
#include "../parser/Tokenizer.hpp"
#include "imgui.h"
#include <algorithm>
#include <cstdio>

// "namespace {}" = te funkcje są widoczne tylko w tym pliku (nie pogryzą się z ConsoleMenu.cpp)
namespace {

const ImVec4 errorColor(1.0f, 0.45f, 0.45f, 1.0f);

Matrix makeStartMatrix(double v00, double v01, double v10, double v11) {
    Matrix matrix(2, 2);
    matrix.set(0, 0, v00);
    matrix.set(0, 1, v01);
    matrix.set(1, 0, v10);
    matrix.set(1, 1, v11);
    return matrix;
}

// Nowa macierz o innym rozmiarze. Liczby, które się mieszczą, zostają; reszta to zera.
Matrix resized(const Matrix& old, int rows, int columns) {
    Matrix result(rows, columns);
    for (int r = 0; r < std::min(rows, old.getRows()); r++) {
        for (int c = 0; c < std::min(columns, old.getColumns()); c++) {
            result.set(r, c, old.get(r, c));
        }
    }
    return result;
}

// Rysuje macierz jako tabelkę z ramkami (tylko do odczytu).
void drawMatrixTable(const char* id, const Matrix& matrix) {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit;
    if (ImGui::BeginTable(id, matrix.getColumns(), flags)) {
        for (int r = 0; r < matrix.getRows(); r++) {
            ImGui::TableNextRow();
            for (int c = 0; c < matrix.getColumns(); c++) {
                ImGui::TableNextColumn();
                ImGui::Text(" %g ", matrix.get(r, c));
            }
        }
        ImGui::EndTable();
    }
}

}  // namespace

CalculatorPanel::CalculatorPanel()
    : a(makeStartMatrix(1, 2, 3, 4)), b(makeStartMatrix(5, 6, 7, 8)) {
    std::snprintf(expression, sizeof(expression), "2 * A + B");
}

void CalculatorPanel::draw() {
    // Jedno okno ImGui na całą powierzchnię okna Windows, bez paska tytułu.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Kalkulator", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::BeginTabBar("tabs")) {
        if (ImGui::BeginTabItem("Kalkulator")) {
            // A i B obok siebie: tabela z dwiema kolumnami
            if (ImGui::BeginTable("matrices", 2)) {
                ImGui::TableNextColumn();
                drawMatrixEditor("A", a);
                ImGui::TableNextColumn();
                drawMatrixEditor("B", b);
                ImGui::EndTable();
            }
            ImGui::Spacing();
            drawExpressionInput();
            ImGui::Spacing();
            drawResult();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Pomoc")) {
            drawHelp();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void CalculatorPanel::drawMatrixEditor(const char* name, Matrix& matrix) {
    ImGui::PushID(name);  // żeby pola A i B miały różne identyfikatory
    ImGui::SeparatorText((std::string("Macierz ") + name).c_str());

    // --- rozmiar ---
    int rows = matrix.getRows();
    int columns = matrix.getColumns();
    float sizeWidth = ImGui::GetFontSize() * 6;
    ImGui::SetNextItemWidth(sizeWidth);
    bool sizeChanged = ImGui::InputInt("wiersze", &rows);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeWidth);
    sizeChanged |= ImGui::InputInt("kolumny", &columns);
    if (sizeChanged) {
        rows = std::clamp(rows, 1, 8);  // rozmiar zawsze 1..8
        columns = std::clamp(columns, 1, 8);
        matrix = resized(matrix, rows, columns);
    }

    // --- komórki ---
    float cellWidth = ImGui::GetFontSize() * 3.5f;
    for (int r = 0; r < matrix.getRows(); r++) {
        for (int c = 0; c < matrix.getColumns(); c++) {
            if (c > 0) ImGui::SameLine();
            ImGui::PushID(r * 8 + c);
            double value = matrix.get(r, c);
            ImGui::SetNextItemWidth(cellWidth);
            if (ImGui::InputDouble("##cell", &value, 0.0, 0.0, "%g")) {  // "##" = pole bez podpisu
                matrix.set(r, c, value);
            }
            ImGui::PopID();
        }
    }

    ImGui::PopID();
}

void CalculatorPanel::drawExpressionInput() {
    ImGui::SeparatorText("Wyrażenie");

    // Przyciski, które dopisują tekst do wyrażenia
    const char* operators[] = {"A", "B", "+", "-", "*", "^", "^T", "(", ")", ","};
    const char* functions[] = {"inv(", "det(", "trans(", "rref(", "tr(", "rank(", "solve("};
    for (const char* text : operators) {
        if (text != operators[0]) ImGui::SameLine();  // pierwszy przycisk w nowej linii, reszta obok
        if (ImGui::Button(text)) appendToExpression(text);
    }
    for (const char* text : functions) {
        if (text != functions[0]) ImGui::SameLine();
        if (ImGui::Button(text)) appendToExpression(text);
    }

    // Pole tekstowe + przycisk. Enter też liczy.
    ImGui::SetNextItemWidth(-ImGui::GetFontSize() * 6);  // ujemna szerokość = zostaw miejsce z prawej
    bool enterPressed = ImGui::InputText("##expression", expression, sizeof(expression),
                                         ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Oblicz", ImVec2(-1, 0)) || enterPressed) {
        calculate();
    }
}

void CalculatorPanel::drawResult() {
    ImGui::SeparatorText("Wynik");

    if (!errorText.empty()) {
        ImGui::TextColored(errorColor, "%s", errorText.c_str());
    } else if (!hasResult) {
        ImGui::TextDisabled("Wpisz wyrażenie i naciśnij Oblicz albo Enter.");
    } else if (std::holds_alternative<double>(result)) {
        ImGui::Text("%g", std::get<double>(result));
    } else {
        const Matrix& matrix = std::get<Matrix>(result);
        ImGui::Text("Macierz %dx%d:", matrix.getRows(), matrix.getColumns());
        drawMatrixTable("result", matrix);
    }
}

void CalculatorPanel::drawHelp() {
    ImGui::SeparatorText("Działania");
    ImGui::BulletText("A + B,  A - B         dodawanie, odejmowanie");
    ImGui::BulletText("2 * A,  A * B         mnożenie przez liczbę i macierzy");
    ImGui::BulletText("-A                    zmiana znaku");
    ImGui::BulletText("A^2,  A^-1            potęga (A^-1 to macierz odwrotna)");
    ImGui::BulletText("A^T                   transpozycja");

    ImGui::SeparatorText("Funkcje");
    ImGui::BulletText("inv(A)    macierz odwrotna");
    ImGui::BulletText("det(A)    wyznacznik");
    ImGui::BulletText("trans(A)  transpozycja");
    ImGui::BulletText("rref(A)   postać schodkowa zredukowana");
    ImGui::BulletText("tr(A)     ślad");
    ImGui::BulletText("rank(A)   rząd");
    ImGui::BulletText("solve(A, B)   rozwiązuje A * X = B");

    ImGui::SeparatorText("Przykłady");
    ImGui::BulletText("A * (A + B)");
    ImGui::BulletText("2 * A + trans(B)");
    ImGui::BulletText("rank(A) + tr(A)");
}

void CalculatorPanel::appendToExpression(const char* text) {
    std::string updated = std::string(expression) + text;
    std::snprintf(expression, sizeof(expression), "%s", updated.c_str());  // przytnie, gdy za długie
}

void CalculatorPanel::calculate() {
    errorText.clear();
    try {
        Parser parser(tokenize(expression), a, b);
        result = parser.parse();
        hasResult = true;
    } catch (const ParseError& error) {
        errorText = std::string("Błąd składni: ") + error.what();
    } catch (const MatrixError& error) {
        errorText = std::string("Błąd obliczeń: ") + error.what();
    }
}
