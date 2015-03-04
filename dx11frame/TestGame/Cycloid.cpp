// Cycloid.cpp
// Implementation file for Cycloid.

#include "Cycloid.h"

Cycloid::Cycloid(CDX11Frame* pCDX11Frame) : IEntity(pCDX11Frame)
{
	Init();
}

Cycloid::~Cycloid(void)
{
	Cleanup();
}

bool		Cycloid::Init(void)
{
	bool good = true;

	bool valid = false;
	while (!valid)
	{
		RandomCycloid(_CycloidPrevious);
		valid = _CycloidPrevious.CalculateNeededCycles(MAX_CYCLES);
	}
	valid = false;
	while (!valid)
	{
		RandomCycloid(_CycloidNext);
		valid = _CycloidNext.CalculateNeededCycles(MAX_CYCLES);
	}

	_TimeDeltaMorph = 1.0;
	_TimeDeltaNewCycloid = 30.0;
	_TimeStampMorph = 0;
	_TimeStampNewCycloid = 0;

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

	for (int i = 0; i < MAX_VERTICIES; ++i)
		_vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	return _IsInit = good;
}

bool		Cycloid::Update(void)
{
	bool good = true;

	double t = GetCDX11Frame()->GetCTimer()->GetTotalElapsed();

	if (_TimeStampNewCycloid + _TimeDeltaNewCycloid < t)
	{
		_TimeStampNewCycloid = t;
		_CycloidCurrent.CopyTo(_CycloidPrevious);
		bool valid = false;
		while (!valid)
		{
			RandomCycloid(_CycloidNext);
			valid = _CycloidNext.CalculateNeededCycles(MAX_CYCLES);
		}
	}
	double lerpAmt = (t - _TimeStampNewCycloid) / (_TimeDeltaNewCycloid - 2.0);
	_CycloidCurrent.ArmLength = lerp(_CycloidPrevious.ArmLength, _CycloidNext.ArmLength, lerpAmt);
	_CycloidCurrent.Radius2 = lerp(_CycloidPrevious.Radius2, _CycloidNext.Radius2, lerpAmt);
	_CycloidCurrent.Cycles = lerp(_CycloidPrevious.Cycles, _CycloidNext.Cycles, lerpAmt);
	_CycloidCurrent.r = lerp(_CycloidPrevious.r, _CycloidNext.r, (float)lerpAmt);
	_CycloidCurrent.g = lerp(_CycloidPrevious.g, _CycloidNext.g, (float)lerpAmt);
	_CycloidCurrent.b = lerp(_CycloidPrevious.b, _CycloidNext.b, (float)lerpAmt);

	CalculateRawVerticies(
		_CycloidCurrent,
		_verticesRaw,
		MAX_VERTICIES);
	ConvertToScreen(
		_CycloidCurrent,
		_verticesRaw,
		_vertices,
		GetCDX11Frame()->GetCWin32()->GetScreenRect());
	for (int i = 0; i < _CycloidCurrent.NumberOfVerticies; ++i)
	{
		_vertices[i].color.x = _CycloidCurrent.r;
		_vertices[i].color.y = _CycloidCurrent.g;
		_vertices[i].color.z = _CycloidCurrent.b;
	}

	return good;
}

bool		Cycloid::Render(void)
{
	bool good = true;

	_pBasicEffect->Apply(GetCDX11Frame()->GetCDirectX()->GetContext());
	GetCDX11Frame()->GetCDirectX()->GetContext()->IASetInputLayout(_pID3D11InputLayout);

	_pPrimtiveBatch->Begin();
	_pPrimtiveBatch->Draw(
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
		_vertices,
		_CycloidCurrent.NumberOfVerticies + (_CycloidCurrent.CopyFirstToEnd ? 1 : 0)
		);
	_pPrimtiveBatch->End();

	return good;
}

void		Cycloid::Cleanup(void)
{
	SafeRelease(_pID3D11InputLayout);
}

#pragma region Private methods

void		Cycloid::ColorVerticiesByAnglePosition(
	CycloidParameters &cycloid,
	DoublePoint raw[],
	VertexPositionColor vert[])
{
	for (int i = 0; i < cycloid.NumberOfVerticies; ++i)
	{
		vert[i].color.x = (float)NormSin(raw[i].a);
		vert[i].color.y = (float)NormSin((raw[i].a) + (XM_2PI / 3.0));
		vert[i].color.z = (float)NormSin((raw[i].a) + (XM_2PI * 2.0 / 3.0));
	}
}

void		Cycloid::ColorVerticiesByPolarCoordinates(
	CycloidParameters &cycloid,
	DoublePoint raw[],
	VertexPositionColor vert[])
{
	for (int i = 0; i < cycloid.NumberOfVerticies; ++i)
	{
		vert[i].color.x = (float)NormSin(raw[i].p);
		vert[i].color.y = (float)NormSin((raw[i].p) + (XM_2PI / 3.0));
		vert[i].color.z = (float)NormSin((raw[i].p) + (XM_2PI * 2.0 / 3.0));
	}
}

void		Cycloid::ColorVerticiesByRandom(
	CycloidParameters &cycloid,
	VertexPositionColor vert[])
{
	float tempx, tempy, tempz;
	tempx = rand() / (float)RAND_MAX;
	tempy = rand() / (float)RAND_MAX;
	tempz = rand() / (float)RAND_MAX;
	for (int i = 0; i < cycloid.NumberOfVerticies; ++i)
	{
		vert[i].color.x = tempx;
		vert[i].color.y = tempy;
		vert[i].color.z = tempz;
	}
}

void		Cycloid::CalculateRawVerticies(
	CycloidParameters &cycloid,
	DoublePoint raw[],
	int maxVert)
{
	double maxDist = 0, recalcArmLength = 0;
	// Recalculate the drawing point position.
	recalcArmLength = _CycloidCurrent.ArmLength * _CycloidCurrent.Radius2;

	// Verify the number of verticies does not exceed our array and max count for
	// DTK primitive batch.
	if (cycloid.NumberOfVerticies > maxVert - (cycloid.CopyFirstToEnd ? 1 : 0))
		cycloid.NumberOfVerticies = maxVert - (cycloid.CopyFirstToEnd ? 1 : 0);

	// Calculate the raw coordinates, and find max absolute component.
	int i = 0;
	for (; i < cycloid.NumberOfVerticies; ++i)
	{
		raw[i].a =
			((1.0 / (double)cycloid.NumberOfVerticies) * (i - (cycloid.NumberOfVerticies / 2.0)) *
			XM_2PI * cycloid.Cycles);
		raw[i].x =
			((cycloid.Radius1 - cycloid.Radius2) * cos(raw[i].a)) +
			(recalcArmLength * cos(((cycloid.Radius1 - cycloid.Radius2) / cycloid.Radius2) * raw[i].a));
		raw[i].y =
			((cycloid.Radius1 - cycloid.Radius2) * sin(raw[i].a)) -
			(recalcArmLength * sin(((cycloid.Radius1 - cycloid.Radius2) / cycloid.Radius2) * raw[i].a));
		raw[i].d = sqrt(pow(raw[i].x, 2) + pow(raw[i].y, 2));
		raw[i].p = atan2(raw[i].y, raw[i].x);
		if (abs(raw[i].p) != raw[i].p)
			raw[i].p = (double)XM_2PI + raw[i].p;
		maxDist = max(maxDist, raw[i].d);
	}
	if (cycloid.CopyFirstToEnd)
		// Copy the starting raw vertex to the end of the used array.
		raw[i] = raw[0];

	// Use maxDist to make sure all raw points are within a normalized circle
	// on xy plane. Rotate the coordinates one quarter turn counter clockwise
	// so zero angle is at top of window.
	double swap = 0;
	for (int i = 0; i < cycloid.NumberOfVerticies + (cycloid.CopyFirstToEnd ? 1 : 0); ++i)
	{
		// First normalize the coordinate components.
		raw[i].x *= (1.0 / maxDist);
		raw[i].y *= (1.0 / maxDist);

		// Then rotate one quarter turn counter-clockwise.
		swap = raw[i].x;
		raw[i].x = -(raw[i].y);
		raw[i].y = -swap;
	}
}

void		Cycloid::RandomCycloid(CycloidParameters &cycloid)
{
	// Generate random radius2 [0.2, 0.8] in 0.001 increments.
	cycloid.Radius2 = (rand() % 601 + 200) / 1000.0;
	// Generate random arm length [0.2,2.0] in 0.01 increments.
	cycloid.ArmLength = (rand() % 181 + 20) / 100.0;
	// Generate a random color.
	cycloid.r = rand() / (float)RAND_MAX;
	cycloid.g = rand() / (float)RAND_MAX;
	cycloid.b = rand() / (float)RAND_MAX;
}

void		Cycloid::ConvertToScreen(
	CycloidParameters &cycloid,
	DoublePoint raw[],
	VertexPositionColor vert[],
	RECT canvas)
{
	int max = min(canvas.right, canvas.bottom);
	for (int i = 0; i < cycloid.NumberOfVerticies + (cycloid.CopyFirstToEnd ? 1 : 0); ++i)
	{
		vert[i].position.x =
			(float)(((raw[i].x * 0.95) * max * 0.5) + (canvas.right * 0.5));
		vert[i].position.y =
			(float)(((raw[i].y * 0.95) * max * 0.5) + (canvas.bottom * 0.5));
	}
}

#pragma endregion