// Hypotrochoid.cpp
// Implementation file for Hypotrochoid.

#include "Hypotrochoid.h"

Hypotrochoid::Hypotrochoid(CDX11Frame* pCDX11Frame) : IEntity(pCDX11Frame)
{
	Init();
}

Hypotrochoid::~Hypotrochoid(void)
{
	Cleanup();
}

bool		Hypotrochoid::Init(void)
{
	bool good = true;

	// Initial values. See header for descriptions.
	_Radius1 = 1.0;
	_Radius2 = 0.5;
	_ArmLength = 0.4;
	_NumVerticesPerCycle = 2048;
	_AnimationDelay = 2.5;
	_NumVertices = 16384;
	_Cycles = 32;
	_CalcCycles = false;
	_CopyOriginToEnd = true;
	_Randomize = true;

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

	for (int i = 0; i < _MaxVertices; ++i)
		_vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	return _IsInit = good;
}

bool		Hypotrochoid::Update(void)
{
	bool good = true;

	double t = GetCDX11Frame()->GetCTimer()->GetTotalElapsed();
	RECT r = GetCDX11Frame()->GetCWin32()->GetScreenRect();
	double maxDist = 0;
	int maxSquareEdgeScreenCoords = min(r.right, r.bottom);

	// Animate the radius of rolling circle.
	static double timestamp = -_AnimationDelay, recalcArmLength = 0;
	if (t > timestamp + _AnimationDelay)
	{
		timestamp += _AnimationDelay;
		// Make changes to cycloid parameters.
		if (_Randomize)
		{
			bool valid = false;
			//if number of cycles is valid exit loop
			while (!valid)
			{
				// Generate random radius2 (0, 1) in 0.001 increments.
				_Radius2 = (rand() % 800 + 100) / 1000.0;
				// Generate random arm length [0,2] in 0.01 increments.
				_ArmLength = (rand() % 201) / 100.0;
				// Calculate needed cycles to complete
				int cycles = 0;
				while (_NumVerticesPerCycle * cycles < _MaxVertices)
				{
					++cycles;
					if ((cycles / _Radius2) == (int)(cycles / _Radius2))
					{
						valid = true;
						break;
					}
				}
				_Cycles = cycles;
				// Also base the number of verticies on how many cycles are needed.
				_NumVertices = _NumVerticesPerCycle * cycles;
			}
		}
		else
		{
			_Radius2 = NormSin(t * 0.002) * 0.8 + 0.1;

			// Calculate the number of cycles needed to return to the starting vertex.
			// This method only works if _Radius1 says at 1.
			if (_CalcCycles)
			{
				int cycles = 0;
				while (_Radius2 != 0)
				{
					++cycles;
					if ((cycles / _Radius2) == (int)(cycles / _Radius2)) break;
					if (_NumVerticesPerCycle * cycles > _MaxVertices) break;
				}
				_Cycles = cycles;
				// Also base the number of verticies on how many cycles are needed.
				_NumVertices = _NumVerticesPerCycle * cycles;
			}
		}

		// Recalculate the drawing point position.
		recalcArmLength = _ArmLength * _Radius2;

		// Verify the number of verticies does not exceed our array and max count for
		// DTK primitive batch.
		if (_NumVertices > _MaxVertices - (_CopyOriginToEnd ? 1 : 0))
			_NumVertices = _MaxVertices - (_CopyOriginToEnd ? 1 : 0);

		// Calculate the raw coordinates, and find max absolute component.
		int i = 0;
		for (; i < _NumVertices; ++i)
		{
			_verticesRaw[i].a = ((1.0 / (double)_NumVertices) * (i - (_NumVertices / 2.0)) * XM_2PI *_Cycles);
			_verticesRaw[i].x =
				((_Radius1 - _Radius2) * cos(_verticesRaw[i].a)) + (recalcArmLength * cos(((_Radius1 - _Radius2) / _Radius2) * _verticesRaw[i].a));
			_verticesRaw[i].y =
				((_Radius1 - _Radius2) * sin(_verticesRaw[i].a)) - (recalcArmLength * sin(((_Radius1 - _Radius2) / _Radius2) * _verticesRaw[i].a));
			_verticesRaw[i].d = sqrt(pow(_verticesRaw[i].x, 2) + pow(_verticesRaw[i].y, 2));
			maxDist = max(maxDist, _verticesRaw[i].d);
		}
		if (_CopyOriginToEnd)
			// Copy the starting raw vertex to the end of the used array.
			_verticesRaw[i] = _verticesRaw[0];
		// Use maxDist to make sure all raw points are within a normalized circle on xy plane.
		// Rotate the coordinates one quarter turn counter clockwise so zero angle is at top
		// of window, then convert to screen pixels.
		double swap = 0;
		for (int i = 0; i < _NumVertices + (_CopyOriginToEnd ? 1 : 0); ++i)
		{
			// First normalize the coordinate components.
			_verticesRaw[i].x *= (1.0 / maxDist);
			_verticesRaw[i].y *= (1.0 / maxDist);
			// Then rotate one quarter turn counter-clockwise.
			swap = _verticesRaw[i].x;
			_verticesRaw[i].x = -(_verticesRaw[i].y);
			_verticesRaw[i].y = -swap;
			// Now translate the raw coordinates to screen coordinates.
			// The * 0.XX is to move the drawing just a tad away from the window border.
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
	}

	return good;
}

bool		Hypotrochoid::Render(void)
{
	bool good = true;

	_pBasicEffect->Apply(GetCDX11Frame()->GetCDirectX()->GetContext());
	GetCDX11Frame()->GetCDirectX()->GetContext()->IASetInputLayout(_pID3D11InputLayout);

	_pPrimtiveBatch->Begin();
	_pPrimtiveBatch->Draw(
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
		_vertices,
		_NumVertices + (_CopyOriginToEnd ? 1 : 0)
		);
	_pPrimtiveBatch->End();

	return good;
}

void		Hypotrochoid::Cleanup(void)
{
	SafeRelease(_pID3D11InputLayout);
}