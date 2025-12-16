#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//#include "Animation.h"

class Assets
{
private:
    std::map<std::string, sf::Texture>   m_textures;
    std::map<std::string, sf::SoundBuffer> m_sounds;
    std::map<std::string, sf::Font>      m_fonts;

public:
    // Load assets
    void addTexture(const std::string& name, const std::string& path);
    void addSound(const std::string& name, const std::string& path);
    void addFont(const std::string& name, const std::string& path);

    // Retrieve assets
    const sf::Texture& getTexture(const std::string& name) const;
    const sf::SoundBuffer& getSound(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
};
