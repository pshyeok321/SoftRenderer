
#include  "Precompiled.h"
#include "Transform.h"


Matrix3x3 Transform::GetModelingMatrix() const
{
	/* �̵� ��ȯ�� ���
	1 0 a
	0 1 b
	0 0 1 	*/
	Vector3 transCols(Position._X, Position._Y, 1);
	Matrix3x3 translateMatrix(Vector3::UnitX, Vector3::UnitY, transCols);
	
	/*  ȸ�� ��ȯ�� ���
		c-s 0
		s c 0
		0 0 1 	*/
	float rot = Math::Deg2Rad(Rotation);
	Vector3 rotX(cosf(rot), sinf(rot), 0);
	Vector3 rotY(-sinf(rot), cosf(rot), 0);
	Matrix3x3 rotateMatrix(rotX, rotY, Vector3::UnitZ);

	/*  ũ�� ��ȯ�� ���
		a 0 0
		0 b 0
		0 0 1 	*/
	Matrix3x3 scaleMatrix(Vector3::UnitX * Scale._X, Vector3::UnitY * Scale._Y, Vector3::UnitZ);

	return translateMatrix * rotateMatrix * scaleMatrix;
}

void Transform::CalculateLocalAxis()
{	
	float cos = cosf(Math::Deg2Rad(Rotation));
	float sin = sinf(Math::Deg2Rad(Rotation));
	Right = Vector2(cos, sin); //right���ʹ� x��  c     s
	Up = Vector2(-sin, cos); // up���ʹ� y��     -s     c		
}
