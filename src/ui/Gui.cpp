// Okno Windows + DirectX 11 + Dear ImGui.
// To standardowy "szkielet" z przykładu ImGui (example_win32_directx11) - zwykle się go nie zmienia.
// Cały wygląd kalkulatora jest w CalculatorPanel.cpp.

#include "Gui.hpp"
#include "CalculatorPanel.hpp"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <fstream>
#include <windows.h>

// Funkcja z backendu ImGui, która przekazuje mu kliknięcia i klawisze.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace {

// ======================= DirectX 11 =======================
ID3D11Device* device = nullptr;
ID3D11DeviceContext* deviceContext = nullptr;
IDXGISwapChain* swapChain = nullptr;
ID3D11RenderTargetView* renderTarget = nullptr;
UINT resizeWidth = 0;
UINT resizeHeight = 0;

void createRenderTarget() {
    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
    backBuffer->Release();
}

void cleanupRenderTarget() {
    if (renderTarget) { renderTarget->Release(); renderTarget = nullptr; }
}

bool createDevice(HWND window) {
    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = window;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};
    D3D_FEATURE_LEVEL chosenLevel;
    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, 2,
                                                   D3D11_SDK_VERSION, &desc, &swapChain, &device,
                                                   &chosenLevel, &deviceContext);
    if (result == DXGI_ERROR_UNSUPPORTED) {  // brak karty z DX11 -> rysowanie programowe (WARP)
        result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, levels, 2,
                                               D3D11_SDK_VERSION, &desc, &swapChain, &device,
                                               &chosenLevel, &deviceContext);
    }
    if (result != S_OK) return false;
    createRenderTarget();
    return true;
}

void cleanupDevice() {
    cleanupRenderTarget();
    if (swapChain) { swapChain->Release(); swapChain = nullptr; }
    if (deviceContext) { deviceContext->Release(); deviceContext = nullptr; }
    if (device) { device->Release(); device = nullptr; }
}

// ======================= Okno Windows =======================
LRESULT WINAPI windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam)) return true;

    switch (message) {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED) return 0;
            resizeWidth = LOWORD(lParam);  // zmiana rozmiaru dopiero w pętli głównej
            resizeHeight = HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) return 0;  // wyłącz menu pod klawiszem Alt
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

// ======================= Wygląd =======================
void setupStyle(float scale) {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(16, 16);
    style.FramePadding = ImVec2(8, 5);
    style.ItemSpacing = ImVec2(8, 8);
    style.FrameRounding = 5;
    style.GrabRounding = 5;
    style.TabRounding = 5;
    style.ScaleAllSizes(scale);  // większe odstępy na ekranach z powiększeniem (np. 150%)

    // Czcionka Segoe UI z polskimi literami (ą ć ę ł ń ó ś ź ż).
    // Domyślna czcionka ImGui ich nie ma - byłyby znaki zapytania.
    static const ImWchar polishRanges[] = {
        0x0020, 0x00FF,  // podstawowa łacinka
        0x0100, 0x017F,  // Latin Extended-A: polskie litery
        0,
    };
    const char* fontPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    if (std::ifstream(fontPath).good()) {
        ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 19.0f * scale, nullptr, polishRanges);
    }
}

}  // namespace

int runGui() {
    ImGui_ImplWin32_EnableDpiAwareness();
    float scale = ImGui_ImplWin32_GetDpiScaleForMonitor(MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY));

    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = windowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = L"MatrixCalculator";
    RegisterClassExW(&windowClass);
    HWND window = CreateWindowW(windowClass.lpszClassName, L"Kalkulator macierzy", WS_OVERLAPPEDWINDOW,
                                100, 100, static_cast<int>(1000 * scale), static_cast<int>(760 * scale),
                                nullptr, nullptr, windowClass.hInstance, nullptr);

    if (!createDevice(window)) {
        cleanupDevice();
        UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
        return 1;
    }
    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().IniFilename = nullptr;  // nie zapisuj stanu okna do pliku imgui.ini
    setupStyle(scale);
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, deviceContext);

    CalculatorPanel panel;
    bool done = false;
    while (!done) {
        // 1. Odbierz zdarzenia od Windowsa (klawisze, mysz, zamknięcie okna)
        MSG message;
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
            if (message.message == WM_QUIT) done = true;
        }
        if (done) break;

        if (resizeWidth != 0 && resizeHeight != 0) {
            cleanupRenderTarget();
            swapChain->ResizeBuffers(0, resizeWidth, resizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            resizeWidth = resizeHeight = 0;
            createRenderTarget();
        }

        // 2. Zbuduj klatkę: tu rysuje się cały kalkulator
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        panel.draw();
        ImGui::Render();

        // 3. Wyczyść ekran i wyświetl klatkę
        const float background[4] = {0.10f, 0.10f, 0.12f, 1.0f};
        deviceContext->OMSetRenderTargets(1, &renderTarget, nullptr);
        deviceContext->ClearRenderTargetView(renderTarget, background);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapChain->Present(1, 0);  // 1 = czekaj na odświeżenie monitora (bez marnowania CPU)
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    cleanupDevice();
    DestroyWindow(window);
    UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
    return 0;
}
