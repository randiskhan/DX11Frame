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

	static const int			_NumVertices = 16382;
	VertexPositionColor			_vertices[_NumVertices];
	DoublePoint					_verticesRaw[_NumVertices];

public:
	Hypotrochoid(CDX11Frame* pCDX11Frame);
	virtual ~Hypotrochoid(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);
};
