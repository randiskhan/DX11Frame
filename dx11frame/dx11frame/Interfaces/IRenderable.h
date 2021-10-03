#pragma once

// ReSharper disable once CommentTypo
class i_renderable  // NOLINT(cppcoreguidelines-special-member-functions)
{

protected:
	bool			do_render_;

public:

	// Construction/Destruction
	i_renderable() :do_render_(false) {}
	virtual			~i_renderable() = default;

	// Pure virtual members
	virtual bool	render() = 0;

	// Instance members
	void			set_do_render(const bool render) { do_render_ = render; }
	bool			get_do_render() const { return do_render_; }

};
