//
// This file is part of the Terathon Math Library, by Eric Lengyel.
// Copyright 1999-2022, Terathon Software LLC
//
// This software is licensed under the GNU General Public License version 3.
// Separate proprietary licenses are available from Terathon Software.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
// EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//


#include "TSVector2D.h"


using namespace Terathon;


const ConstVector2D Vector2D::zero = {0.0F, 0.0F};
const ConstPoint2D Origin2D::origin = {0.0F, 0.0F};
const Origin2D Point2D::origin = {};

const ConstVector2D Vector2D::x_unit = {1.0F, 0.0F};
const ConstVector2D Vector2D::y_unit = {0.0F, 1.0F};

const ConstVector2D Vector2D::minus_x_unit = {-1.0F, 0.0F};
const ConstVector2D Vector2D::minus_y_unit = {0.0F, -1.0F};


Vector2D& Vector2D::Rotate(float angle)
{
	Vector2D t = CosSin(angle);
	float nx = t.x * x - t.y * y;
	float ny = t.y * x + t.x * y;

	x = nx;
	y = ny;
	return (*this);
}
