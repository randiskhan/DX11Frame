// Cycloid.h
// Declaration file for Cycloid.

#pragma once

// Maxumum number of verticies.
// Max DTK can handle is 65534.
#define MAX_VERTICIES 32768
#define DEFAULT_VERTICIES_PER_CYCLE 512
#define MAX_CYCLES 24

#include "dx11_frame.h"
#include "i_entity.h"

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

	float				r, g, b;

	CycloidParameters::CycloidParameters()
	{
		Radius2 = 0.25;
		ArmLength = 1.0;
		Cycles = 1.0;
		NumberOfVerticiesPerCycle = DEFAULT_VERTICIES_PER_CYCLE;
		NumberOfVerticies = MAX_VERTICIES;
		CopyFirstToEnd = false;
		r = g = b = 1.0f;
	}

	bool	CycloidParameters::CalculateNeededCycles(int maxCycles)
	{
		auto found = false;
		auto cycles = 1;
		while (Radius2 != 0)
		{
			if ((cycles / Radius2) == (int)(cycles / Radius2))
			{
				found = true;
				break;
			}
			if (cycles > maxCycles) break;
			if (NumberOfVerticiesPerCycle * cycles > MAX_VERTICIES - 1) break;
			++cycles;
		}
		Cycles = cycles + (1.0 / (double)NumberOfVerticiesPerCycle);
		// Also base the number of verticies on how many cycles are needed.
		NumberOfVerticies = NumberOfVerticiesPerCycle * cycles;

		return found;
	}

	void	CycloidParameters::CopyTo(CycloidParameters& cycloid)
	{
		cycloid.Radius2 = Radius2;
		cycloid.ArmLength = ArmLength;
		cycloid.Cycles = Cycles;
		cycloid.NumberOfVerticies = NumberOfVerticies;
		cycloid.NumberOfVerticiesPerCycle = NumberOfVerticiesPerCycle;
		cycloid.CopyFirstToEnd = CopyFirstToEnd;
		cycloid.r = r;
		cycloid.g = g;
		cycloid.b = b;
	}
};

class Cycloid : public i_entity
{
private:
	unique_ptr<PrimitiveBatch<VertexPositionColor>>	_pPrimtiveBatch;
	unique_ptr<BasicEffect>							_pBasicEffect;
	ID3D11InputLayout*								_pID3D11InputLayout{};

	// Array of verticies to send to GPU.
	VertexPositionColor			_vertices[MAX_VERTICIES]{};
	// Array of raw verticies in coordinate [-1, 1] range.
	DoublePoint					_verticesRaw[MAX_VERTICIES];

	CycloidParameters	_CycloidCurrent;
	CycloidParameters	_CycloidNext;
	CycloidParameters	_CycloidPrevious;

	double	_TimeDeltaMorph{};
	double	_TimeDeltaNewCycloid{};
	double	_TimeStampMorph{};
	double	_TimeStampNewCycloid{};

public:
	Cycloid(dx11_frame* pCDX11Frame);
	virtual ~Cycloid(void);

	bool	init(void);
	bool	Update(void);
	bool	render(void);
	void	cleanup(void);

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
