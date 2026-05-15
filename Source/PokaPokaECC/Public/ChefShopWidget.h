#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USkillDataAsset.h"
#include "ChefShopWidget.generated.h"

class UUniformGridPanel;
class UChefShopItemWidget;

UCLASS()
class POKAPOKAECC_API UChefShopWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // GameInstanceからデータを取ってショップを初期化する
    UFUNCTION(BlueprintCallable, Category = "Shop UI")
    void InitShopItems();

protected:
    // UMGの "ItemGrid" という名前のUniformGridPanelと自動で紐づく
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ItemGrid;

    // スポーンさせる緑の枠（WBP_ShopItem）のクラスをエディタで指定する
    UPROPERTY(EditAnywhere, Category = "Shop UI")
    TSubclassOf<UChefShopItemWidget> ItemWidgetClass;
};