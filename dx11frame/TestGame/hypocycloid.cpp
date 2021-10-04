// centered_trochoid.cpp
// Implementation file for centered_trochoid.

#include "hypocycloid.h"

using namespace dx11_frame_helpers;

centered_trochoid::centered_trochoid(dx11_frame* dx11_frame) :
	i_entity(dx11_frame)
{
	centered_trochoid::init();
}

centered_trochoid::~centered_trochoid()
{
	centered_trochoid::cleanup();
}

bool		centered_trochoid::init()
{

	auto good = true;

	auto valid = false;
	while (!valid)
	{
		random_hypocycloid(centered_trochoid_previous_);
		valid = 
			centered_trochoid_previous_.calculate_needed_cycles(max_cycles);
	}
	valid = false;
	while (!valid)
	{
		random_hypocycloid(centered_trochoid_next_);
		valid = centered_trochoid_next_.calculate_needed_cycles(max_cycles);
	}

	time_delta_morph_ = 1.0;
	time_delta_new_centered_trochoid_ = 15.0;
	time_stamp_morph_ = 0;
	time_stamp_new_centered_trochoid_ = 0;

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

bool		centered_trochoid::update()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	const auto t = get_cdx11_frame()->get_timer()->get_total_elapsed();

	if (time_stamp_new_centered_trochoid_ + time_delta_new_centered_trochoid_ 
		< t)
	{
		time_stamp_new_centered_trochoid_ = t;
		centered_trochoid_current_.copy_to(centered_trochoid_previous_);
		auto valid = false;
		while (!valid)
		{
			random_hypocycloid(centered_trochoid_next_);
			valid = 
				centered_trochoid_next_.calculate_needed_cycles(max_cycles);
		}
	}
	const auto lerp_amt = 
		(t - time_stamp_new_centered_trochoid_) / 
		(time_delta_new_centered_trochoid_ - 2.0);
	centered_trochoid_current_.arm_length = interpolate_cos(
		centered_trochoid_previous_.arm_length, 
		centered_trochoid_next_.arm_length, 
		lerp_amt);
	centered_trochoid_current_.radius_2 = interpolate_cos(
		centered_trochoid_previous_.radius_2, 
		centered_trochoid_next_.radius_2, 
		lerp_amt);
	centered_trochoid_current_.cycles = interpolate_cos(
		centered_trochoid_previous_.cycles, 
		centered_trochoid_next_.cycles, 
		lerp_amt);
	centered_trochoid_current_.r = interpolate_cos(
		centered_trochoid_previous_.r, 
		centered_trochoid_next_.r, 
		static_cast<float>(lerp_amt));
	centered_trochoid_current_.g = interpolate_cos(
		centered_trochoid_previous_.g, 
		centered_trochoid_next_.g, 
		static_cast<float>(lerp_amt));
	centered_trochoid_current_.b = interpolate_cos(
		centered_trochoid_previous_.b, 
		centered_trochoid_next_.b, 
		static_cast<float>(lerp_amt));

	calculate_raw_vertices(
		centered_trochoid_current_,
		vertices_raw_,
		max_vertices);
	convert_to_screen(
		centered_trochoid_current_,
		vertices_raw_,
		vertices_,
		get_cdx11_frame()->get_win32()->get_screen_rect());
	for (auto i = 0; i < centered_trochoid_current_.number_of_vertices; ++i)
	{
		vertices_[i].color.x = centered_trochoid_current_.r;
		vertices_[i].color.y = centered_trochoid_current_.g;
		vertices_[i].color.z = centered_trochoid_current_.b;
	}

	return good;

}

bool		centered_trochoid::render()
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
		centered_trochoid_current_.number_of_vertices + 
			(centered_trochoid_current_.copy_first_to_end ? 1 : 0)
		);
	primitive_batch_->End();

	return good;

}

void		centered_trochoid::cleanup()
{
	safe_release(i_d3d11_input_layout_);
}

#pragma region Private methods

void		centered_trochoid::color_vertices_by_angle_position(
	const centered_trochoid_parameters& centered_trochoid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < centered_trochoid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].a));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].a + XM_2PI * 2.0 / 3.0));
	}

}

void		centered_trochoid::color_vertices_by_polar_coordinates(
	const centered_trochoid_parameters& centered_trochoid,
	double_point raw[],
	VertexPositionColor vert[]) const
{

	for (auto i = 0; i < centered_trochoid.number_of_vertices; ++i)
	{
		vert[i].color.x = 
			static_cast<float>(norm_sin(raw[i].p));
		vert[i].color.y = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI / 3.0));
		vert[i].color.z = 
			static_cast<float>(norm_sin(raw[i].p + XM_2PI * 2.0 / 3.0));
	}

}

void		centered_trochoid::color_vertices_by_random(
	const centered_trochoid_parameters& centered_trochoid,
	VertexPositionColor vert[]) const
{

	const float temp_x = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_y = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	const float temp_z = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	for (auto i = 0; i < centered_trochoid.number_of_vertices; ++i)
	{
		vert[i].color.x = temp_x;
		vert[i].color.y = temp_y;
		vert[i].color.z = temp_z;
	}

}

void		centered_trochoid::calculate_raw_vertices(
	centered_trochoid_parameters &centered_trochoid,
	double_point raw[],
	const int max_vert) const
{

	double max_dist = 0;
	// Recalculate the drawing point position.
	const double recalculate_arm_length = 
		centered_trochoid_current_.arm_length * 
		centered_trochoid_current_.radius_2;

	// Verify the number of vertices does not exceed our array and max count
	// for DTK primitive batch.
	if (centered_trochoid.number_of_vertices > 
		max_vert - (centered_trochoid.copy_first_to_end ? 1 : 0))
		centered_trochoid.number_of_vertices = 
		max_vert - (centered_trochoid.copy_first_to_end ? 1 : 0);

	// Calculate the raw coordinates, and find max absolute component.
	auto i = 0;
	for (; i < centered_trochoid.number_of_vertices; ++i)
	{
		raw[i].a =
			1.0 / static_cast<double>(centered_trochoid.number_of_vertices) * 
			(i - centered_trochoid.number_of_vertices / 2.0) *
			XM_2PI * centered_trochoid.cycles;
		raw[i].x =
			(centered_trochoid.radius_1 - centered_trochoid.radius_2) * 
			cos(raw[i].a) +
			recalculate_arm_length * 
			cos((centered_trochoid.radius_1 - centered_trochoid.radius_2) / 
				centered_trochoid.radius_2 * 
				raw[i].a);
		raw[i].y =
			(centered_trochoid.radius_1 - centered_trochoid.radius_2) * 
			sin(raw[i].a) -
			recalculate_arm_length * 
			sin((centered_trochoid.radius_1 - centered_trochoid.radius_2) / 
				centered_trochoid.radius_2 * 
				raw[i].a);
		raw[i].d = sqrt(pow(raw[i].x, 2) + pow(raw[i].y, 2));
		raw[i].p = atan2(raw[i].y, raw[i].x);
		if (abs(raw[i].p) != raw[i].p)
			raw[i].p = static_cast<double>(XM_2PI) + raw[i].p;
		max_dist = max(max_dist, raw[i].d);
	}
	if (centered_trochoid.copy_first_to_end)
		// Copy the starting raw vertex to the end of the used array.
		raw[i] = raw[0];

	for (auto j = 0; 
	     j < centered_trochoid.number_of_vertices + 
			(centered_trochoid.copy_first_to_end ? 1 : 0); 
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

void		centered_trochoid::random_hypocycloid(
	centered_trochoid_parameters &centered_trochoid) const
{

	// Generate random radius_2 [0.2, 0.8] in 0.001 increments.
	centered_trochoid.radius_2 = 
		get_cdx11_frame()->get_rng()->get_rand_int(200, 801) / 1000.0;
	// Generate random arm length [0.2,2.0] in 0.01 increments.
	centered_trochoid.arm_length = 
		get_cdx11_frame()->get_rng()->get_rand_int(20, 201) / 100.0;
	// Generate a random color.
	centered_trochoid.r = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	centered_trochoid.g = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);
	centered_trochoid.b = 
		get_cdx11_frame()->get_rng()->get_rand_float(0.0f, 1.0f);

}

void		centered_trochoid::convert_to_screen(
	const centered_trochoid_parameters& centered_trochoid,
	double_point raw[],
	VertexPositionColor vert[],
	const RECT canvas)
{

	const int max = min(canvas.right, canvas.bottom);
	for (auto i = 0; 
		i < centered_trochoid.number_of_vertices + 
			(centered_trochoid.copy_first_to_end ? 1 : 0); 
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