#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USkillDataAsset.h"
#include "ChefShopItemWidget.generated.h"

UCLASS()
class POKAPOKAECC_API UChefShopItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 親ウィジェットから呼ばれて、自身のスキルデータをセットする
    UFUNCTION(BlueprintCallable, Category = "Shop Item")
    void InitItem(UUSkillDataAsset* InSkill);

    // 購入ボタン（緑の枠）がクリックされた時にBPから呼ぶ
    UFUNCTION(BlueprintCallable, Category = "Shop Item")
    void OnBuyClicked();

protected:
    // この枠に割り当てられたスキルデータ
    UPROPERTY(BlueprintReadOnly, Category = "Shop Item")
    UUSkillDataAsset* SkillData;

    // BP側でテキストやアイコンの表示を更新するためのイベント
    UFUNCTION(BlueprintImplementableEvent, Category = "Shop Item")
    void OnItemInitialized();
};
