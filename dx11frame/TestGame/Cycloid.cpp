// Cycloid.cpp
// Implementation file for Cycloid.

#include "Cycloid.h"

using namespace dx11_frame_helpers;

cycloid::cycloid(dx11_frame* dx11_frame) : i_entity(dx11_frame)
{
	cycloid::init();
}

cycloid::~cycloid()
{
	cycloid::cleanup();
}

bool		cycloid::init()
{

	auto good = true;

	auto valid = false;
	while (!valid)
	{
		random_cycloid(cycloid_previous_);
		valid = cycloid_previous_.calculate_needed_cycles(max_cycles);
	}
	valid = false;
	while (!valid)
	{
		random_cycloid(cycloid_next_);
		valid = cycloid_next_.calculate_needed_cycles(max_cycles);
	}

	time_delta_morph_ = 1.0;
	time_delta_new_cycloid_ = 15.0;
	time_stamp_morph_ = 0;
	time_stamp_new_cycloid_ = 0;

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

bool		cycloid::update()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	const auto t = get_cdx11_frame()->get_timer()->get_total_elapsed();

	if (time_stamp_new_cycloid_ + time_delta_new_cycloid_ < t)
	{
		time_stamp_new_cycloid_ = t;
		cycloid_current_.copy_to(cycloid_previous_);
		auto valid = false;
		while (!valid)
		{
			random_cycloid(cycloid_next_);
			valid = cycloid_next_.calculate_needed_cycles(max_cycles);
		}
	}
	const auto lerp_amt = 
		(t - time_stamp_new_cycloid_) / (time_delta_new_cycloid_ - 2.0);
	cycloid_current_.arm_length = interpolate_cos(
		cycloid_previous_.arm_length, 
		cycloid_next_.arm_length, 
		lerp_amt);
	cycloid_current_.radius_2 = interpolate_cos(
		cycloid_previous_.radius_2, 
		cycloid_next_.radius_2, 
		lerp_amt);
	cycloid_current_.cycles = interpolate_cos(
		cycloid_previous_.cycles, 
		cycloid_next_.cycles, 
		lerp_amt);
	cycloid_current_.r = interpolate_cos(
		cycloid_previous_.r, 
		cycloid_next_.r, 
		static_cast<float>(lerp_amt));
	cycloid_current_.g = interpolate_cos(
		cycloid_previous_.g, 
		cycloid_next_.g, 
		static_cast<float>(lerp_amt));
	cycloid_current_.b = interpolate_cos(
		cycloid_previous_.b, 
		cycloid_next_.b, 
		static_cast<float>(lerp_amt));

	calculate_raw_vertices(
		cycloid_current_,
		vertices_raw_,
		max_vertices);
	convert_to_screen(
		cycloid_current_,
		vertices_raw_,
		vertices_,
		get_cdx11_frame()->get_win32()->get_screen_rect());
	for (auto i = 0; i < cycloid_current_.number_of_vertices; ++i)
	{
		vertices_[i].color.x = cycloid_current_.r;
		vertices_[i].color.y = cycloid_current_.g;
		vertices_[i].color.z = cycloid_current_.b;
	}

	return good;

}

bool		cycloid::render()
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
		cycloid_current_.number_of_vertices + 
			(cycloid_current_.copy_first_to_end ? 1 : 0)
		);
	primitive_batch_->End();

	return good;

}

void		cycloid::cleanup()
{
	safe_release(i_d3d11_input_layout_);
}

#pragma region Private methods

void		cycloid::color_vertices_by_angle_position(
	const cycloid_parameters& cycloid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < cycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].a));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI * 2.0 / 3.0));
	}

}

void		cycloid::color_vertices_by_polar_coordinates(
	const cycloid_parameters& cycloid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < cycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].p));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI * 2.0 / 3.0));
	}

}

void		cycloid::color_vertices_by_random(
	const cycloid_parameters& cycloid,
	VertexPositionColor vert[]) const
{

	const float temp_x = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_y = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_z = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	for (auto i = 0; i < cycloid.number_of_vertices; ++i)
	{
		vert[i].color.x = temp_x;
		vert[i].color.y = temp_y;
		vert[i].color.z = temp_z;
	}

}

void		cycloid::calculate_raw_vertices(
	cycloid_parameters &cycloid,
	double_point raw[],
	const int max_vert) const
{

	double max_dist = 0;
	// Recalculate the drawing point position.
	const double recalculate_arm_length = 
		cycloid_current_.arm_length * cycloid_current_.radius_2;

	// Verify the number of vertices does not exceed our array and max count
	// for DTK primitive batch.
	if (cycloid.number_of_vertices > 
		max_vert - (cycloid.copy_first_to_end ? 1 : 0))
		cycloid.number_of_vertices = 
		max_vert - (cycloid.copy_first_to_end ? 1 : 0);

	// Calculate the raw coordinates, and find max absolute component.
	auto i = 0;
	for (; i < cycloid.number_of_vertices; ++i)
	{
		raw[i].a =
			1.0 / static_cast<double>(cycloid.number_of_vertices) * 
			(i - cycloid.number_of_vertices / 2.0) *
			XM_2PI * cycloid.cycles;
		raw[i].x =
			(cycloid.radius_1 - cycloid.radius_2) * cos(raw[i].a) +
			recalculate_arm_length * 
			cos((cycloid.radius_1 - cycloid.radius_2) / cycloid.radius_2 * 
				raw[i].a);
		raw[i].y =
			(cycloid.radius_1 - cycloid.radius_2) * sin(raw[i].a) -
			recalculate_arm_length * 
			sin((cycloid.radius_1 - cycloid.radius_2) / cycloid.radius_2 * 
				raw[i].a);
		raw[i].d = sqrt(pow(raw[i].x, 2) + pow(raw[i].y, 2));
		raw[i].p = atan2(raw[i].y, raw[i].x);
		if (abs(raw[i].p) != raw[i].p)
			raw[i].p = static_cast<double>(XM_2PI) + raw[i].p;
		max_dist = max(max_dist, raw[i].d);
	}
	if (cycloid.copy_first_to_end)
		// Copy the starting raw vertex to the end of the used array.
		raw[i] = raw[0];

	for (auto j = 0; 
	     j < cycloid.number_of_vertices + (cycloid.copy_first_to_end ? 1 : 0); 
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

void		cycloid::random_cycloid(cycloid_parameters &cycloid) const
{

	// Generate random radius_2 [0.2, 0.8] in 0.001 increments.
	cycloid.radius_2 = 
		get_cdx11_frame()->get_rng()->get_rand_int(200, 801) / 1000.0;
	// Generate random arm length [0.2,2.0] in 0.01 increments.
	cycloid.arm_length = 
		get_cdx11_frame()->get_rng()->get_rand_int(20, 201) / 100.0;
	// Generate a random color.
	cycloid.r = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	cycloid.g = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	cycloid.b = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);

}

void		cycloid::convert_to_screen(
	const cycloid_parameters& cycloid,
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