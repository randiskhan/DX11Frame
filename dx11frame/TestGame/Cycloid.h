// Cycloid.h
// Declaration file for Cycloid.

#pragma once

#include "CDX11Frame.h"
#include "IEntity.h"

struct DoublePoint
{
	double	x; // This vertex's  coordinate.
	double	y; // This vertex's  coordinate.
	double	a; // Angle used to calculate this vertex.
	double	d; // This vertex's distance from origin.
	double	p; // This vertex's angle in polar coordinates.
	DoublePoint()
	{
		x = y = a = d = p = 0.0;
	}
};

struct CycloidParameters
{
	// Parameters for Cycloid.
	// Fixed circle radius: _Radius1
	// Moving circle radius: _Radius2
	// Drawing point distance from center of moving circle: _ArmLength
	// I find it easiest to "normalize" these values. I keep radius1 at 1.0, so I
	// can think of radius2 as a ratio to/of radius1. This also helps with
	// _ArmLength - If _Radius1 is 1, then when _ArmLength equals _Radius2,
	// The Drawing point is exactly on the cirumfrence of the moving circle.
	const double		Radius1 = 1.0;
	double				Radius2;
	double				ArmLength;
	double				Cycles;

	int					NumberOfVerticiesPerCycle;
	int					NumberOfVerticies;

	bool				CopyFirstToEnd;

	CycloidParameters()
	{
		Radius2 = ArmLength = Cycles = 0.0;
		NumberOfVerticiesPerCycle = 2048;
		CopyFirstToEnd = false;
	}

	void	CalculateNeededCycles(int maxVerticies)
	{
		int cycles = 0;
		while (Radius2 != 0)
		{
			++cycles;
			if ((cycles / Radius2) == (int)(cycles / Radius2)) break;
			if (NumberOfVerticiesPerCycle * cycles > maxVerticies) break;
		}
		Cycles = cycles;
		// Also base the number of verticies on how many cycles are needed.
		NumberOfVerticies = NumberOfVerticiesPerCycle * cycles;
	}
};

class Cycloid : public IEntity
{
private:
	unique_ptr<PrimitiveBatch<VertexPositionColor>>	_pPrimtiveBatch;
	unique_ptr<BasicEffect>							_pBasicEffect;
	ID3D11InputLayout*								_pID3D11InputLayout;

	// Maxumum number of verticies. Used to declare arrays.
	// Max DTK can handle is 65534.
	static const int			_MaxVertices = 65534;
	// Array of verticies to send to GPU.
	VertexPositionColor			_vertices[_MaxVertices];
	// Array of raw verticies in coordinate [-1, 1] range.
	DoublePoint					_verticesRaw[_MaxVertices];

	CycloidParameters	_CycloidCurrent;
	CycloidParameters	_CycloidNext;

public:
	Cycloid(CDX11Frame* pCDX11Frame);
	virtual ~Cycloid(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);

private:
	void	CalculateRawVerticies(
		CycloidParameters&,
		DoublePoint[],
		int);
	// Color the raw verticies based on the angular position of the inner circle
	// to the outer circle (angle used in cycloid calculation).
	void	ColorVerticiesByAnglePosition(
		CycloidParameters&,
		DoublePoint[],
		VertexPositionColor[]);
	// Color the raw verticies based on the azmuth of the polar coordinate of the
	// raw vertex after the quarter turn counter-clockwise.
	void	ColorVerticiesByPolarCoordinates(
		CycloidParameters&,
		DoublePoint[],
		VertexPositionColor[]);
	// Pick a random color for all verticies.
	void	ColorVerticiesByRandom(
		CycloidParameters &cycloid,
		VertexPositionColor vert[]);
	void	RandomCycloid(CycloidParameters &);
	void	ConvertToScreen(
		CycloidParameters&,
		DoublePoint[],
		VertexPositionColor[],
		RECT);
};
