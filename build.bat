@echo off
rem Kompilacja kalkulatora z oknem Dear ImGui. Uruchom z folderu projektu: build.bat
rem -static = exe dziala tez poza MSYS2 (bez plikow .dll od g++)

g++ -std=c++17 -O2 -Wall -Wextra -Isrc -Ithird_party/imgui ^
    src/main.cpp ^
    src/core/Matrix.cpp src/core/Operations.cpp ^
    src/parser/Tokenizer.cpp src/parser/Parser.cpp ^
    src/ui/ConsoleMenu.cpp src/ui/CalculatorPanel.cpp src/ui/Gui.cpp ^
    third_party/imgui/imgui.cpp third_party/imgui/imgui_draw.cpp ^
    third_party/imgui/imgui_tables.cpp third_party/imgui/imgui_widgets.cpp ^
    third_party/imgui/imgui_impl_win32.cpp third_party/imgui/imgui_impl_dx11.cpp ^
    -ld3d11 -ld3dcompiler -ldxgi -lgdi32 -ldwmapi -static ^
    -o MatrixCalculator.exe

if errorlevel 1 (
    echo.
    echo Kompilacja nieudana.
    exit /b 1
)
echo Gotowe: MatrixCalculator.exe
