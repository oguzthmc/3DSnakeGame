#ifndef KUP_HPP
#define KUP_HPP
#include <glad/glad.h>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <vector>
#include<glm/glm.hpp>
#include <string>


struct Vertex{
    glm::vec3 pos;
    glm::vec2 tex;
};

class Kup
{
public:

    enum DIRECTION
    {
        DIR_RIGHT   =  1,
        DIR_LEFT    = -1,
        DIR_UP      =  2,
        DIR_DOWN    = -2

    };
    
    Kup(float uzunluk,glm::vec3 pos,glm::mat4 transform,std::string Donus,std::vector<unsigned int>& indices,std::vector<Vertex>& vertices,int kaplamaNo);
    
    DIRECTION getYon();

    void setYon(DIRECTION dir);
    void setTransform(float aci,float camPos);
    glm::vec3 getPosition();
    glm::mat4 getTransform();
    int getKaplamaNo();

    void move();
    void kupOlustur(float uzunluk,std::vector<unsigned int>& indices,std::vector<Vertex>& vertices);
    std::string getRotation();                

private:
    DIRECTION           m_Yon;
    int                 m_KaplamaNo;
    float               m_Uzunluk;
    glm::vec3           m_Position;
    glm::mat4           mtxTransform;
    std::string         m_Donus;
    
};


#endif