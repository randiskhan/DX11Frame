// Hypotrochoid.cpp
// Implementation file for Hypotrochoid.

#include "Hypotrochoid.h"

Hypotrochoid::Hypotrochoid(CDX11Frame* pCDX11Frame) : IEntity(pCDX11Frame)
{
	ZeroMemory(_vertices, sizeof(VertexPositionColor)*_NumVertices);
	ZeroMemory(_verticesRaw, sizeof(DoublePoint)*_NumVertices);
	Init();
}

Hypotrochoid::~Hypotrochoid(void)
{
	Cleanup();
}

bool		Hypotrochoid::Init(void)
{
	bool good = true;

	HRESULT hr;

	_pPrimtiveBatch.reset(
		new PrimitiveBatch<VertexPositionColor>(
		GetCDX11Frame()->GetCDirectX()->GetContext(), 65535U, 65535U));
	_pBasicEffect.reset(
		new BasicEffect(GetCDX11Frame()->GetCDirectX()->GetDevice()));

	RECT r = GetCDX11Frame()->GetCWin32()->GetScreenRect();

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

	hr = GetCDX11Frame()->GetCDirectX()->GetDevice()->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		&_pID3D11InputLayout);
	if (FAILED(hr)) good = false;

	for (int i = 0; i < _NumVertices; ++i)
		_vertices[i].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	return _IsInit = good;
}

bool		Hypotrochoid::Update(void)
{
	bool good = true;

	double t = GetCDX11Frame()->GetCTimer()->GetTotalElapsed();
	RECT r = GetCDX11Frame()->GetCWin32()->GetScreenRect();
	double maxDist = 0;
	int maxSquareEdgeScreenCoords = min(r.right, r.bottom);

	// Parameters for Hypotrochoid.
	// Fixed circle radius: radius1
	// Moving circle radius: radius2
	// Drawing point distance from center: d
	// Angle in radians: a
	// I find it easiest to "normalize" these values. I keep radius1 at 1.0, so I
	// can think of radius2 as a ratio to/of radius1. Next is d, which I prefer to
	// be a ratio of radius2, hence the d *= radius2. That way I can always think of
	// d = 1.0 to be drawing point on edge of radius2.
	double radius1 = 1.0, radius2 = 0.0, d = 0.5, a = 0.0;

	radius2 = NormSin(t * 0.002);
	d = radius2 * 0.5;

	// Calculate the raw coordinates, and find max absolute component.
	for (int i = 0; i < _NumVertices; ++i)
	{
		_verticesRaw[i].a = ((1.0 / (double)_NumVertices) * (i - (_NumVertices / 2.0)) * XM_2PI * 32.0);
		_verticesRaw[i].x = ((radius1 - radius2)*cos(_verticesRaw[i].a)) + (d*cos(((radius1 - radius2) / radius2)*_verticesRaw[i].a));
		_verticesRaw[i].y = ((radius1 - radius2)*sin(_verticesRaw[i].a)) - (d*sin(((radius1 - radius2) / radius2)*_verticesRaw[i].a));
		_verticesRaw[i].d = sqrt(pow(_verticesRaw[i].x, 2) + pow(_verticesRaw[i].y, 2));
		maxDist = max(maxDist, _verticesRaw[i].d);
	}
	// Use maxDist to make sure all raw points are within a normalized circle on xy plane.
	// Rotate the coordinates one quarter turn counter clockwise so zero angle is at top
	// of window, then convert to screen pixels.
	double swap = 0;
	for (int i = 0; i < _NumVertices; ++i)
	{
		// First normalize the coordinate components.
		_verticesRaw[i].x *= (1.0 / maxDist);
		_verticesRaw[i].y *= (1.0 / maxDist);
		// Then rotate one quarter turn counter-clockwise.
		swap = _verticesRaw[i].x;
		_verticesRaw[i].x = -(_verticesRaw[i].y);
		_verticesRaw[i].y = -swap;
		// Now translate the raw coordinates to screen coordinates.
		// The 0.99 is to move the drawing just a tad away from the window border.
		_vertices[i].position.x =
			(float)(((_verticesRaw[i].x * 0.95) * maxSquareEdgeScreenCoords * 0.5) + (r.right * 0.5));
		_vertices[i].position.y =
			(float)(((_verticesRaw[i].y * 0.95) * maxSquareEdgeScreenCoords * 0.5) + (r.bottom * 0.5));
		// Some color variation based on raw coordinates.
		// Must translate from range [-1.0,1.0] to [0.0,1.0].
		//_vertices[i].color.x = (float)(_verticesRaw[i].x * 0.5 + 0.5);
		//_vertices[i].color.y = (float)(_verticesRaw[i].y * 0.5 + 0.5);
		//_vertices[i].color.z = (float)(1.0-sqrt(pow(_verticesRaw[i].x,2) + pow(_verticesRaw[i].y,2)));
		_vertices[i].color.x = (float)NormSin(_verticesRaw[i].a);
		_vertices[i].color.y = (float)NormSin((_verticesRaw[i].a) + (XM_2PI / 3.0));
		_vertices[i].color.z = (float)NormSin((_verticesRaw[i].a) + (XM_2PI * 2.0 / 3.0));
	}

	return good;
}

bool		Hypotrochoid::Render(void)
{
	bool good = true;

	_pBasicEffect->Apply(GetCDX11Frame()->GetCDirectX()->GetContext());
	GetCDX11Frame()->GetCDirectX()->GetContext()->IASetInputLayout(_pID3D11InputLayout);

	_pPrimtiveBatch->Begin();
	_pPrimtiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, _vertices, _NumVertices);
	_pPrimtiveBatch->End();

	return good;
}

void		Hypotrochoid::Cleanup(void)
{
	SafeRelease(_pID3D11InputLayout);
}