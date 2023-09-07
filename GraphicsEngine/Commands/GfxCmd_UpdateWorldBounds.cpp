#include "GraphicsEngine.pch.h"
#include "GfxCmd_UpdateWorldBounds.h"
#include "GraphicsEngine.h"

GfxCmd_UpdateWorldBounds::GfxCmd_UpdateWorldBounds(const CommonUtilities::Vector3f& aMin, const CommonUtilities::Vector3f& aMax): GraphicsCommand(RenderStage::Light), myMin(aMin), myMax(aMax)
{
}

void GfxCmd_UpdateWorldBounds::Execute()
{
	CommonUtilities::Vector3f& engineMin = GetWorldBoundsMin();
	CommonUtilities::Vector3f& engineMax = GetWorldBoundsMax();
	bool boundsUpdated = false;
	if (myMin.x < engineMin.x)
	{
		engineMin.x = myMin.x;
		boundsUpdated = true;
	}
	if (myMin.y < engineMin.y)
	{
		engineMin.y = myMin.y;
		boundsUpdated = true;
	}
	if (myMin.z < engineMin.z)
	{
		engineMin.z = myMin.z;
		boundsUpdated = true;
	}

	if (myMax.x > engineMax.x)
	{
		engineMax.x = myMax.x;
		boundsUpdated = true;
	}
	if (myMax.y > engineMax.y)
	{
		engineMax.y = myMax.y;
		boundsUpdated = true;
	}
	if (myMax.z > engineMax.z)
	{
		engineMax.z = myMax.z;
		boundsUpdated = true;
	}

	if (boundsUpdated)
	{
		GetWorldBoundsOrigin() = (engineMin + engineMax) * 0.5f;
		GetWorldBoundsRadius() = ((engineMax - engineMin) * 0.5f).Length();
	}
}
