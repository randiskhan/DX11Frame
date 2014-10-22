// TestEntity2.cpp
// Implementation file for TestEntity2.

#include "TestEntity2.h"

TestEntity2::TestEntity2(CAppBase* pCAppBase) : IEntity(pCAppBase)
{
	ZeroMemory(_vertices,sizeof(VertexPositionColor)*_NumVertices);
	ZeroMemory(_verticesRaw,sizeof(DoublePoint)*_NumVertices);
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
			GetFrame()->GetCDirectX()->GetContext(), 65535U, 65535U));
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
		_vertices[i].color = XMFLOAT4(0.5f,0.5f,0.5f,1.0f);

	return _IsInit = good;
}

bool		TestEntity2::Update(void)
{
	bool good = true;

	float t = GetFrame()->GetCTimer()->GetTotalElapsed();
	RECT r = GetFrame()->GetCWin32()->GetScreenRect();
	double maxCoord=0;
	
	// Parameters for Hypotrochoid.
	// Fixed circle radius: radius1
	// Moving circle radius: radius2
	// Drawing point distance from center: d
	// Angle in radians: a
	// I find it easiest to "normalize" these values. I keep radius1 at 1.0, so I
	// can think of radius2 as a ratio to/of radius1. Next is d, which I prefer to 
	// be a ratio of radius2, hence the d *= radius2. That way I can always think of
	// d = 1.0 to be drawing point on edge of radius2.
	double radius1 = 1.0, radius2 = 0.515, d = 0.5, a = 0.0;

	if(DoUpdate())
	{
		d *= radius2;

		// Calculate the raw coordinates, and find max absolute component.
		for(int i = 0; i < _NumVertices; ++i)
		{
			a = ((1.0/(double)_NumVertices) * (i-(_NumVertices/2.0)) * XM_2PI * 256.0);
			_verticesRaw[i].x = ((radius1-radius2)*cos(a))+(d*cos(((radius1-radius2)/radius2)*a));
			_verticesRaw[i].y = ((radius1-radius2)*sin(a))-(d*sin(((radius1-radius2)/radius2)*a));
			maxCoord = max(maxCoord,abs(_verticesRaw[i].x)); 
			maxCoord = max(maxCoord,abs(_verticesRaw[i].y));
		}
		// Use max absolute component to normalize raw coordinates to ([-1,1],[-1,1]),
		// Rotate the coordinates one quarter turn counter clockwise so zero angle is at top
		// of window, then convert to screen pixels.
		double swap = 0;
		for(int i = 0; i < _NumVertices; ++i)
		{
			// First normalize the coordinate components.
			_verticesRaw[i].x *= (1.0/maxCoord);
			_verticesRaw[i].y *= (1.0/maxCoord);
			// Then rotate one quarter turn counter-clockwise.
			swap = _verticesRaw[i].x;
			_verticesRaw[i].x = -(_verticesRaw[i].y);
			_verticesRaw[i].y = -(swap);
			// Now translate the raw coordinates to screen coordinates.
			// The 0.99 is to move the drawing just a tad away from the window border.
			_vertices[i].position.x = (float)(( (_verticesRaw[i].x * 0.99) * r.bottom * 0.5 ) + ( r.right * 0.5 ));
			_vertices[i].position.y = (float)(( (_verticesRaw[i].y * 0.99) * r.bottom * 0.5 ) + ( r.bottom * 0.5 ));
			// Some color variation based on raw coordinates.
			// Must translate from range [-1.0,1.0] to [0.0,1.0].
			_vertices[i].color.x = (float)(_verticesRaw[i].x * 0.5 + 0.5);
			_vertices[i].color.y = 0.5f;
			_vertices[i].color.z = (float)(_verticesRaw[i].y * 0.5 + 0.5);
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
		_pPrimtiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, _vertices, _NumVertices);
		_pPrimtiveBatch->End();
	}
	
	return good;
}

void		TestEntity2::Cleanup(void)
{
	SafeRelease(_pID3D11InputLayout);
}
