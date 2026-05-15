#include "ChefShopWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "ChefShopItemWidget.h"
#include "PokaPokaECCGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UChefShopWidget::InitShopItems()
{
    if (!ItemGrid || !ItemWidgetClass) return;

    // 前回の表示が残っていればクリア
    ItemGrid->ClearChildren();

    // ★ GameInstanceを取得
    UPokaPokaECCGameInstance* GameInst = Cast<UPokaPokaECCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInst) return;

    // GameInstanceから8つのスキルを取得
    TArray<UUSkillDataAsset*> ShopSkills = GameInst->GetShopSkills();

    // 取得したスキルをグリッドに配置
    for (int32 i = 0; i < ShopSkills.Num(); ++i)
    {
        UUSkillDataAsset* Skill = ShopSkills[i];
        if (!Skill) continue;

        UChefShopItemWidget* ItemWidget = CreateWidget<UChefShopItemWidget>(this, ItemWidgetClass);
        if (ItemWidget)
        {
            ItemWidget->InitItem(Skill);

            // 横4個の計算 (i=0~3はRow=0, i=4~7はRow=1)
            int32 Row = i / 4;
            int32 Col = i % 4;

            UUniformGridSlot* GridSlot = ItemGrid->AddChildToUniformGrid(ItemWidget, Row, Col);
            if (GridSlot)
            {
                GridSlot->SetHorizontalAlignment(HAlign_Fill);
                GridSlot->SetVerticalAlignment(VAlign_Fill);
            }
        }
    }
}