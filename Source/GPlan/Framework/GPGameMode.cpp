//Copyright GPlan Project. All Rights Reserved.

#include "Framework/GPGameMode.h"
#include "GPlan.h"


AGPGameMode::AGPGameMode()
{

}

void AGPGameMode::NotifyEnemyKilled(AActor* EnemyActor)
{
    ++EnemiesKilled;

    UE_LOG(LogGP, Display, TEXT("Enemy killed: %s | Total Kills: %d"), *GetNameSafe(EnemyActor), EnemiesKilled);

    if (!bGameWon && KillsToWin > 0 && EnemiesKilled >= KillsToWin)
    {
        bGameWon = true;
        UE_LOG(LogGP, Display, TEXT("Level Cleared!"));
        OnGameWon.Broadcast(); 
    }
}
