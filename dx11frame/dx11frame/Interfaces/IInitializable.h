#pragma once

// ReSharper disable once CommentTypo
class i_initializable  // NOLINT(cppcoreguidelines-special-member-functions)
{

protected:
	bool			is_init_;

public:

	// Construction/Destruction
	i_initializable() : is_init_(false) {}
	virtual			~i_initializable() = default;

	// Pure virtual members
	virtual bool	init() = 0;

	// Instance members
	bool			is_init() const { return is_init_; }

};
