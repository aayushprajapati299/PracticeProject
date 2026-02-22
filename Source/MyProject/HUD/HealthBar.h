

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBar.generated.h"


UCLASS()
class MYPROJECT_API UHealthBar : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthBarPercentage(float Percentage);
private:
	UPROPERTY(EditAnywhere)
	class UHealthBarWidget* HealthBarWidget;	
	
};
