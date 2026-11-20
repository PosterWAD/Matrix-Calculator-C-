#include "ui/Gui.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    FreeConsole();  // schowaj czarne okno konsoli, gdy program uruchomiono dwuklikiem
#endif
    return runGui();  // cały kalkulator działa w oknie ImGui
}
