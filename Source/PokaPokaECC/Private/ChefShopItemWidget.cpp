#include "ChefShopItemWidget.h"
#include "PokaPokaECCGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UChefShopItemWidget::InitItem(UUSkillDataAsset* InSkill)
{
    SkillData = InSkill;
    OnItemInitialized(); // BPに見た目の更新を指示
}

void UChefShopItemWidget::OnBuyClicked()
{
    if (!SkillData) return;

    // ★ GameInstanceに購入処理を依頼
    UPokaPokaECCGameInstance* GameInst = Cast<UPokaPokaECCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInst)
    {
        bool bSuccess = GameInst->BuySkill(SkillData);

        if (bSuccess)
        {
            // 例：購入成功したらボタンを無効化する等の処理をBPで行えるようにする
            // OnPurchaseSuccessful(); 
        }
    }
}