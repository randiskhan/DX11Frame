// centered_trochoid.h
// Declaration file for centered_trochoid.

#pragma once

// Maximum number of vertices.
// Max DTK can handle is 65534.
constexpr int max_vertices = 32768;
constexpr int default_vertices_per_cycle = 512;
constexpr int max_cycles = 24;

#include "dx11_frame.h"
#include "i_entity.h"

struct double_point
{

	double	x; // This vertex's coordinate.
	double	y; // This vertex's coordinate.
	double	a; // Angle used to calculate this vertex.
	double	d; // This vertex's distance from origin.
	double	p; // This vertex's angle in polar coordinates.

	double_point()
	{
		x = y = a = d = p = 0.0;
	}

};

struct centered_trochoid_parameters
{

	// Parameters for centered trochoid.
	// Fixed circle radius: radius_1
	// Moving circle radius: radius_2
	// Drawing point distance from center of moving circle: arm_length
	// I find it easiest to "normalize" these values. I keep radius_1 at 1.0,
	// so I can think of radius_2 as a ratio to/of radius_1. This also helps
	// with arm_length - If radius_1 is 1, then when arm_length equals
	// radius_2, the drawing point is exactly on the circumference of the
	// moving circle.
	const double		radius_1 = 1.0;
	double				radius_2;
	double				arm_length;
	double				cycles;

	int					number_of_vertices_per_cycle;
	int					number_of_vertices;

	bool				copy_first_to_end;

	float				r, g, b;

	centered_trochoid_parameters::centered_trochoid_parameters()
	{

		radius_2 = 0.25;
		arm_length = 1.0;
		cycles = 1.0;
		number_of_vertices_per_cycle = default_vertices_per_cycle;
		number_of_vertices = max_vertices;
		copy_first_to_end = false;
		r = g = b = 1.0f;

	}

	bool	centered_trochoid_parameters::calculate_needed_cycles(
		const int max_cycles)
	{

		auto found = false;
		auto cycles_candidate = 1;
		while (radius_2 != 0)
		{
			if (cycles_candidate / radius_2 == 
				static_cast<int>(cycles_candidate / radius_2))
			{
				found = true;
				break;
			}
			if (cycles_candidate > max_cycles) 
				break;
			if (number_of_vertices_per_cycle * 
				cycles_candidate > max_vertices - 1) 
				break;
			++cycles_candidate;
		}
		cycles = 
			cycles_candidate + 
			1.0 / static_cast<double>(number_of_vertices_per_cycle);
		// Also base the number of vertices on how many cycles are needed.
		number_of_vertices = number_of_vertices_per_cycle * cycles_candidate;

		return found;

	}

	void	centered_trochoid_parameters::copy_to(
		centered_trochoid_parameters& centered_trochoid) const
	{

		centered_trochoid.radius_2 = radius_2;
		centered_trochoid.arm_length = arm_length;
		centered_trochoid.cycles = cycles;
		centered_trochoid.number_of_vertices = number_of_vertices;
		centered_trochoid.number_of_vertices_per_cycle = 
			number_of_vertices_per_cycle;
		centered_trochoid.copy_first_to_end = copy_first_to_end;
		centered_trochoid.r = r;
		centered_trochoid.g = g;
		centered_trochoid.b = b;

	}

};

// ReSharper disable once CommentTypo
class centered_trochoid final : public i_entity  // NOLINT(cppcoreguidelines-special-member-functions)
{

	unique_ptr<PrimitiveBatch<VertexPositionColor>>	primitive_batch_{};
	unique_ptr<BasicEffect>							basic_effect_{};
	ID3D11InputLayout*								i_d3d11_input_layout_{};

	// Array of vertices to send to GPU.
	VertexPositionColor			vertices_[max_vertices]{};
	// Array of raw vertices in coordinate [-1, 1] range.
	double_point				vertices_raw_[max_vertices];

	centered_trochoid_parameters	centered_trochoid_current_;
	centered_trochoid_parameters	centered_trochoid_next_;
	centered_trochoid_parameters	centered_trochoid_previous_;

	double	time_delta_morph_{};
	double	time_delta_new_centered_trochoid_{};
	double	time_stamp_morph_{};
	double	time_stamp_new_centered_trochoid_{};

public:
	explicit centered_trochoid(dx11_frame* dx11_frame);
	~centered_trochoid() override;

	bool	init() override;
	bool	update() override;
	bool	render() override;
	void	cleanup() override;

private:
	void	calculate_raw_vertices(
		centered_trochoid_parameters&,
		double_point[],
		int) const;
	// Color the raw vertices based on the angular position of the inner
	// circle to the outer circle (angle used in centered_trochoid calculation).
	void	color_vertices_by_angle_position(
		const centered_trochoid_parameters&,
		double_point [],
		VertexPositionColor []) const;
	// Color the raw vertices based on the azimuth of the polar coordinate
	// of the raw vertex after the quarter turn counter-clockwise.
	void	color_vertices_by_polar_coordinates(
		const centered_trochoid_parameters&,
		double_point [],
		VertexPositionColor []) const;
	// Pick a random color for all vertices.
	void	color_vertices_by_random(
		const centered_trochoid_parameters& centered_trochoid,
		VertexPositionColor vert[]) const;
	void	random_hypocycloid(centered_trochoid_parameters &) const;
	static void	convert_to_screen(
		const centered_trochoid_parameters&,
		double_point [],
		VertexPositionColor [],
		RECT);

};
