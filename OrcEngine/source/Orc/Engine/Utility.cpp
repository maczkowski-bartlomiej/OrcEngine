#include "Engine/Utility.hpp"

#include <string>

#include <system_error>

namespace orc {

std::string getErrnoMessage(int error)
{
	return std::system_category().message(error);
}

}
