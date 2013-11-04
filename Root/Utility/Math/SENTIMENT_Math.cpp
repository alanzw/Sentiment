// Programmer: Wyatt Kennedy
// Date: 3/11/2013
// File: SYNC_Math.cpp
// This file contains the implements of the methods declared in SYNC_Math.h

#include "SENTIMENT_Math.h"

//-----------------------------------------
// Vector2 defintions
//-----------------------------------------

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const Vector2 & vec)
{
	x = vec.x;
	y = vec.y;
}

Vector2::Vector2(const float & ix, const float & iy)
{
	x = ix;
	y = iy;
}

Vector2::~Vector2()
{
}

void Vector2::Normal(Vector2 & rhs)
{
	rhs.x = y;
	rhs.y = -x;
}

Vector2 & Vector2::Normalize()
{
	if(x > 0.000001 || y > 0.000001)
	{
		float length = sqrt((x * x) + (y * y));
		x /= length;
		y /= length;
	}
	else
	{
		x = 0;
		y = 0;
	}

	return *this;
}

void Vector2::Normalize(Vector2 & rhs)
{
	if(x > 0.000001 || y > 0.000001)
	{
		float length = sqrt((x * x) + (y * y));
		rhs.x = x / length;
		rhs.y = y / length;
	}
	else
	{
		rhs.x = 0;
		rhs.y = 0;
	}
}

Vector2 & Vector2::Dot(const Vector2 & rhs1, const Vector2 & rhs2)
{
	x = rhs1.x * rhs2.x;
	y = rhs1.y * rhs2.y;

	return *this;
}

float Vector2::Cross(const Vector2 & rhs1, Vector2 & rhs2)
{
	return ((rhs1.x * rhs2.y) - (rhs1.y * rhs2.x));
}


//-----------------------------------------
// Vector3 defintions
//-----------------------------------------


Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const Vector3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


Vector3::Vector3(const float & ix, const float & iy, const float & iz)
{
	x = ix;
	y = iy;
	z = iz;
}


Vector3::~Vector3()
{
}


void Vector3::Normalize()
{
	if(x > 0.000001 || y > 0.000001 || z > 0.000001)
	{
		float length = sqrt((x * x) + (y * y) + (z * z));
		x /= length;
		y /= length;
		z /= length;
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
	}
}

void Vector3::Normalize(Vector3 rhs)
{
	if(x > 0.000001 || y > 0.000001 || z > 0.000001)
	{
		float length = sqrt((x * x) + (y * y) + (z * z));
		rhs.x /= length;
		rhs.y /= length;
		rhs.z /= length;
	}
	else
	{
		rhs.x = 0;
		rhs.y = 0;
		rhs.z = 0;
	}
}

void Vector3::Dot(const Vector3 & vec1, const Vector3 & vec2)
{
	x = vec1.x * vec2.x;
	y = vec1.y * vec2.y;
	z = vec1.z * vec2.z;
}

void Vector3::Cross(const Vector3 & vec1, const Vector3 & vec2)
{
	x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
	y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
	z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));
}

//-----------------------------------------
// Vector4 defintions
//-----------------------------------------

Vector4::Vector4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector4::Vector4(const Vector4 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

}

Vector4::Vector4(const float & ix, const float & iy, const float & iz, const float & iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}

Vector4::~Vector4()
{
}


//---------------------------------
// Quaternion definitions
//---------------------------------


Quaternion::Quaternion()
{
	v.x = 0;
	v.y = 0;
	v.z = 0;
	w = 0;
}

Quaternion::Quaternion(const Quaternion & rhs)
{
	v.x = rhs.v.x;
	v.y = rhs.v.y;
	v.z = rhs.v.z;
	w = rhs.w;
}

Quaternion::Quaternion(const Vector3 & iv, const float & iw)
{
	v = iv;
	w = iw;
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator*(Quaternion rhs)
{
	rhs.v.x = (w * rhs.v.x) + (v.x * rhs.w)  + (v.y * rhs.v.z) - (v.z * rhs.v.y);
	rhs.v.y = (w * rhs.v.y) - (v.x * rhs.v.z) + (v.y * rhs.w) + (v.z * rhs.v.x);
	rhs.v.z = (w * rhs.v.z) + (v.x * rhs.v.y) - (v.y * rhs.v.x) + (v.z * rhs.w);
	rhs.w = (w * rhs.w) - (v.x * rhs.v.x) - (v.y * rhs.v.y) - (v.z * rhs.v.z);

	return rhs;
}

Quaternion & Quaternion::mul(const Quaternion & rhs)
{
	v.x = (w * rhs.v.x) + (v.x * rhs.w)  + (v.y * rhs.v.z) - (v.z * rhs.v.y);
	v.y = (w * rhs.v.y) - (v.x * rhs.v.z) + (v.y * rhs.w) + (v.z * rhs.v.x);
	v.z = (w * rhs.v.z) + (v.x * rhs.v.y) - (v.y * rhs.v.x) + (v.z * rhs.w);
	w = (w * rhs.w) - (v.x * rhs.v.x) - (v.y * rhs.v.y) - (v.z * rhs.v.z);

	return *this;
}

void Quaternion::Conjugate()
{
	v *= -1;
}

void Quaternion::Conjugate(Quaternion & rhs)
{
	rhs.v = v * -1;
	rhs.w = w;
}

void Quaternion::Normalize()
{
	float length = sqrt((w*w) + (v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if(length > 0.000001)
	{
		v.x /= length;
		v.y /= length;
		v.z /= length;
		w /= length;
	}
	else
	{
		v.x = 0;
		v.y = 0;
		v.z = 0;
		w = 0;
	}
}

void Quaternion::Normalize(Quaternion & rhs)
{
	float length = sqrt((w*w) + (v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if(length > 0.000001)
	{
		rhs.v.x = v.x / length;
		rhs.v.y = v.y / length;
		rhs.v.z = v.z / length;
		rhs.w = w / length;
	}
	else
	{
		rhs.v.x = 0;
		rhs.v.y = 0;
		rhs.v.z = 0;
		rhs.w = 0;
	}
}



