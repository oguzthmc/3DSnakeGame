/**   
 * @file  		    TextureManager.cpp
 * @description     Tek bir instance oluşturma ayarları ile yükleme, aktif etme ve bağlama gibi kaplama yönetimi işlemleri yapılır.
 * @course  	    Bilgisayar Grafiği
 * @assignment      Proje
 * @authors  	    Oğuzhan Tohumcu 2A B181210397  
 *				    Abdurrahman Doğan 1A B181210394
*/

#include "TextureManager.hpp"
#include<glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//m_Instance statik bir değişken olduğu için ilk değeri global scope'ta atanıyor.
TextureManager* TextureManager::m_Instance(0);      

TextureManager::TextureManager()
{
    initialize();
}

void TextureManager::initialize()
{

}
//Kaplamayı aktif eden fonksiyon
void TextureManager::activateTexture(std::string fileName)
{
    if(m_TextureMap.count(fileName)!=0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureMap[fileName]);
    }
}
//Kaplamayı yükleyen ve kaplamaya ait id değerini döndüren fonksiyon
unsigned int TextureManager::loadTexture(std::string fileName)
{
    //hatalı olduğunu gösteriyor.
    unsigned int id = -1;
    //0 dan farklıysa daha önce oluşturulan kaplama nesnesinin id sini çek.
    if(m_TextureMap.count(fileName)!=0)
    {
        id = m_TextureMap[fileName];
    }
    else
    {
        int width, height, nrChannels;
        
        //dosyayı oku
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0); 
      
        unsigned int textureId;
        
        //Nesne oluşturulur, bağlanır (id ilişkilendirmesi) ve gerekli ayarlamalar yapılır.
        glGenTextures(1, &textureId);  
        glBindTexture(GL_TEXTURE_2D, textureId);  
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_TextureMap[fileName] = textureId; //haritadaki ilgili dosyaya id değeri atanır

        id = textureId;
        
        stbi_image_free(data);  //gereksiz hafıza alanını serbest bırak
    }
    
    return id; 
}
//m_Instance üzerinden private elemana erişerek nesne oluşturuluyor.
TextureManager* TextureManager::getInstance()
{
    if(!m_Instance)
        m_Instance = new TextureManager();

    return m_Instance;
}