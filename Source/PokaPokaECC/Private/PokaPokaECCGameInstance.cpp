#include "PokaPokaECCGameInstance.h"

bool UPokaPokaECCGameInstance::BuySkill(UUSkillDataAsset* SkillToBuy)
{
    if (!SkillToBuy) return false;

    // 価格プロパティ（Price）がUSkillDataAssetに定義されている前提
    //if (CurrentMoney >= SkillToBuy->Price)
    //{
    //    CurrentMoney -= SkillToBuy->Price;
    //    AcquiredSkills.Add(SkillToBuy); // 取得済みスキルに追加

    //    UE_LOG(LogTemp, Warning, TEXT("ショップでスキルを購入しました: %s"), *SkillToBuy->SkillName);
    //    return true; // 購入成功
    //}

    UE_LOG(LogTemp, Warning, TEXT("お金が足りません！"));
    return false; // 購入失敗
}

TArray<UUSkillDataAsset*> UPokaPokaECCGameInstance::GetShopSkills()
{
    TArray<UUSkillDataAsset*> Result;
    TArray<UUSkillDataAsset*> ShopPool = MasterSkillList;

    // リストをシャッフル
    for (int32 i = 0; i < ShopPool.Num(); ++i)
    {
        int32 Index = FMath::RandRange(i, ShopPool.Num() - 1);
        ShopPool.Swap(i, Index);
    }

    // ★ 8個抽出する
    for (int32 i = 0; i < FMath::Min(8, ShopPool.Num()); ++i)
    {
        Result.Add(ShopPool[i]);
    }

    return Result;
}