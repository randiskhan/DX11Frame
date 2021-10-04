#pragma once

#include <random>

class rng
{

public:
	rng() : gen_(std::random_device()()) {}  // NOLINT(modernize-use-equals-default)

    int get_rand_int(const int min, const int max)
	{
	    const std::uniform_int_distribution<int>
			urd_int(min, max);
        return urd_int(gen_);
    }

    float get_rand_float(const float min, const float max)
	{
	    const std::uniform_real_distribution<float>
			urd_float(min, max);
        return urd_float(gen_);
    }

    double get_rand_double(const double min, const double max)
	{
	    const std::uniform_real_distribution<double>
			urd_double(min, max);
        return urd_double(gen_);
    }

private:
    std::mt19937 gen_;

};

