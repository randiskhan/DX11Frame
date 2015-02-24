// Hypotrochoid.h
// Declaration file for Hypotrochoid.

#pragma once

#include "CDX11Frame.h"
#include "IEntity.h"

struct DoublePoint
{
	double	x;
	double	y;
	double	a;
	double	d;
	DoublePoint()
	{
		x = y = a = d = 0.0;
	}
};

class Hypotrochoid : public IEntity
{
private:
	unique_ptr<PrimitiveBatch<VertexPositionColor>>	_pPrimtiveBatch;
	unique_ptr<BasicEffect>							_pBasicEffect;
	ID3D11InputLayout*								_pID3D11InputLayout;

	// Maxumum number of verticies. Used to declare arrays.
	// Max DTK can handle is 65534.
	static const int			_MaxVertices = 65534;
	// Number of verticies to update and render.
	int							_NumVertices;
	// Density of verticies in one revolution of inner circle along outer circle.
	int							_NumVerticesPerCycle;
	// Array of verticies to send to GPU.
	VertexPositionColor			_vertices[_MaxVertices];
	// Array of raw verticies in coordinate [-1, 1] range.
	DoublePoint					_verticesRaw[_MaxVertices];
	// Recalculate cycles each update?
	bool						_CalcCycles;
	// Add origin vertex to end of array to close pattern?
	bool						_CopyOriginToEnd;

	double	_AnimationDelay;

	// Parameters for Hypotrochoid.
	// Fixed circle radius: _Radius1
	// Moving circle radius: _Radius2
	// Drawing point distance from center of moving circle: _ArmLength
	// I find it easiest to "normalize" these values. I keep radius1 at 1.0, so I
	// can think of radius2 as a ratio to/of radius1. This also helps with
	// _ArmLength - If _Radius1 is 1, then when _ArmLength equals _Radius2,
	// The Drawing point is exactly on the cirumfrence of the moving circle.
	double		_Radius1;
	double		_Radius2;
	double		_ArmLength;
	double		_Cycles;

public:
	Hypotrochoid(CDX11Frame* pCDX11Frame);
	virtual ~Hypotrochoid(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);
};
