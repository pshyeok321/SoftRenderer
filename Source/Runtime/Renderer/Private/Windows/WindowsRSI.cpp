
#include "Precompiled.h"
#include "Windows/WindowsRSI.h"
#include "Windows/WindowsGDI.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor& InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor& InColor)
{
	if (InX < 0 || InX >= _ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < _ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor& InColor)
{
	if (InY < 0 || InY >= _ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < _ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(_ScreenSize, InVectorPos), InColor);
}

// Center = 0, Left = 1, Right = 2, Bot = 4, Top = 8
int WindowsRSI::TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	int result = Center;

	if (InVectorPos.X < InMinPos.X) result = Left;
	else if (InVectorPos.X > InMaxPos.X) result = Right;
	if (InVectorPos.Y < InMinPos.Y) result = Bot;
	else if (InVectorPos.Y > InMaxPos.Y) result = Top;

	return result;
}

bool WindowsRSI::CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	float x0 = InOutStartPos.X;
	float x1 = InOutEndPos.X;

	float y0 = InOutStartPos.Y;
	float y1 = InOutEndPos.Y;

	int tr0 = TestRegion(InOutStartPos, InMinPos, InMaxPos);
	int tr1 = TestRegion(InOutEndPos, InMinPos, InMaxPos);

	bool accept = false;

	while (true) {
		if ((tr0 | tr1) == 0) {
			accept = true;
			break;
		}
		else if ((tr0 & tr1) > 0) {
			//시작과 끝이 측면에 있음.
			break;
		}
		else {
			float x = 0;
			float y = 0;
			int trOut = tr0 > 0 ? tr0 : tr1;

			float k = (y1 - y0) / (x1 - x0);

			// y = y0 + k(x-x0)
		   //  x = x0 + 1/k*(y-y0)
			if ((trOut & Top) > 0) {
				y = InMaxPos.Y;
				x = x0 + (y - y0) / k;
			}
			else if ((trOut & Bot) > 0) {
				y = InMinPos.Y;
				x = x0 + (y - y0) / k;
			}
			else if ((trOut & Left) > 0) {
				x = InMinPos.X;
				y = y0 + (x - x0) * k;
			}
			else if ((trOut & Right) > 0) {
				x = InMaxPos.X;
				y = y0 + (x - x0) * k;
			}

			if (trOut == tr0) {
				x0 = x;
				y0 = y;
				tr0 = TestRegion(Vector2(x, y), InMinPos, InMaxPos);
			}
			else
			{
				x1 = x;
				y1 = y;
				tr1 = TestRegion(Vector2(x, y), InMinPos, InMaxPos);
			}
		}
	}
	if (accept) {
		InOutStartPos.X = x0;
		InOutStartPos.Y = y0;

		InOutEndPos.X = x1;
		InOutEndPos.Y = y1;
	}
	return accept;
}

void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	Vector2 clippedStart = InStartPos;
	Vector2 clippedEnd = InEndPos;
	Vector2 screenExtend = Vector2(_ScreenSize.X, _ScreenSize.Y) * 0.5f;
	Vector2 minScreen = -screenExtend;
	Vector2 maxScreen = screenExtend;
	if (!CohenSutherlandLineClip(clippedStart, clippedEnd, minScreen, maxScreen))
	{
		return;
	}

	ScreenPoint startPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedStart);
	ScreenPoint endPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedEnd);

	PushStatisticText("Cohen-Sutherland" + clippedEnd.ToString());

	int width = endPosition.X - startPosition.X;
	int height = endPosition.Y - startPosition.Y;

	bool isGradualSlope = (Math::Abs(width) >= Math::Abs(height));
	int dx = (width >= 0) ? 1 : -1;
	int dy = (height > 0) ? 1 : -1;
	int fw = dx * width;
	int fh = dy * height;

	int f = isGradualSlope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = isGradualSlope ? 2 * fh : 2 * fw;
	int f2 = isGradualSlope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = startPosition.X;
	int y = startPosition.Y;

	if (isGradualSlope)
	{
		while (x != endPosition.X)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += dy;
			}

			x += dx;
		}
	}
	else
	{
		while (y != endPosition.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dx;
			}

			y += dy;
		}
	}
}

void WindowsRSI::PushStatisticText(std::string&& InText)
{
	_StatisticTexts.emplace_back(InText);
}

void WindowsRSI::PushStatisticTexts(std::vector<std::string>&& InTexts)
{
	std::move(InTexts.begin(), InTexts.end(), std::back_inserter(_StatisticTexts));
}
