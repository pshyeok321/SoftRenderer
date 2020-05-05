
#include "Precompiled.h"
#include "SoftRenderer.h"

// �׸��� �׸���
void SoftRenderer::DrawGrid2D()
{
	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���� �׸���
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize._X; x <= _ScreenSize._X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize._X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize._X - x, gridColor);
		}
	}

	for (int y = screenHalfSize._Y; y <= _ScreenSize._Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize._Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize._Y - y, gridColor);
		}
	}

	// ���� �� �׸���
	_RSI->DrawFullHorizontalLine(screenHalfSize._Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize._X, LinearColor::Green);
}


// ���� ���� 
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// Matrix ���� �ȵ�.

	// ��� �ð� ����
	static float _time = 0.f;
	
	static bool isOne, isTwo = false;
	float halfPiSin = sinf(_time * Math::HalfPI);
	float twoPISin = sinf(_time * Math::TwoPI);
	float piSin = sinf(_time * Math::PI);

	if(!isOne && !isTwo)
		_time += InDeltaSeconds;
	if (isOne && !isTwo)
		_time -= InDeltaSeconds;
	if (isOne && isTwo)
		_time += InDeltaSeconds;

	if(_time > 4 && !isOne && !isTwo) {
		isOne = true;
	}
	if (_time < 0 && isOne && !isTwo) {
		isTwo = true;
	}
	if (_time > 4 && isOne && isTwo) {
		_time = 0;  isOne = false;	 isTwo = false;
	}
	
	if (!isOne && !isTwo) {
		_Transform.SetPosition(Vector2(_time, halfPiSin));
		_CurrentColor = LinearColor::Red;
	}
	if (isOne && !isTwo) {
		_Transform.SetPosition(Vector2(_time, twoPISin));
		_CurrentColor = LinearColor::Blue;
	}
	if (isOne && isTwo) {
		_Transform.SetPosition(Vector2(_time, piSin));
		_CurrentColor = LinearColor::Black;
	}	
}

// ������ ����
void SoftRenderer::Render2D()
{
	//Matrix �ᵵ ��.
	// ���� �׸���
	DrawGrid2D();

	Vector3 originVector(0.f, 0.f, 1.f);
	// 18�� �������� 20���� ���� �׸���.
	for (int i = 0; i < 20; i++)
	{
		float rotateRadian = Math::Deg2Rad(i * 18.f);
		float sin = sinf(rotateRadian);
		float cos = cosf(rotateRadian);
		Vector3 newXAxis(cos, sin, 0);
		Vector3 newYAxis(-sin, cos, 0);
		Matrix3x3 rotateMatrix(newXAxis, newYAxis, Vector3(0.f, 0.f, 1.f));

		Vector2 newPosition = (rotateMatrix * _Transform.GetModelingMatrix() * _Transform.GetPosition()) * 10.f;

		// ������ ��ġ�� ������ �������� �� ���
		_RSI->DrawPoint(newPosition, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitX, _CurrentColor);
		_RSI->DrawPoint(newPosition + Vector2::UnitY, _CurrentColor);
		_RSI->DrawPoint(newPosition - Vector2::UnitY, _CurrentColor);
	}
}