#pragma once;

class Transform
{
public:
	Transform() = default;

public:
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void AddPosition(const Vector2& InDeltaPosition) { Position += InDeltaPosition; }
	void SetScale(const Vector2& InScale) { Scale = InScale; }
	void SetRotation(float InDegree) { Rotation = InDegree; CalculateLocalAxis(); }

	Vector2 GetPosition() const { return Position; }
	Vector2 GetScale() const { return Scale; }
	float GetRotation() const { return Rotation; }

	// Rendering Pipeline
	Matrix3x3 GetModelingMatrix() const;

private:
	void CalculateLocalAxis();

	Vector2 Position = Vector2::Zero;
	float Rotation = 0.f;
	Vector2 Scale = Vector2::One;

	Vector2 Right = Vector2::UnitX;
	Vector2 Up = Vector2::UnitY;

};