// TestEntity.h
// Declaration file for TestEntity.

#pragma once

#include "CAppBase.h"
#include "IEntity.h"

class TestEntity2 :	public IEntity
{
private:
	unique_ptr<PrimitiveBatch<VertexPositionColor>>	_pPrimtiveBatch;
	unique_ptr<BasicEffect>							_pBasicEffect;
	ID3D11InputLayout*								_pID3D11InputLayout;

	static const int			_NumVertices = 65534;
	VertexPositionColor			_vertices[_NumVertices];

public:
	TestEntity2(CAppBase* pCAppBase);
	virtual ~TestEntity2(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);

};

