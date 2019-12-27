#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <map>
#include<string>

class TextureManager
{
public:
    static TextureManager* getInstance();

    unsigned int loadTexture(std::string fileName);

    void activateTexture(std::string fileName);

private:
    TextureManager();
    void initialize();

    static TextureManager* m_Instance;

    std::map<std::string,unsigned int>  m_TextureMap;

};

#endif