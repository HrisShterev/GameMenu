#pragma once

#include <string>

class Action
{
private:
    std::string m_name;  // The name of the action (e.g., "MoveLeft", "Jump")
    std::string m_type;  // The type of action (e.g., "START", "STOP")

public:
    Action();
    Action(const std::string& name, const std::string& type);

    // Getter methods
    const std::string& name() const;
    const std::string& type() const;
};
