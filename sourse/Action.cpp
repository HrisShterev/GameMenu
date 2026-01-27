#include "Action.h"
#include <sstream>

// Default constructor (optional, depends on how you use it)
Action::Action()
{
}

// Constructor with parameters
Action::Action(const std::string& name, const std::string& type)
    : m_name(name), m_type(type)
{
}

void Action::setCharacter(char c)
{
    m_character = c;
}

char Action::character() const
{
    return m_character;
}

// Getter for action name
const std::string& Action::name() const
{
    return m_name;
}

// Getter for action type
const std::string& Action::type() const
{
    return m_type;
}
