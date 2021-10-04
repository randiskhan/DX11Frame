// Cycloid.cpp
// Implementation file for Cycloid.

#include "hypocycloid.h"

using namespace dx11_frame_helpers;

hypocycloid::hypocycloid(dx11_frame* dx11_frame) : i_entity(dx11_frame)
{
	hypocycloid::init();
}

hypocycloid::~hypocycloid()
{
	hypocycloid::cleanup();
}

bool		hypocycloid::init()
{

	auto good = true;

	auto valid = false;
	while (!valid)
	{
		random_hypocycloid(hypocycloid_previous_);
		valid = hypocycloid_previous_.calculate_needed_cycles(max_cycles);
	}
	valid = false;
	while (!valid)
	{
		random_hypocycloid(hypocycloid_next_);
		valid = hypocycloid_next_.calculate_needed_cycles(max_cycles);
	}

	time_delta_morph_ = 1.0;
	time_delta_new_hypocycloid_ = 15.0;
	time_stamp_morph_ = 0;
	time_stamp_new_hypocycloid_ = 0;

	primitive_batch_.reset(
		new PrimitiveBatch<VertexPositionColor>(
		get_cdx11_frame()->get_directx()->get_context(), 
			65535U, 
			65535U));
	basic_effect_.reset(
		new BasicEffect(get_cdx11_frame()->get_directx()->get_device()));

	const auto r = get_cdx11_frame()->get_win32()->get_screen_rect();

	basic_effect_->SetProjection(
		XMMatrixOrthographicOffCenterRH(
		0,
		static_cast<float>(r.right),
		static_cast<float>(r.bottom),
		0,
		0,
		1.0f));
	basic_effect_->SetVertexColorEnabled(true);

	void const* shader_byte_code;
	size_t byte_code_length;

	basic_effect_->GetVertexShaderBytecode(
		&shader_byte_code, 
		&byte_code_length);

	const HRESULT hr = 
		get_cdx11_frame()->get_directx()->get_device()->CreateInputLayout(
			VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shader_byte_code,
			byte_code_length,
			&i_d3d11_input_layout_);
	if (FAILED(hr)) good = false;

	for (auto& vertices : vertices_)
		vertices.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	return is_init_ = good;

}

bool		hypocycloid::update()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	const auto t = get_cdx11_frame()->get_timer()->get_total_elapsed();

	if (time_stamp_new_hypocycloid_ + time_delta_new_hypocycloid_ < t)
	{
		time_stamp_new_hypocycloid_ = t;
		hypocycloid_current_.copy_to(hypocycloid_previous_);
		auto valid = false;
		while (!valid)
		{
			random_hypocycloid(hypocycloid_next_);
			valid = hypocycloid_next_.calculate_needed_cycles(max_cycles);
		}
	}
	const auto lerp_amt = 
		(t - time_stamp_new_hypocycloid_) / (time_delta_new_hypocycloid_ - 2.0);
	hypocycloid_current_.arm_length = interpolate_cos(
		hypocycloid_previous_.arm_length, 
		hypocycloid_next_.arm_length, 
		lerp_amt);
	hypocycloid_current_.radius_2 = interpolate_cos(
		hypocycloid_previous_.radius_2, 
		hypocycloid_next_.radius_2, 
		lerp_amt);
	hypocycloid_current_.cycles = interpolate_cos(
		hypocycloid_previous_.cycles, 
		hypocycloid_next_.cycles, 
		lerp_amt);
	hypocycloid_current_.r = interpolate_cos(
		hypocycloid_previous_.r, 
		hypocycloid_next_.r, 
		static_cast<float>(lerp_amt));
	hypocycloid_current_.g = interpolate_cos(
		hypocycloid_previous_.g, 
		hypocycloid_next_.g, 
		static_cast<float>(lerp_amt));
	hypocycloid_current_.b = interpolate_cos(
		hypocycloid_previous_.b, 
		hypocycloid_next_.b, 
		static_cast<float>(lerp_amt));

	calculate_raw_vertices(
		hypocycloid_current_,
		vertices_raw_,
		max_vertices);
	convert_to_screen(
		hypocycloid_current_,
		vertices_raw_,
		vertices_,
		get_cdx11_frame()->get_win32()->get_screen_rect());
	for (auto i = 0; i < hypocycloid_current_.number_of_vertices; ++i)
	{
		vertices_[i].color.x = hypocycloid_current_.r;
		vertices_[i].color.y = hypocycloid_current_.g;
		vertices_[i].color.z = hypocycloid_current_.b;
	}

	return good;

}

bool		hypocycloid::render()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	basic_effect_->Apply(get_cdx11_frame()->get_directx()->get_context());
	get_cdx11_frame()->get_directx()->get_context()->IASetInputLayout(
		i_d3d11_input_layout_);

	primitive_batch_->Begin();
	primitive_batch_->Draw(
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
		vertices_,
		hypocycloid_current_.number_of_vertices + 
			(hypocycloid_current_.copy_first_to_end ? 1 : 0)
		);
	primitive_batch_->End();

	return good;

}

void		hypocycloid::cleanup()
{
	safe_release(i_d3d11_input_layout_);
}

#pragma region Private methods

void		hypocycloid::color_vertices_by_angle_position(
	const hypocycloid_parameters& hypocycloid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < hypocycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].a));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI * 2.0 / 3.0));
	}

}

void		hypocycloid::color_vertices_by_polar_coordinates(
	const hypocycloid_parameters& hypocycloid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < hypocycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].p));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI * 2.0 / 3.0));
	}

}

void		hypocycloid::color_vertices_by_random(
	const hypocycloid_parameters& hypocycloid,
	VertexPositionColor vert[]) const
{

	const float temp_x = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_y = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_z = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	for (auto i = 0; i < hypocycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = temp_x;
		vert[i].color.y = temp_y;
		vert[i].color.z = temp_z;
	}

}

void		hypocycloid::calculate_raw_vertices(
	hypocycloid_parameters &hypocycloid,
	double_point raw[],
	const int max_vert) const
{

	double max_dist = 0;
	// Recalculate the drawing point position.
	const double recalculate_arm_length = 
		hypocycloid_current_.arm_length * hypocycloid_current_.radius_2;

	// Verify the number of vertices does not exceed our array and max count
	// for DTK primitive batch.
	if (hypocycloid.number_of_vertices > 
		max_vert - (hypocycloid.copy_first_to_end ? 1 : 0))
		hypocycloid.number_of_vertices = 
		max_vert - (hypocycloid.copy_first_to_end ? 1 : 0);

	// Calculate the raw coordinates, and find max absolute component.
	auto i = 0;
	for (; i < hypocycloid.number_of_vertices; ++i)
	{
		raw[i].a =
			1.0 / static_cast<double>(hypocycloid.number_of_vertices) * 
			(i - hypocycloid.number_of_vertices / 2.0) *
			XM_2PI * hypocycloid.cycles;
		raw[i].x =
			(hypocycloid.radius_1 - hypocycloid.radius_2) * cos(raw[i].a) +
			recalculate_arm_length * 
			cos((hypocycloid.radius_1 - hypocycloid.radius_2) / 
				hypocycloid.radius_2 * 
				raw[i].a);
		raw[i].y =
			(hypocycloid.radius_1 - hypocycloid.radius_2) * sin(raw[i].a) -
			recalculate_arm_length * 
			sin((hypocycloid.radius_1 - hypocycloid.radius_2) / 
				hypocycloid.radius_2 * 
				raw[i].a);
		raw[i].d = sqrt(pow(raw[i].x, 2) + pow(raw[i].y, 2));
		raw[i].p = atan2(raw[i].y, raw[i].x);
		if (abs(raw[i].p) != raw[i].p)
			raw[i].p = static_cast<double>(XM_2PI) + raw[i].p;
		max_dist = max(max_dist, raw[i].d);
	}
	if (hypocycloid.copy_first_to_end)
		// Copy the starting raw vertex to the end of the used array.
		raw[i] = raw[0];

	for (auto j = 0; 
	     j < hypocycloid.number_of_vertices + (hypocycloid.copy_first_to_end ? 1 : 0); 
	     ++j)
	{
		// First normalize the coordinate components.
		raw[j].x *= 1.0 / max_dist;
		raw[j].y *= 1.0 / max_dist;

		// Then rotate one quarter turn counter-clockwise.
		const double swap = raw[j].x;
		raw[j].x = -raw[j].y;
		raw[j].y = -swap;
	}

}

void		hypocycloid::random_hypocycloid(
	hypocycloid_parameters &hypocycloid) const
{

	// Generate random radius_2 [0.2, 0.8] in 0.001 increments.
	hypocycloid.radius_2 = 
		get_cdx11_frame()->get_rng()->get_rand_int(200, 801) / 1000.0;
	// Generate random arm length [0.2,2.0] in 0.01 increments.
	hypocycloid.arm_length = 
		get_cdx11_frame()->get_rng()->get_rand_int(20, 201) / 100.0;
	// Generate a random color.
	hypocycloid.r = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	hypocycloid.g = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	hypocycloid.b = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);

}

void		hypocycloid::convert_to_screen(
	const hypocycloid_parameters& cycloid,
	double_point raw[],
	VertexPositionColor vert[],
	const RECT canvas)
{

	const int max = min(canvas.right, canvas.bottom);
	for (auto i = 0; 
		i < cycloid.number_of_vertices + (cycloid.copy_first_to_end ? 1 : 0); 
		++i)
	{
		vert[i].position.x =
			static_cast<float>(
				raw[i].x * 0.95 * max * 0.5 + canvas.right * 0.5);
		vert[i].position.y =
			static_cast<float>(
				raw[i].y * 0.95 * max * 0.5 + canvas.bottom * 0.5);
	}

}

#pragma endregion