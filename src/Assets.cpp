#include "Assets.h"
#include <iostream>

// Load a texture from file and store it in the map
void Assets::addTexture(const std::string& name, const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }
    m_textures[name] = texture;
}

// Load a sound from file and store it in the map
void Assets::addSound(const std::string& name, const std::string& path)
{
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(path))
    {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return;
    }
    m_sounds[name] = soundBuffer;
}

// Load a font from file and store it in the map
void Assets::addFont(const std::string& name, const std::string& path)
{
    sf::Font font;
    if (!font.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << path << std::endl;
        return;
    }
    m_fonts[name] = font;
}

// Retrieve a texture
const sf::Texture& Assets::getTexture(const std::string& name) const
{
    return m_textures.at(name);
}

// Retrieve a sound buffer
const sf::SoundBuffer& Assets::getSound(const std::string& name) const 
{
    return m_sounds.at(name);
}

// Retrieve a font
const sf::Font& Assets::getFont(const std::string& name) const
{
    return m_fonts.at(name);
}
