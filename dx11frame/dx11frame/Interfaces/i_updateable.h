#pragma once

// ReSharper disable once CommentTypo
class i_updateable  // NOLINT(cppcoreguidelines-special-member-functions)
{

protected:
	bool			do_update_;

public:

	// Construction/Destruction
	i_updateable() : do_update_(false) {}
	virtual			~i_updateable() = default;

	// Pure virtual members
	virtual bool	update() = 0;

	// Instance members
	void			set_do_update(const bool update) { do_update_ = update; }
	bool			get_do_update() const { return do_update_; }

};
