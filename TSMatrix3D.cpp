//
// This file is part of the Terathon Math Library, by Eric Lengyel.
// Copyright 1999-2025, Terathon Software LLC
//
// This software is distributed under the MIT License.
// Separate proprietary licenses are available from Terathon Software.
//


#include "TSMatrix3D.h"


using namespace Terathon;


alignas(64) const ConstMatrix3D Matrix3D::identity = {{{1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}}};
alignas(64) const ConstTransform2D Transform2D::identity = {{{1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}}};


Matrix3D::Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22) : Mat3D<TypeMatrix3D>(n00, n01, n02, n10, n11, n12, n20, n21, n22)
{
}

Matrix3D::Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	col0 = a;
	col1 = b;
	col2 = c;
}

Matrix3D& Matrix3D::Set(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22)
{
	matrix.Set(n00, n01, n02, n10, n11, n12, n20, n21, n22);
	return (*this);
}

Matrix3D& Matrix3D::Set(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	col0 = a;
	col1 = b;
	col2 = c;
	return (*this);
}

Matrix3D& Matrix3D::operator *=(const Matrix3D& m)
{
	float t = m00 * m(0,0) + m01 * m(1,0) + m02 * m(2,0);
	float u = m00 * m(0,1) + m01 * m(1,1) + m02 * m(2,1);
	m02 = m00 * m(0,2) + m01 * m(1,2) + m02 * m(2,2);
	m00 = t;
	m01 = u;

	t = m10 * m(0,0) + m11 * m(1,0) + m12 * m(2,0);
	u = m10 * m(0,1) + m11 * m(1,1) + m12 * m(2,1);
	m12 = m10 * m(0,2) + m11 * m(1,2) + m12 * m(2,2);
	m10 = t;
	m11 = u;

	t = m20 * m(0,0) + m21 * m(1,0) + m22 * m(2,0);
	u = m20 * m(0,1) + m21 * m(1,1) + m22 * m(2,1);
	m22 = m20 * m(0,2) + m21 * m(1,2) + m22 * m(2,2);
	m20 = t;
	m21 = u;

	return (*this);
}

Matrix3D& Matrix3D::operator *=(float s)
{
	matrix *= s;
	return (*this);
}

Matrix3D& Matrix3D::operator /=(float s)
{
	matrix /= s;
	return (*this);
}

Matrix3D& Matrix3D::SetIdentity(void)
{
	m00 = m11 = m22 = 1.0F;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.0F;
	return (*this);
}

Matrix3D& Matrix3D::Orthogonalize(int32 column)
{
	Vector3D& x = (*this)[column];
	Vector3D& y = (*this)[(column < 2) ? column + 1 : 0];
	Vector3D& z = (*this)[(column > 0) ? column - 1 : 2];

	x.Normalize();
	y = Normalize(y - x * Dot(x, y));
	z = Normalize(z - x * Dot(x, z) - y * Dot(y, z));

	return (*this);
}

void Matrix3D::GetEulerAngles(float *x, float *y, float *z) const
{
	float sy = m02;
	if (sy < 1.0F)
	{
		if (sy > -1.0F)
		{
			*x = -Arctan(m12, m22);
			*y = Arcsin(sy);
			*z = -Arctan(m01, m00);
		}
		else
		{
			*x = 0.0F;
			*y = -Math::tau_over_4;
			*z = Arctan(m10, m11);
		}
	}
	else
	{
		*x = 0.0F;
		*y = Math::tau_over_4;
		*z = Arctan(m10, m11);
	}
}

Matrix3D& Matrix3D::SetEulerAngles(float x, float y, float z)
{
	Vector2D xrot = CosSin(x);
	Vector2D yrot = CosSin(y);
	Vector2D zrot = CosSin(z);

	m00 = yrot.x * zrot.x;
	m10 = xrot.x * zrot.y + xrot.y * yrot.y * zrot.x;
	m20 = xrot.y * zrot.y - xrot.x * yrot.y * zrot.x;
	m01 = -yrot.x * zrot.y;
	m11 = xrot.x * zrot.x - xrot.y * yrot.y * zrot.y;
	m21 = xrot.y * zrot.x + xrot.x * yrot.y * zrot.y;
	m02 = yrot.y;
	m12 = -xrot.y * yrot.x;
	m22 = xrot.x * yrot.x;

	return (*this);
}

Matrix3D Matrix3D::MakeRotationX(float angle)
{
	Vector2D v = CosSin(angle);

	return (Matrix3D(1.0F, 0.0F, 0.0F,
	                 0.0F,  v.x, -v.y,
	                 0.0F,  v.y,  v.x));
}

Matrix3D Matrix3D::MakeRotationY(float angle)
{
	Vector2D v = CosSin(angle);

	return (Matrix3D( v.x, 0.0F,  v.y,
	                 0.0F, 1.0F, 0.0F,
	                 -v.y, 0.0F,  v.x));
}

Matrix3D Matrix3D::MakeRotationZ(float angle)
{
	Vector2D v = CosSin(angle);

	return (Matrix3D( v.x, -v.y, 0.0F,
	                  v.y,  v.x, 0.0F,
	                 0.0F, 0.0F, 1.0F));
}

Matrix3D Matrix3D::MakeRotation(float angle, const Bivector3D& axis)
{
	float	c, s;

	CosSin(angle, &c, &s);
	float d = 1.0F - c;

	float x = axis.x * d;
	float y = axis.y * d;
	float z = axis.z * d;
	float axay = x * axis.y;
	float axaz = x * axis.z;
	float ayaz = y * axis.z;

	return (Matrix3D(c + x * axis.x, axay - s * axis.z, axaz + s * axis.y,
	                 axay + s * axis.z, c + y * axis.y, ayaz - s * axis.x,
	                 axaz - s * axis.y, ayaz + s * axis.x, c + z * axis.z));
}

Matrix3D Matrix3D::MakeReflection(const Vector3D& a)
{
	float x = a.x * -2.0F;
	float y = a.y * -2.0F;
	float z = a.z * -2.0F;
	float axay = x * a.y;
	float axaz = x * a.z;
	float ayaz = y * a.z;

	return (Matrix3D(x * a.x + 1.0F, axay, axaz,
	                 axay, y * a.y + 1.0F, ayaz,
	                 axaz, ayaz, z * a.z + 1.0F));
}

Matrix3D Matrix3D::MakeInvolution(const Vector3D& a)
{
	float x = a.x * 2.0F;
	float y = a.y * 2.0F;
	float z = a.z * 2.0F;
	float axay = x * a.y;
	float axaz = x * a.z;
	float ayaz = y * a.z;

	return (Matrix3D(x * a.x - 1.0F, axay, axaz,
	                 axay, y * a.y - 1.0F, ayaz,
	                 axaz, ayaz, z * a.z - 1.0F));
}

Matrix3D Matrix3D::MakeScale(float scale)
{
	return (Matrix3D(scale, 0.0F, 0.0F, 0.0F, scale, 0.0F, 0.0F, 0.0F, scale));
}

Matrix3D Matrix3D::MakeScale(float sx, float sy, float sz)
{
	return (Matrix3D(sx, 0.0F, 0.0F, 0.0F, sy, 0.0F, 0.0F, 0.0F, sz));
}

Matrix3D Matrix3D::MakeScale(float scale, const Vector3D& a)
{
	scale -= 1.0F;
	float x = a.x * scale;
	float y = a.y * scale;
	float z = a.z * scale;
	float axay = x * a.y;
	float axaz = x * a.z;
	float ayaz = y * a.z;

	return (Matrix3D(x * a.x + 1.0F, axay, axaz,
	                 axay, y * a.y + 1.0F, ayaz,
	                 axaz, ayaz, z * a.z + 1.0F));
}

Matrix3D Matrix3D::MakeScaleX(float sx)
{
	return (Matrix3D(sx, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F));
}

Matrix3D Matrix3D::MakeScaleY(float sy)
{
	return (Matrix3D(1.0F, 0.0F, 0.0F, 0.0F, sy, 0.0F, 0.0F, 0.0F, 1.0F));
}

Matrix3D Matrix3D::MakeScaleZ(float sz)
{
	return (Matrix3D(1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, sz));
}

Matrix3D Matrix3D::MakeSkew(float angle, const Vector3D& a, const Vector3D& b)
{
	float t = Tan(angle);
	float x = a.x * t;
	float y = a.y * t;
	float z = a.z * t;

	return (Matrix3D(x * b.x + 1.0F, x * b.y, x * b.z,
	                 y * b.x, y * b.y + 1.0F, y * b.z,
	                 z * b.x, z * b.y, z * b.z + 1.0F));
}


Matrix3D Terathon::operator *(const Matrix3D& m, float s)
{
	return (Matrix3D(m(0,0) * s, m(0,1) * s, m(0,2) * s, m(1,0) * s, m(1,1) * s, m(1,2) * s, m(2,0) * s, m(2,1) * s, m(2,2) * s));
}

Matrix3D Terathon::operator /(const Matrix3D& m, float s)
{
	s = 1.0F / s;
	return (Matrix3D(m(0,0) * s, m(0,1) * s, m(0,2) * s, m(1,0) * s, m(1,1) * s, m(1,2) * s, m(2,0) * s, m(2,1) * s, m(2,2) * s));
}

float Terathon::Determinant(const Matrix3D& m)
{
	return (m(0,0) * (m(1,1) * m(2,2) - m(1,2) * m(2,1)) - m(0,1) * (m(1,0) * m(2,2) - m(1,2) * m(2,0)) + m(0,2) * (m(1,0) * m(2,1) - m(1,1) * m(2,0)));
}

Matrix3D Terathon::Inverse(const Matrix3D& m)
{
	const Vector3D& a = m[0];
	const Vector3D& b = m[1];
	const Vector3D& c = m[2];

	Bivector3D g0 = b ^ c;
	Bivector3D g1 = c ^ a;
	Bivector3D g2 = a ^ b;

	float invDet = 1.0F / (g2 ^ c);

	return (Matrix3D(g0.x * invDet, g0.y * invDet, g0.z * invDet,
	                 g1.x * invDet, g1.y * invDet, g1.z * invDet,
	                 g2.x * invDet, g2.y * invDet, g2.z * invDet));
}

Matrix3D Terathon::Adjugate(const Matrix3D& m)
{
	const Vector3D& a = m[0];
	const Vector3D& b = m[1];
	const Vector3D& c = m[2];

	Bivector3D g0 = b ^ c;
	Bivector3D g1 = c ^ a;
	Bivector3D g2 = a ^ b;

	return (Matrix3D(g0.x, g0.y, g0.z, g1.x, g1.y, g1.z, g2.x, g2.y, g2.z));
}


Transform2D::Transform2D(float n00, float n01, float n02, float n10, float n11, float n12)
{
	m00 = n00;
	m01 = n01;
	m02 = n02;
	m10 = n10;
	m11 = n11;
	m12 = n12;

	row2.Set(0.0F, 0.0F, 1.0F);
}

Transform2D::Transform2D(const Vector2D& a, const Vector2D& b, const Point2D& p)
{
	m00 = a.x;
	m10 = a.y;
	m01 = b.x;
	m11 = b.y;
	m02 = p.x;
	m12 = p.y;

	row2.Set(0.0F, 0.0F, 1.0F);
}

Transform2D::Transform2D(const Vector2D& r0, float n02, const Vector2D& r1, float n12)
{
	row0.Set(r0.x, r0.y, n02);
	row1.Set(r1.x, r1.y, n12);
	row2.Set(0.0F, 0.0F, 1.0F);
}

Transform2D::Transform2D(const Matrix2D& m)
{
	m00 = m.m00;
	m10 = m.m10;
	m01 = m.m01;
	m11 = m.m11;

	m02 = m12 = 0.0F;
	row2.Set(0.0F, 0.0F, 1.0F);
}

Transform2D::Transform2D(const Matrix2D& m, const Vector2D& v)
{
	m00 = m.m00;
	m10 = m.m10;
	m01 = m.m01;
	m11 = m.m11;
	m02 = v.x;
	m12 = v.y;

	row2.Set(0.0F, 0.0F, 1.0F);
}

Transform2D& Transform2D::Set(float n00, float n01, float n02, float n10, float n11, float n12)
{
	m00 = n00;
	m01 = n01;
	m02 = n02;
	m10 = n10;
	m11 = n11;
	m12 = n12;

	row2.Set(0.0F, 0.0F, 1.0F);
	return (*this);
}

Transform2D& Transform2D::Set(const Vector2D& a, const Vector2D& b, const Point2D& p)
{
	m00 = a.x;
	m10 = a.y;
	m01 = b.x;
	m11 = b.y;
	m02 = p.x;
	m12 = p.y;

	row2.Set(0.0F, 0.0F, 1.0F);
	return (*this);
}

Transform2D& Transform2D::Set(const Matrix2D& m, const Vector2D& v)
{
	m00 = m.m00;
	m10 = m.m10;
	m01 = m.m01;
	m11 = m.m11;
	m02 = v.x;
	m12 = v.y;

	row2.Set(0.0F, 0.0F, 1.0F);
	return (*this);
}

Transform2D& Transform2D::operator *=(const Transform2D& m)
{
	float x = m00;
	float y = m01;
	m00 = x * m.m00 + y * m.m10;
	m01 = x * m.m01 + y * m.m11;
	m02 = x * m.m02 + y * m.m12 + m02;

	x = m10;
	y = m11;
	m10 = x * m.m00 + y * m.m10;
	m11 = x * m.m01 + y * m.m11;
	m12 = x * m.m02 + y * m.m12 + m12;

	return (*this);
}

Transform2D& Transform2D::operator *=(const Matrix2D& m)
{
	float x = m00;
	float y = m01;
	m00 = x * m.m00 + y * m.m10;
	m01 = x * m.m01 + y * m.m11;

	x = m10;
	y = m11;
	m10 = x * m.m00 + y * m.m10;
	m11 = x * m.m01 + y * m.m11;

	return (*this);
}

Transform2D& Transform2D::SetMatrix2D(const Vector2D& a, const Vector2D& b)
{
	m00 = a.x;
	m10 = a.y;
	m01 = b.x;
	m11 = b.y;

	return (*this);
}

Transform2D& Transform2D::Orthogonalize(int32 column)
{
	Vector2D& x = (*this)[column];
	Vector2D& y = (*this)[1 - column];

	x.Normalize();
	y = Normalize(y - x * Dot(x, y));

	return (*this);
}

Transform2D Transform2D::MakeRotation(float angle)
{
	Vector2D v = CosSin(angle);

	return (Transform2D(v.x, -v.y, 0.0F,
	                    v.y,  v.x, 0.0F));
}

Transform2D Transform2D::MakeScale(float sx, float sy)
{
	return (Transform2D(sx, 0.0F, 0.0F, 0.0F, sy, 0.0F));
}

Transform2D Transform2D::MakeTranslation(const Vector2D& dv)
{
	return (Transform2D(1.0F, 0.0F, dv.x, 0.0F, 1.0F, dv.y));
}


Transform2D Terathon::operator *(const Transform2D& m1, const Transform2D& m2)
{
	return (Transform2D(m1(0,0) * m2(0,0) + m1(0,1) * m2(1,0),
	                    m1(0,0) * m2(0,1) + m1(0,1) * m2(1,1),
	                    m1(0,0) * m2(0,2) + m1(0,1) * m2(1,2) + m1(0,2),
	                    m1(1,0) * m2(0,0) + m1(1,1) * m2(1,0),
	                    m1(1,0) * m2(0,1) + m1(1,1) * m2(1,1),
	                    m1(1,0) * m2(0,2) + m1(1,1) * m2(1,2) + m1(1,2)));
}

Matrix3D Terathon::operator *(const Matrix3D& m1, const Transform2D& m2)
{
	return (Matrix3D(m1(0,0) * m2(0,0) + m1(0,1) * m2(1,0),
	                 m1(0,0) * m2(0,1) + m1(0,1) * m2(1,1),
	                 m1(0,0) * m2(0,2) + m1(0,1) * m2(1,2) + m1(0,2),
	                 m1(1,0) * m2(0,0) + m1(1,1) * m2(1,0),
	                 m1(1,0) * m2(0,1) + m1(1,1) * m2(1,1),
	                 m1(1,0) * m2(0,2) + m1(1,1) * m2(1,2) + m1(1,2),
	                 m1(2,0) * m2(0,0) + m1(2,1) * m2(1,0),
	                 m1(2,0) * m2(0,1) + m1(2,1) * m2(1,1),
	                 m1(2,0) * m2(0,2) + m1(2,1) * m2(1,2) + m1(2,2)));
}

Matrix3D Terathon::operator *(const Transform2D& m1, const Matrix3D& m2)
{
	return (Matrix3D(m1(0,0) * m2(0,0) + m1(0,1) * m2(1,0) + m1(0,2) * m2(2,0),
	                 m1(0,0) * m2(0,1) + m1(0,1) * m2(1,1) + m1(0,2) * m2(2,1),
	                 m1(0,0) * m2(0,2) + m1(0,1) * m2(1,2) + m1(0,2) * m2(2,2),
	                 m1(1,0) * m2(0,0) + m1(1,1) * m2(1,0) + m1(1,2) * m2(2,0),
	                 m1(1,0) * m2(0,1) + m1(1,1) * m2(1,1) + m1(1,2) * m2(2,1),
	                 m1(1,0) * m2(0,2) + m1(1,1) * m2(1,2) + m1(1,2) * m2(2,2),
	                 m2(2,0), m2(2,1), m2(2,2)));
}

Vector2D Terathon::operator *(const Transform2D& m, const Vector2D& v)
{
	return (m * v.xy);
}

Vector2D Terathon::operator *(const Vector2D& v, const Transform2D& m)
{
	return (v.xy * m);
}

Point2D Terathon::operator *(const Transform2D& m, const Point2D& p)
{
	return (Point2D(m(0,0) * p.x + m(0,1) * p.y + m(0,2),
	                m(1,0) * p.x + m(1,1) * p.y + m(1,2)));
}

float Terathon::Determinant(const Transform2D& m)
{
	return (m(0,0) * m(1,1) - m(0,1) * m(1,0));
}

Transform2D Terathon::Inverse(const Transform2D& m)
{
	float invDet = 1.0F / (m(0,0) * m(1,1) - m(0,1) * m(1,0));

	return (Transform2D(m(1,1) * invDet, -m(0,1) * invDet, (m(0,1) * m(1,2) - m(0,2) * m(1,1)) * invDet,
	                    -m(1,0) * invDet, m(0,0) * invDet, (m(0,2) * m(1,0) - m(0,0) * m(1,2)) * invDet));
}

Transform2D Terathon::InverseUnitDet(const Transform2D& m)
{
	return (Transform2D(m(1,1), -m(0,1), m(0,1) * m(1,2) - m(0,2) * m(1,1),
	                    -m(1,0), m(0,0), m(0,2) * m(1,0) - m(0,0) * m(1,2)));
}
