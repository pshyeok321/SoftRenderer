
#pragma once

#include <memory>
#include "WindowsGDI.h"
#include "RenderingSoftwareInterface.h"

class WindowsGDI;
class WindowsRSI : public WindowsGDI, public RenderingSoftwareInterface
{
public:
	WindowsRSI() = default;
	~WindowsRSI();

public:
	virtual bool Init(const ScreenPoint& InScreenSize) override;
	virtual void Shutdown() override;
	virtual bool IsInitialized() const { return _GDIInitialized; }

	virtual void Clear(const LinearColor& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor) override;
	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor) override;

	virtual void DrawFullVerticalLine(int InX, const LinearColor& InColor) override;
	virtual void DrawFullHorizontalLine(int InY, const LinearColor& InColor) override;

	virtual void PushStatisticText(std::string && InText) override;
	virtual void PushStatisticTexts(std::vector<std::string> && InTexts) override;
	int Center = 0;
	int Left = 1;
	int Right = 2;
	int Bot = 4;
	int Top = 8;
private:
	FORCEINLINE void SetPixel(const ScreenPoint& InPos, const LinearColor& InColor);
	int TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos);
	bool CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos);

};

FORCEINLINE void WindowsRSI::SetPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	SetPixelOpaque(InPos, InColor);
}
