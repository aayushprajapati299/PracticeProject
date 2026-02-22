#pragma once

#include "DrawDebugHelpers.h"

#define Draw_Sphere(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(),Location ,30.f, 20, FColor::Color, true, 60.f);

#define Draw_Sphere_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location ,30.f, 20, FColor::Red, false, -1.f);

#define Draw_Line(Start,End) if(GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1.f,0,1.f);

#define Draw_Point(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f,FColor::Red,true);

#define Draw_Vector(Start,End) if (GetWorld()) { \
	DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1.f,0,1.f); \
	DrawDebugPoint(GetWorld(),End,15.f,FColor::Red,true); \
}

#define Draw_Vector_SingleFrame(Start,End) if (GetWorld()) { \
	DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, -1.f,0,1.f); \
	DrawDebugPoint(GetWorld(),End,15.f,FColor::Red,false,-1.f); \
}
 
#define Draw_Point_SingeFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15.f, FColor::Red, false, -1.0f);

