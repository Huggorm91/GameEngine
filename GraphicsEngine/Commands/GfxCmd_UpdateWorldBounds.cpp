#include "GraphicsEngine.pch.h"
#include "GfxCmd_UpdateWorldBounds.h"
#include "GraphicsEngine.h"

GfxCmd_UpdateWorldBounds::GfxCmd_UpdateWorldBounds(const CommonUtilities::Vector3f& aMin, const CommonUtilities::Vector3f& aMax): myMin(aMin), myMax(aMax)
{
}

void GfxCmd_UpdateWorldBounds::Execute()
{
	CommonUtilities::Vector3f& engineMin = GetWorldBoundsMin();
	CommonUtilities::Vector3f& engineMax = GetWorldBoundsMax();
	if (myMin.x < engineMin.x)
	{
		engineMin.x = myMin.x;
	}
	if (myMin.y < engineMin.y)
	{
		engineMin.y = myMin.y;
	}
	if (myMin.z < engineMin.z)
	{
		engineMin.z = myMin.z;
	}

	if (myMax.x > engineMax.x)
	{
		engineMax.x = myMax.x;
	}
	if (myMax.y > engineMax.y)
	{
		engineMax.y = myMax.y;
	}
	if (myMax.z > engineMax.z)
	{
		engineMax.z = myMax.z;
	}
}
