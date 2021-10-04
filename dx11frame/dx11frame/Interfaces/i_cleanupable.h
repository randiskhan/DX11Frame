#pragma once

class i_cleanupable  // NOLINT(cppcoreguidelines-special-member-functions)
{

public:
	// Construction/Destruction
	i_cleanupable() = default;
	virtual ~i_cleanupable() = default;

	// Pure virtual members
	virtual void cleanup() = 0;

};
