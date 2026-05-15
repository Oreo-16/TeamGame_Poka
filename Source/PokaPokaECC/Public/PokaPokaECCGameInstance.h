#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "USkillDataAsset.h"
#include "PokaPokaECCGameInstance.generated.h"

/**
 * シーン（レベル）を跨いでデータを保持・管理するためのGameInstance
 */
UCLASS()
class POKAPOKAECC_API UPokaPokaECCGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // ゲーム全体のスキルリスト（エディタで設定）
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Data")
    TArray<UUSkillDataAsset*> MasterSkillList;

    // プレイヤーがこれまでに取得したスキル
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Data")
    TArray<UUSkillDataAsset*> AcquiredSkills;

    // プレイヤーの所持金
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
    int32 CurrentMoney = 0;

    // スキルを購入する処理
    UFUNCTION(BlueprintCallable, Category = "Game Data")
    bool BuySkill(UUSkillDataAsset* SkillToBuy);

    // ショップ画面に並べるためのスキルを8つ抽出する処理
    UFUNCTION(BlueprintCallable, Category = "Game Data")
    TArray<UUSkillDataAsset*> GetShopSkills();
};