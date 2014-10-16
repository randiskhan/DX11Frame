// TestEntity2.cpp
// Implementation file for TestEntity2.

#include "TestEntity2.h"

TestEntity2::TestEntity2(CAppBase* pCAppBase) : IEntity(pCAppBase)
{
	ZeroMemory(_vertices,sizeof(VertexPositionColor)*_NumVertices);
	Init();
}

TestEntity2::~TestEntity2(void)
{
	Cleanup();
}

bool		TestEntity2::Init(void)
{
	bool good = true;

	HRESULT hr;

	_pPrimtiveBatch.reset(
		new PrimitiveBatch<VertexPositionColor>(
			GetFrame()->GetCDirectX()->GetContext()));
	_pBasicEffect.reset(
		new BasicEffect(GetFrame()->GetCDirectX()->GetDevice()));

	RECT r = GetFrame()->GetCWin32()->GetScreenRect();

	_pBasicEffect->SetProjection(
		XMMatrixOrthographicOffCenterRH(
		0, 
		(float)r.right, 
		(float)r.bottom,
		0, 
		0, 
		1.0f));
	_pBasicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	_pBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	hr = GetFrame()->GetCDirectX()->GetDevice()->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		&_pID3D11InputLayout);
	if(FAILED(hr)) good = false;

	for(int i = 0; i < _NumVertices; ++i)
		_vertices[i].color = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);

	return _IsInit = good;
}

bool		TestEntity2::Update(void)
{
	bool good = true;
	float t = GetFrame()->GetCTimer()->GetTotalElapsed();
	RECT r = GetFrame()->GetCWin32()->GetScreenRect();
	
	if(DoUpdate())
	{
		for(int i = 0; i < _NumVertices; i+=2)
		{
			_vertices[i].position = XMFLOAT3(
				(r.right*(i/(float)_NumVertices)),
				(float)r.bottom,
				0);
			_vertices[i+1].position = XMFLOAT3(
				(float)r.right,
				r.bottom*(1.0f-(i/(float)_NumVertices)),
				0);
		}
	}

	return good;
}

bool		TestEntity2::Render(void)
{
	bool good = true;

	if(DoRender())
	{
		_pBasicEffect->Apply(GetFrame()->GetCDirectX()->GetContext());
		GetFrame()->GetCDirectX()->GetContext()->IASetInputLayout(_pID3D11InputLayout);

		_pPrimtiveBatch->Begin();
		_pPrimtiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, _vertices, _NumVertices);
		_pPrimtiveBatch->End();
	}
	
	return good;
}

void		TestEntity2::Cleanup(void)
{
	SafeRelease(_pID3D11InputLayout);
}
