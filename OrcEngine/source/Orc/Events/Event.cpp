#include "orcPch.hpp"
#include "Events/Event.hpp"

namespace orc {

Event::Event(const Type& type)
	: m_type(type), m_handled(false)
{
}

void Event::markHandled() 
{
	m_handled = true;
}

Event::Type Event::getType() const 
{
	return m_type;
}

bool Event::isHandled() const 
{
	return m_handled;
}

}
