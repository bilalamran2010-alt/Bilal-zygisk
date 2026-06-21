#pragma once
extern bool HasOriginalPos;
#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include "obfuscate.h"
#include "Unity/unity.h"
#include "Class.h"
#include "Rect.h"
inline static int g_GlHeight, g_GlWidth;
#include "Bools.h"
#include <math.h>
#include <stdio.h>
#include <mutex>
#include <string>
#include <KittyMemory/MemoryPatch.h>

int SilentTarget = 0;
const char* SilentPos[] = { "Head", "Hip" };

const char* AimPos[] = { "Head", "Neck", "Chest", "Hip", "Drag" };
float ChestRate = 80.0f;
float SilentChestRate = 0.0f;

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
const char* LineBase[] = { "TOP", "BUTTOM" };

static void* CachedEnemy = nullptr;
static float LastEnemyUpdate = 0;

void UpdatePatches() {

}

inline bool IsValidPlayer(void* player)
{
    if (!player)
        return false;

    uintptr_t addr = (uintptr_t)player;

    if (addr < 0x10000000)
        return false;

    void* tf = Component_GetTransform(player);
    if (!tf)
        return false;

    return true;
}

inline bool IsMatchValid()
{
    void* match = Curent_Match();
    if (!match)
        return false;

    void* local = GetLocalPlayer(match);
    if (!IsValidPlayer(local))
        return false;

    if (get_MaxHP(local) <= 0.1f)
        return false;

    return true;
}

inline bool IsSafeEnemy(void* enemy)
{
    if (!enemy) return false;
    if (!IsValidPlayer(enemy)) return false;
    if (!IsMatchValid()) return false;

    void* tf = Component_GetTransform(enemy);
    if (!tf) return false;

    return true;
}
void* GetClosestEnemy()
{
    if (!cfg.aim.enabled || !cfg.aim.all)
        return nullptr;

    void* match = Curent_Match();
    if (!match)
        return nullptr;
        
    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer)
        return nullptr;
        
    void* camera = Camera_main();
    if (!camera)
        return nullptr;

    uintptr_t matchAddr = (uintptr_t)match;

    if (matchAddr < 0x10000000)
        return nullptr;

    auto players = *(monoDictionary<uint8_t*, void**>**)
    (matchAddr + m_ShortIDToPlayers);

    if (!players || (uintptr_t)players < 0x10000000)
        return nullptr;

    auto values = players->getValues();
    if (!values)
        return nullptr;

    int count = players->getNumValues();

    if (count <= 0)
        return nullptr;

    if (count > 100)
        count = 100;

    float shortestAngle = 9999.0f;
    void* closestEnemy = nullptr;

    Vector3 localPos = getPosition(localPlayer);
    void* camTf = Component_GetTransform(camera);
    if (!camTf)
        return nullptr;

    Vector3 forward = GetForward(camTf);
        for (int i = 0; i < count; i++)
    {
        void* enemy = values[i];

        if (enemy == localPlayer)
            continue;

        if (get_isLocalTeam(enemy))
            continue;

        if (get_IsDieing(enemy))
            continue;

        if (get_MaxHP(enemy) <= 0.1f)
            continue;

        if (!get_isVisible(enemy))
            continue;

        Vector3 enemyPos = getPosition(enemy);

        float distance = Vector3::Distance(localPos, enemyPos);
        if (distance > Aimdis)
            continue;

        Vector3 screen = WorldToScreenPoint(camera, enemyPos);

        if (screen.z < 1.0f)
            continue;

        if (isnan(screen.x) || isnan(screen.y))
            continue;

        bool insideFov = isFov(
            Vector3(screen.x, screen.y),
            Vector3(g_GlWidth / 2, g_GlHeight / 2),
            Fov_Aim
        );

        if (!insideFov)
            continue;

        Vector3 diff = enemyPos - localPos;

        if (diff.x == 0 && diff.y == 0 && diff.z == 0)
            continue;

        Vector3 dir = Vector3::Normalized(diff);
        
        float dot = Vector3::Dot(forward, dir);

        if (dot <= 0.02f)
            continue;

        float angle = Vector3::Angle(dir, forward);

        if (angle < shortestAngle && angle <= Fov_Aim)
        {
            shortestAngle = angle;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}

// CACHE CLOSESTENEMY FOR MORE SAFE USAGE
void* GetClosestEnemyCached()
{
    float time = ImGui::GetTime();

    if (time - LastEnemyUpdate > 0.1f)
    {
        LastEnemyUpdate = time;
        CachedEnemy = GetClosestEnemy();
    }

    return CachedEnemy;
}
// ESP
inline void DrawSimpleESP(float screenWidth, float screenHeight)
{
    if (!cfg.esp.enabled || !cfg.aim.all)
        return;
        
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw)
        return;

    if (!HideFov) {
        ImVec2 center = ImVec2(screenWidth / 2.0f, screenHeight / 2.0f);

        int segments = (int)(Fov_Aim * 0.8f);

        draw->AddCircle(
            center,
            Fov_Aim,
            ImGui::ColorConvertFloat4ToU32(EspFovColor),
            segments,
            1.8f
        );
    }

    void* match = Curent_Match();
    if (!match)
        return;

    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer)
        return;

    void* camera = Camera_main();
    if (!camera)
        return;

    auto players = *(monoDictionary<uint8_t*, void**>**)((uintptr_t)match + m_ShortIDToPlayers);
    if (!players || (uintptr_t)players < 0x10000000)
        return;
    
    int count = players->getNumValues();
    if (count <= 0)
        return;

    if (count > 100)
        count = 100;
    
 //   DrawEnemiesCount(screenWidth);
    
    auto values = players->getValues();
    if (!values) return;

    for (int i = 0; i < count; i++)
    {
        void* enemy = values[i];
        if (!enemy) continue;

        void* safeEnemy = enemy;
            
        if (!safeEnemy) continue;
            
        if (safeEnemy == localPlayer)
            continue;

        if (get_isLocalTeam(safeEnemy))
            continue;

        if (get_MaxHP(safeEnemy) <= 0.1f)
            continue;
            
        if (!get_isVisible(safeEnemy))
            continue;

        Vector3 worldPos = getPosition(safeEnemy);
        Vector3 screenPos = WorldToScreenPoint(camera, worldPos);
        if (screenPos.z < 1)
            continue;

        Vector3 headWorld = worldPos + Vector3(0, 1.7f, 0);
        Vector3 headScreen = WorldToScreenPoint(camera, headWorld);
        if (headScreen.z < 1)
            continue;

        float height = fabs(headScreen.y - screenPos.y);
        float width  = height * 0.5f;

        float x = headScreen.x - width / 2;
        float y = screenHeight - headScreen.y;
                // ============= CORNER BOX ============
        if (cfg.esp.box)
        {
            ImU32 boxColor = get_IsDieing(safeEnemy) ?
                IM_COL32(255, 0, 0, 200) :
                ImGui::ColorConvertFloat4ToU32(EspBoxColor);

            float lineW = 1.8f;
            float cornerW = width * 0.30f;
            float cornerH = height * 0.30f;

            // TOP LEFT
            draw->AddLine(ImVec2(x, y), ImVec2(x + cornerW, y), boxColor, lineW);
            draw->AddLine(ImVec2(x, y), ImVec2(x, y + cornerH), boxColor, lineW);

            // TOP RIGHT
            draw->AddLine(ImVec2(x + width, y), ImVec2(x + width - cornerW, y), boxColor, lineW);
            draw->AddLine(ImVec2(x + width, y), ImVec2(x + width, y + cornerH), boxColor, lineW);

            // BOTTOM LEFT
            draw->AddLine(ImVec2(x, y + height), ImVec2(x + cornerW, y + height), boxColor, lineW);
            draw->AddLine(ImVec2(x, y + height), ImVec2(x, y + height - cornerH), boxColor, lineW);

            // BOTTOM RIGHT
            draw->AddLine(ImVec2(x + width, y + height), ImVec2(x + width - cornerW, y + height), boxColor, lineW);
            draw->AddLine(ImVec2(x + width, y + height), ImVec2(x + width, y + height - cornerH), boxColor, lineW);
        }

        // ============= NEW BOX ============
        if (cfg.esp.box)
        {
            ImVec4 color = EspBoxColor;
            color.w = 0.30f;

            ImU32 boxColor = get_IsDieing(safeEnemy) ?
                IM_COL32(255, 0, 0, 80) :
                ImGui::ColorConvertFloat4ToU32(color);

            draw->AddRectFilled(
                ImVec2(x, y),
                ImVec2(x + width, y + height),
                boxColor
            );
        }

        // ================= LINE =================
        if (cfg.esp.line)
        {
            ImU32 lineColor = get_IsDieing(safeEnemy) ?
                IM_COL32(255, 0, 0, 200) :
                ImGui::ColorConvertFloat4ToU32(EspLineColor);

            ImVec2 lineStart;

            if (LineBaseMode == 0) // TOP
                lineStart = ImVec2(screenWidth / 2, 0);
            else
                lineStart = ImVec2(screenWidth / 2, screenHeight);
            
            ImVec2 lineEnd;
            if (LineBaseMode == 0)
                lineEnd = ImVec2(x + width / 2, y);
            else
                lineEnd = ImVec2(x + width / 2, y + height);

            draw->AddLine(
                lineStart,
                lineEnd,
                lineColor,
                1.8f
            );
        }
        
        // ================= HEALTH =================
        if (cfg.esp.Health)
        {
            float maxHp = get_MaxHP(safeEnemy);
            if (maxHp <= 0.1f)
                continue;

            float hp = GetHp(safeEnemy);
            float ratio = hp / maxHp;

            ImU32 healthColor = IM_COL32(0,255,0,255);
            if (ratio < 0.6f)
                healthColor = IM_COL32(255,255,0,255);
            if (ratio < 0.3f)
                healthColor = IM_COL32(255,0,0,255);

            float barHeight = height * ratio;

            draw->AddRectFilled(
                ImVec2(x - 6, y),
                ImVec2(x - 3, y + height),
                IM_COL32(0,0,0,255)
            );

            draw->AddRectFilled(
                ImVec2(x - 6, y + height - barHeight),
                ImVec2(x - 3, y + height),
                healthColor
            );
        }
    }
}
void AimbotRageVoid()
{
    if (!cfg.aim.enabled || !cfg.aim.all || !cfg.aim.rage)
        return;

    void* match = Curent_Match();
    void* localPlayer = GetLocalPlayer(match);
    static void* lastLocal = nullptr;

    if (localPlayer != lastLocal)
    {
        CachedEnemy = nullptr;
     //   LockedEnemy = nullptr;
     //   IsEnemyLocked = false;
        HasOriginalPos = false;
        lastLocal = localPlayer;
    }

    void* enemy = GetClosestEnemyCached();
    if (!enemy)
        return;

    Vector3 enemyPos = GetHeadPosition(enemy);

    if (AimTarget == 1)
        enemyPos.y -= 0.14f;
    else if (AimTarget == 2)
        enemyPos.y -= 0.25f;
    else if (AimTarget == 3)
        enemyPos.y -= 0.35f;
    else if (AimTarget == 4)
    {
        float enemyHP = GetHp(enemy);
        enemyPos.y -= 0.14f;
        if (enemyHP <= ChestRate)
        {
            enemyPos.y -= 0.16f;
        }
    }

    Vector3 camPos = CameraMain(localPlayer);
    Quaternion rot = GetRotationToLocation(enemyPos, 0.1f, camPos);
    bool firing = get_IsFiring(localPlayer);
    bool scope = get_IsSighting(localPlayer);

    if (AimWhen == 0)
        set_aim(localPlayer, rot);
    else if (AimWhen == 1 && firing)
        set_aim(localPlayer, rot);
    else if (AimWhen == 2 && scope)
        set_aim(localPlayer, rot);
    else if (AimWhen == 3 && (firing || scope))
        set_aim(localPlayer, rot);
}

void AimbotLegitVoid()
{
    if (!cfg.aim.enabled || !cfg.aim.all || !cfg.aim.legit)
        return;

    void* match = Curent_Match();
    if (!match || (uintptr_t)match < 0x10000000)
        return;

    void* localPlayer = GetLocalPlayer(match);
    static void* lastLocal = nullptr;

    if (localPlayer != lastLocal)
    {
        CachedEnemy = nullptr;
       // LockedEnemy = nullptr;
       // IsEnemyLocked = false;
        HasOriginalPos = false;
        lastLocal = localPlayer;
    }

    void* enemy = GetClosestEnemyCached();
    if (!enemy)
        return;

    void* collider = *(void**)((uintptr_t)enemy + 0x78);
    void* head = Player_GetHeadCollider(enemy);
    if (head && collider != head)
        SetAimCollider(enemy, head);
}

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

bool isEnemyInRangeWeapon(void *player, void *enemy, void *weapon)
{
    if (!player || !enemy || !weapon) return false;
    Vector3 PlayerPos = getPosition(player);
    Vector3 EnemyPos  = getPosition(enemy);
    float distance = Vector3::Distance(PlayerPos, EnemyPos);
    float range = get_Range(weapon);
    if (range <= 0.0f) return false;
    if (distance <= (range * 0.95f)) return true;
    return false;
}

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

int (*orig_PlayerNetwork_TakeDamage)(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID);
int hook_PlayerNetwork_TakeDamage(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) {
    void* CurrentMatch = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(CurrentMatch);
    if (LocalPlayer && damageInfo) {
        if ((uintptr_t)damageInfo > 0x10000000) {
            Vector3 enemyPos = getPosition(ClosestEnemy);
            *(Vector3*)((uintptr_t)damageInfo + 0x30) = enemyPos;
        }
    }
    return orig_PlayerNetwork_TakeDamage(ClosestEnemy, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
}

int (*old_BLAGCMCGEJG1)(void *, HitObjectInfo *);
int BLAGCMCGEJG1(void *ist, HitObjectInfo *HitObject) {
    if (cfg.aim.enabled && cfg.aim.all && cfg.aim.silent) {
        if (!HitObject || (uintptr_t)HitObject < 0x10000000) 
            return old_BLAGCMCGEJG1(ist, HitObject);
            
        void* current_match = Curent_Match();
        if (!current_match || (uintptr_t)current_match < 0x10000000) 
            return old_BLAGCMCGEJG1(ist, HitObject);
            
        void* local_player = GetLocalPlayer(current_match);
        auto *ClosestEnemy = GetClosestEnemyCached();
        
        if (ClosestEnemy != nullptr) {
            if (!get_isVisible(ClosestEnemy)) 
                return old_BLAGCMCGEJG1(ist, HitObject);
                
            if (!get_isLocalTeam(ClosestEnemy)) {
                Vector3 EnemyLocation = GetSmartSilentTarget(ClosestEnemy);
                Vector3 PlayerLocation = CameraMain(local_player);
                void* collider = Player_GetHeadCollider(ClosestEnemy);
                
                if (!collider || (uintptr_t)collider < 0x10000000) 
                    return old_BLAGCMCGEJG1(ist, HitObject);
                    
                void* obj = get_gameObject(collider);
                if (!obj || (uintptr_t)obj < 0x10000000) 
                    return old_BLAGCMCGEJG1(ist, HitObject);
                    
                HitObject->HitObject = obj;
                HitObject->HitCollider = collider;
                HitObject->StartPosition = PlayerLocation;
                HitObject->HitLocation = EnemyLocation;
                HitObject->RayDir = Vector3::Normalized(EnemyLocation - PlayerLocation);
                HitObject->HitGroup = 1;
                HitObject->ViewBlocked = false;
            }
        }
    }
    return old_BLAGCMCGEJG1(ist, HitObject);
}
void InitPatches() {

}

extern GetTouch_t old_GetTouch;

int hook_GetTouch(void* instance, int fingerId) {
    return 0;
}
