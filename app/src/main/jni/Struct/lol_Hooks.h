#pragma once
#include <imgui.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Struct/main.h>

// Silent Config
int SilentTarget = 0; 
const char* SilentPos[] = { "Head", "Hip" };
const char* AimPos[] = { "Head", "Neck", "Chest", "Hip", "Drag" };
float ChestRate = 80.0f;
float SilentChestRate = 0.0f;

// Target Calculation
Vector3 GetSmartSilentTarget(void* enemy)
{
    Vector3 head = GetHeadPosition(enemy);
    Vector3 hip  = GetHipPosition(enemy);
    float t = SilentChestRate / 100.0f;
    Vector3 target;
    target.x = head.x + (hip.x - head.x) * t;
    target.y = head.y + (hip.y - head.y) * t;
    target.z = head.z + (hip.z - head.z) * t;
    return target;
}

// Basic Checks
inline bool IsValidPlayer(void* player)
{
    if (!player) return false;
    uintptr_t addr = (uintptr_t)player;
    if (addr < 0x10000000) return false;
    void* tf = Component_GetTransform(player);
    return tf != nullptr;
}

inline bool IsMatchValid()
{
    void* match = Curent_Match();
    if (!match) return false;
    void* local = GetLocalPlayer(match);
    return IsValidPlayer(local) && get_MaxHP(local) > 0.1f;
}

// Enemy Search
void* GetClosestEnemy()
{
    if (!cfg.aim.enabled || !cfg.aim.all) return nullptr;
    void* match = Curent_Match();
    void* localPlayer = GetLocalPlayer(match);
    void* camera = Camera_main();
    if (!match || !localPlayer || !camera) return nullptr;

    auto players = *(monoDictionary<uint8_t*, void**>**) ((uintptr_t)match + m_ShortIDToPlayers);
    if (!players || (uintptr_t)players < 0x10000000) return nullptr;

    auto values = players->getValues();
    int count = players->getNumValues();
    if (count > 100) count = 100;

    float shortestAngle = 9999.0f;
    void* closestEnemy = nullptr;
    Vector3 localPos = getPosition(localPlayer);
    Vector3 forward = GetForward(Component_GetTransform(camera));

    for (int i = 0; i < count; i++)
    {
        void* enemy = values[i];
        if (!enemy || enemy == localPlayer || get_isLocalTeam(enemy) || get_IsDieing(enemy) || get_MaxHP(enemy) <= 0.1f || !get_isVisible(enemy))
            continue;

        Vector3 enemyPos = getPosition(enemy);
        Vector3 screen = WorldToScreenPoint(camera, enemyPos);
        
        if (screen.z < 1.0f || !isFov(Vector3(screen.x, screen.y), Vector3(g_GlWidth / 2, g_GlHeight / 2), Fov_Aim)) 
            continue;

        float angle = Vector3::Angle(Vector3::Normalized(enemyPos - localPos), forward);
        
        if (angle < shortestAngle) 
        {
            shortestAngle = angle;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}

    // ESP Rendering
inline void DrawESP(float screenWidth, float screenHeight)
{
    if (!cfg.esp.enabled || !cfg.aim.all) return;
        
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    // Drawing FOV
    if (!HideFov) {
        draw->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), Fov_Aim, ImGui::ColorConvertFloat4ToU32(EspFovColor), (int)(Fov_Aim * 0.8f), 1.8f);
    }

    void* match = Curent_Match();
    void* localPlayer = GetLocalPlayer(match);
    void* camera = Camera_main();
    if (!match || !localPlayer || !camera) return;

    auto players = *(monoDictionary<uint8_t*, void**>**)((uintptr_t)match + m_ShortIDToPlayers);
    if (!players || players->getNumValues() <= 0) return;

    int count = players->getNumValues() > 100 ? 100 : players->getNumValues();
    auto values = players->getValues();

    for (int i = 0; i < count; i++)
    {
        void* enemy = values[i];
        if (!enemy || enemy == localPlayer || get_isLocalTeam(enemy) || get_MaxHP(enemy) <= 0.1f || !get_isVisible(enemy)) continue;
            
        Vector3 worldPos = getPosition(enemy);
        Vector3 screenPos = WorldToScreenPoint(camera, worldPos);
        if (screenPos.z < 1) continue;

        Vector3 headWorld = worldPos + Vector3(0, 1.7f, 0);
        Vector3 headScreen = WorldToScreenPoint(camera, headWorld);
        
        float height = fabs(headScreen.y - screenPos.y);
        float width = height * 0.5f;
        float x = headScreen.x - width / 2;
        float y = screenHeight - headScreen.y;

        // Box
        if (cfg.esp.box) {
            ImU32 boxColor = get_IsDieing(enemy) ? IM_COL32(255, 0, 0, 200) : ImGui::ColorConvertFloat4ToU32(EspBoxColor);
            draw->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), boxColor);
        }

        // Line
        if (cfg.esp.line) {
            ImU32 lineColor = get_IsDieing(enemy) ? IM_COL32(255, 0, 0, 200) : ImGui::ColorConvertFloat4ToU32(EspLineColor);
            ImVec2 lineStart = (LineBaseMode == 0) ? ImVec2(screenWidth / 2, 0) : ImVec2(screenWidth / 2, screenHeight);
            ImVec2 lineEnd = (LineBaseMode == 0) ? ImVec2(x + width / 2, y) : ImVec2(x + width / 2, y + height);
            draw->AddLine(lineStart, lineEnd, lineColor, 1.8f);
        }

        // Health
        if (cfg.esp.health) {
            float ratio = GetHp(enemy) / get_MaxHP(enemy);
            ImU32 healthColor = (ratio < 0.3f) ? IM_COL32(255, 0, 0, 255) : (ratio < 0.6f) ? IM_COL32(255, 255, 0, 255) : IM_COL32(0, 255, 0, 255);
            float barHeight = height * ratio;
            draw->AddRectFilled(ImVec2(x - 6, y + height - barHeight), ImVec2(x - 3, y + height), healthColor);
        }
    }
}

// Aimbot Rage
void AimbotRageVoid()
{
    if (!cfg.aim.enabled || !cfg.aim.all || !cfg.aim.rage) return;

    void* match = Curent_Match();
    void* localPlayer = GetLocalPlayer(match);
    static void* lastLocal = nullptr;

    if (localPlayer != lastLocal) {
        CachedEnemy = nullptr;
        LockedEnemy = nullptr;
        IsEnemyLocked = false;
        HasOriginalPos = false;
        lastLocal = localPlayer;
    }

    void* enemy = GetClosestEnemyCached();
    if (!enemy) return;

    Vector3 enemyPos = GetHeadPosition(enemy);
    // Applying aim offsets
    if (AimTarget == 1) enemyPos.y -= 0.14f;
    else if (AimTarget == 2) enemyPos.y -= 0.25f;
    else if (AimTarget == 3) enemyPos.y -= 0.35f;
    else if (AimTarget == 4) { // Drag
        if (GetHp(enemy) <= ChestRate) enemyPos.y -= 0.30f;
        else enemyPos.y -= 0.14f;
    }

    Vector3 camPos = CameraMain(localPlayer);
    Quaternion rot = GetRotationToLocation(enemyPos, 0.1f, camPos);
    bool firing = get_IsFiring(localPlayer);
    bool scope = get_IsSighting(localPlayer);

    if (AimWhen == 0 || (AimWhen == 1 && firing) || (AimWhen == 2 && scope) || (AimWhen == 3 && (firing || scope)))
        set_aim(localPlayer, rot);
}

// Aimbot Legit
void AimbotLegitVoid()
{
    if (!cfg.aim.enabled || !cfg.aim.all || !cfg.aim.legit) return;

    void* match = Curent_Match();
    if (!match) return;
    void* localPlayer = GetLocalPlayer(match);
    static void* lastLocal = nullptr;

    if (localPlayer != lastLocal) {
        CachedEnemy = nullptr;
        lastLocal = localPlayer;
    }

    void* enemy = GetClosestEnemyCached();
    if (!enemy) return;

    void* collider = *(void**)((uintptr_t)enemy + 0x78);
    void* head = Player_GetHeadCollider(enemy);
    if (head && collider != head)
        SetAimCollider(enemy, head);
}

// Structures for damage calculation
struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t DBLBLKADCNP;
    int32_t KENBMOOEHBG;
    monoString JANPNJIFOJJ;
    bool NNNADMOFPIE;
    void *GPBDEDFKJNA;
    int32_t PIAMIOFEBKF;
    Vector3 CNEICNJFGLM;
    Vector3 HECJHKEDFEB;
    Vector3 JNLGFLFLBHO;
    uint8_t ACAKHEABPEJ;
    bool MJIHLDJNHLF;
    int32_t LOKIMAEAPCB;
    monoDictionary<uint8_t, void **> *FHLFLAHCIBN;
};

struct PlayerID {
    uint32_t NBPDJAAAFBH;
    uint32_t JEDDPHIHGKL;
    uint8_t IOICFFEKAIL;
    uint8_t PHAFNFOFFDB;
    uint64_t BNFAIDHEHOM;
};

struct HitObjectInfo {
    void *klass;
    void *monitor;
    bool m_IsInPool;
    void *HitObject;
    void *HitCollider;
    Vector3 HitLocation;
    Vector3 HitNormal;
    Vector3 RayDir;
    Vector3 StartPosition;
    int32_t Damage;
    float Distance;
    int32_t ActorLayer;
    int32_t HitGroup;
    void *HitPhysicMaterial;
    bool IgnoreHappens;
    bool ViewBlocked;
    struct Vector3 OrigStartPosition;
    uint8_t SpecialHitType;
    uint32_t SpecialHitLevelObjID;
};

// Hooks
int (*orig_PlayerNetwork_TakeDamage)(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID);
int hook_PlayerNetwork_TakeDamage(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) {

    void* CurrentMatch = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(CurrentMatch);

    if (LocalPlayer && damageInfo && (uintptr_t)damageInfo > 0x10000000) {
        *(Vector3*)((uintptr_t)damageInfo + 0x30) = getPosition(ClosestEnemy);
    }
    return orig_PlayerNetwork_TakeDamage(ClosestEnemy, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
}

int (*old_BLAGCMCGEJG1)(void *, HitObjectInfo *);
int BLAGCMCGEJG1(void *ist, HitObjectInfo *HitObject) {
    if (cfg.aim.enabled && cfg.aim.all && cfg.aim.silent) {
        if (HitObject && (uintptr_t)HitObject > 0x10000000) {
            auto *ClosestEnemy = GetClosestEnemyCached();
            if (ClosestEnemy && get_isVisible(ClosestEnemy) && !get_isLocalTeam(ClosestEnemy)) {
                Vector3 EnemyLocation = GetSmartSilentTarget(ClosestEnemy);
                void* collider = Player_GetHeadCollider(ClosestEnemy);
                if (collider && (uintptr_t)collider > 0x10000000) {
                    HitObject->HitObject = get_gameObject(collider);
                    HitObject->HitCollider = collider;
                    HitObject->HitLocation = EnemyLocation;
                    HitObject->RayDir = Vector3::Normalized(EnemyLocation - CameraMain(GetLocalPlayer(Curent_Match())));
                    HitObject->HitGroup = 1;
                    HitObject->ViewBlocked = false;
                }
            }
        }
    }
    return old_BLAGCMCGEJG1(ist, HitObject);
}
