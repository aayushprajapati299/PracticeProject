// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"


void UHealthBar::SetHealthBarPercentage(float Percentage)
{
	if (HealthBarWidget==nullptr)
	{
		HealthBarWidget= Cast<UHealthBarWidget>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percentage);
	}
}
