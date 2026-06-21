#include "Struct/main.h"
#include "Struct/lol_Hooks.h"
#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <Il2Cpp.h>
#include <jni.h>
#include "Struct/Zygisk.hpp"
#include "Struct/Defines.h"
#include "Struct/Gui.hpp"
#include "fonts/FontAwesome6_solid.h"
#include "ImGui/Toggle.h"
#include "ImGui/Theme.h"
#include <fstream>
#include <string>

bool isAuthorized = false;

bool IsValidPackage() {
    std::ifstream ifs("/proc/self/cmdline");
    std::string cmdline;
    if (ifs) {
        std::getline(ifs, cmdline, '\0');
    }
    return cmdline == "com.dts.freefiremax";
}

Config cfg; 
GetTouch_t old_GetTouch = nullptr;
bool HasOriginalPos = false;
bool fourFingerPressed = false;
bool HideFullMenu = false;
bool NewBox = false;
uintptr_t il2cpp_base = 0;
uintptr_t unity_base = 0;

#define ATTACH_JNI(env)                     \
    JNIEnv* env = nullptr;                 \
    bool attached = false;                 \
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) { \
        jvm->AttachCurrentThread(&env, nullptr); \
        attached = true;                   \
    }

#define DETACH_JNI() \
    if (attached) jvm->DetachCurrentThread();

static double lastTapTime = 0.0;
const double doubleTapDelay = 0.25;
static bool CloseGui = true;
static bool HideGui = false;
bool BlockUnityTouch = false;
static ImVec2 tapPos(50, 50);
static int MenuTab = 0;

void *hack_thread(void *) {
    sleep(10);

    if (!IsValidPackage()) {
        return nullptr;
    }

    pid_t pid = getpid();
    
    do {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        sleep(1);
    } while (il2cpp_base == 0);

    do {
        unity_base = get_module_base(pid, "libunity.so");
        sleep(1);
    } while (unity_base == 0);

    Il2CppAttach();
    InitPatches();

    if (Class_Input__GetTouch != 0) {
        DobbyHook((void *)Class_Input__GetTouch, (void *)hook_GetTouch, (void **)&old_GetTouch);
    }

    while (true) {
        if (cfg.aim.legit) {
            AimbotLegitVoid();
        }
        usleep(1000);
    }
    return nullptr;
}

bool ToggleSwitch(const char* id, bool* v) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    float height = ImGui::GetFrameHeight();
    float width  = height * 1.9f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;
    ImU32 col_bg = ImGui::GetColorU32(
        *v ? ImVec4(0.85f, 0.10f, 0.10f, 1.0f)
           : ImVec4(0.30f, 0.30f, 0.30f, 1.0f));

    draw->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw->AddCircleFilled(
        ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius),
        radius - 1.5f,
        IM_COL32(255,255,255,255)
    );
    return true;
}

bool ToggleSwitchFloat(const char* id, bool* v) {
    return false;
}

UnityEngine_Touch_Fields hook_GetTouch(int index) {
    if (BlockUnityTouch) {
        UnityEngine_Touch_Fields fake{};
        fake.m_Phase = TouchPhase::Canceled;
        return fake;
    }
    return *(UnityEngine_Touch_Fields*)old_GetTouch(index);
}

void *getRealAddr(ulong offset) {
    return reinterpret_cast<void*>(il2cpp_base + offset);
}

bool clearMousePos = true;
bool initImGui = false;
inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);

inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
        prevWidth = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();
        g_IsSetup = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    if (MenuTheme == 0) SetElegantSkyGlassTheme();
    if (MenuTheme == 1) SetElegantBlackAndRedTheme();
    if (MenuTheme == 2) SetDripClientTheme();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    
    if (cfg.aim.enabled && cfg.aim.all) {
        AimbotRageVoid();
    }

    int touchCount = (((int (*)())(Class_Input__get_touchCount))());
    if (touchCount >= 4) {
        if (!fourFingerPressed) {
            HideFullMenu = !HideFullMenu;
            fourFingerPressed = true;
        }
    } else {
        fourFingerPressed = false;
    }

    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = *(UnityEngine_Touch_Fields*)old_GetTouch(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;

        if (touch.m_Phase == TouchPhase::Began || touch.m_Phase == TouchPhase::Moved || touch.m_Phase == TouchPhase::Stationary) {
            io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
            io.MouseDown[0] = true;
        } else {
            io.MouseDown[0] = false;
        }
    } else {
        io.MouseDown[0] = false;
    }

    BlockUnityTouch = io.WantCaptureMouse;
    DrawSimpleESP(g_GlWidth, g_GlHeight);

    if (!HideFullMenu && CloseGui) {
        ImVec2 windowSize(80, 80);
        ImGui::SetNextWindowPos(tapPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

        ImGui::Begin("##TapToOpen", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 winPos = ImGui::GetWindowPos();
        float margin = 6.0f;
        float radius = (windowSize.x * 0.5f) - margin;
        ImVec2 center = ImVec2(winPos.x + windowSize.x * 0.5f, winPos.y + windowSize.y * 0.5f);

        ImGui::InvisibleButton("circle_btn", windowSize);
        draw->AddCircleFilled(center, radius, IM_COL32(25, 25, 25, 230));
        draw->AddCircle(center, radius, IM_COL32(90, 90, 90, 255), 0, 3.0f);

        const char* txt = "LOL";
        ImVec2 txtSize = ImGui::CalcTextSize(txt);
        draw->AddText(ImVec2(center.x - txtSize.x * 0.5f, center.y - txtSize.y * 0.5f), IM_COL32(230, 230, 230, 255), txt);

        ImVec2 mousePos = ImGui::GetIO().MousePos;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (mousePos.x >= center.x - radius && mousePos.x <= center.x + radius && mousePos.y >= center.y - radius && mousePos.y <= center.y + radius) {
                double now = ImGui::GetTime();
                if (now - lastTapTime < doubleTapDelay) CloseGui = false;
                lastTapTime = now;
            }
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            tapPos.x += delta.x;
            tapPos.y += delta.y;
        }
        ImGui::End();
    } else if (!HideFullMenu) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(630, 620), ImGuiCond_Once);

        bool GuiOpened = true;
        if (ImGui::Begin(OBFUSCATE("LOL X Cheat"), &GuiOpened, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar)) {
            CloseGui = !GuiOpened;
            float sidebarWidth = 120.0f;
            float buttonHeight = 55.0f;
            float spacing = 9.0f;

            ImGui::BeginChild("##sidebar", ImVec2(sidebarWidth, 0), true, ImGuiWindowFlags_NoScrollbar);
            auto TabButton = [&](const char* name, int id) {
                if (MenuTab == id) {
                    if (MenuTheme == 0) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.7f, 1.0f, 0.8f));
                    else if (MenuTheme == 1) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
                    else if (MenuTheme == 2) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.25f, 1.0f, 1.0f));
                }
                if (ImGui::Button(name, ImVec2(-1, buttonHeight))) MenuTab = id;
                if (MenuTab == id) ImGui::PopStyleColor();
            };

            TabButton(" AIM ", 0); ImGui::Dummy(ImVec2(0, spacing));
            TabButton(" ESP ", 1); ImGui::Dummy(ImVec2(0, spacing));
            TabButton(" MEM ", 2); ImGui::Dummy(ImVec2(0, spacing));
            TabButton(" MORE ", 3); ImGui::Dummy(ImVec2(0, spacing));
            TabButton("CONFIG", 4); ImGui::Dummy(ImVec2(0, spacing));
            TabButton(" INFO ", 5);
            ImGui::EndChild();
            ImGui::SameLine();
        }
        
            ImGui::BeginChild("##content", ImVec2(0, 0), true);
        if (MenuTab == 0) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Enable Functions");
            ImGui::SameLine(220);
            ToggleSwitch("##EnableAll", &cfg.aim.all);
            ImGui::Separator();
            ImGui::Text(ICON_FA_CROSSHAIRS "  AIM MENU");
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), " AIMBOT");
            ImGui::SameLine(130);
            ToggleSwitch("##EnableAim", &cfg.aim.enabled);
            ImGui::Text("Aimbot Type:");
            ImGui::Checkbox("Aimbot Rage", &cfg.aim.rage);
            ImGui::SameLine();
            ImGui::Checkbox("Aimbot Legit", &cfg.aim.legit);
            ImGui::Checkbox("Aim Silent", &cfg.aim.silent);
            ImGui::Text("Aimbot Config:");
            ImGui::Combo("Aim Type", &AimWhen, dir, IM_ARRAYSIZE(dir));
            ImGui::Combo("Target", &AimTarget, AimPos, IM_ARRAYSIZE(AimPos));
            ImGui::SliderFloat("Fov Rate", &Fov_Aim, 0.0f, 1200.0f, "%.f");
            ImGui::SliderFloat("Distance", &Aimdis, 0.0f, 500.0f, "%.f");
            if (AimTarget == 4) {
                ImGui::Text("Drag Settings:");
                ImGui::SliderFloat("Chest Rate", &ChestRate, 0.0f, 200.0f, "%.0f");
            }
        }
        if (MenuTab == 1) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Enable Functions");
            ImGui::SameLine(220);
            ToggleSwitch("##EnableAll", &cfg.aim.all);
            ImGui::Separator();
            ImGui::Text(ICON_FA_EYE "  ESP MENU");
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), " ESP");
            ImGui::SameLine(130);
            ToggleSwitch("##EnableESP", &cfg.esp.enabled);
            ImGui::Text("ESP Type:");
            ImGui::Checkbox("ESP Line", &cfg.esp.line);
            ImGui::Combo("LINE Base", &LineBaseMode, LineBase, IM_ARRAYSIZE(LineBase));
            ImGui::Checkbox("ESP Box", &cfg.esp.box);
            ImGui::SameLine();
            ImGui::Checkbox("ESP FullBox", &NewBox);
            ImGui::Checkbox("ESP Health", &cfg.esp.Health);
            ImGui::Checkbox("HIDE FOV", &HideFov);
        }
        if (MenuTab == 2) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Enable Functions");
            ImGui::SameLine(220);
            ToggleSwitch("##EnableAll", &cfg.aim.all);
            ImGui::Separator();
            ImGui::Text(ICON_FA_BOLT "  MEMORY FUNCTIONS");
            ImGui::Separator();
        }
        if (MenuTab == 3) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Enable Functions");
            ImGui::SameLine(220);
            ToggleSwitch("##EnableAll", &cfg.aim.all);
            ImGui::Separator();
            ImGui::Text(ICON_FA_PLUS "  MORE FUNCTIONS");
            ImGui::Separator();
            ImGui::Text("Aim:");
        }
        if (MenuTab == 4) {
            ImGui::Text(ICON_FA_EDIT "  CONFIG MENU");
            ImGui::Separator();
            ImGui::Text(OBFUSCATE("Esp Config Color:"));
            ImGui::ColorEdit3(OBFUSCATE("Line"), (float*)&EspLineColor, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::ColorEdit3(OBFUSCATE("Box"), (float*)&EspBoxColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3(OBFUSCATE("Fov "), (float*)&EspFovColor, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::ColorEdit3(OBFUSCATE("Name"), (float*)&EspNameColor, ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("Enable RGB", &cfg.esp.enabledRgb);
            ImGui::SliderFloat("Name Size", &EspTextScale, 0.1f, 1.0f, "%.1f");
            ImGui::Text("Menu Style:");
            ImGui::RadioButton("Sky Glass", &MenuTheme, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Black&Blood", &MenuTheme, 1);
            ImGui::RadioButton("Drip Client", &MenuTheme, 2);
            ImGui::Text("Menu Settings:");
            ImGui::Checkbox("Hide Menu", &HideFullMenu);
        }
        if (MenuTab == 5) {
            ImGui::Text(ICON_FA_INFO "  PANEL INFO");
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.6f, 1), ICON_FA_CHECK "  Modded by LOL");
            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1), ICON_FA_TELEGRAM "  LOLXCheat");
        }
        ImGui::EndChild();
    } 

    ImGui::Render();
    ImGui::End();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

inline void StartGUI() {
    void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (ptr_eglSwapBuffer != nullptr) {
        DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
        LOGD("GUI started successfully");
    }
}

void StartHacks() {
    // Wait for the authorized signal from MenuService
    while (!isAuthorized) {
        sleep(2);
    }

    pid_t pid = getpid();
    do {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        sleep(7);
    } while (!il2cpp_base);

    Il2CppAttach();
    InitPatches();

    do {
        unity_base = get_module_base(pid, "libunity.so");
        sleep(1);
    } while (!unity_base);

    StartGUI();

    while (true) {
        if (cfg.aim.legit) {
            AimbotLegitVoid();
        }
        usleep(1000);
    }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    jvm = vm;
    std::thread(StartHacks).detach();
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL
Java_com_LOL_project_MenuService_setAuthorized(JNIEnv* env, jobject obj, jboolean status) {
    isAuthorized = status;
}
