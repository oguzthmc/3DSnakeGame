/**   
 * @file  		    Kup.cpp
 * @description     Küpe ait hareket, dönüş, döndürme, öteleme, pozisyon ve kaplama koordinatları işlemleri yapıldı.
 *                  Kamera, Projeksiyon, Öteleme ve Döndürme matrisleri hesaplanarak, bunların çarpımıyla Dönüşüm matrisi elde edildi.
 * @course  	    Bilgisayar Grafiği
 * @assignment      Proje
 * @authors  	    Oğuzhan Tohumcu 2A B181210397  
 *				    Abdurrahman Doğan 1A B181210394
*/

#include "Kup.hpp"
#include <glad/glad.h>
#include<cmath>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>


Kup::Kup(float uzunluk,glm::vec3 pos,glm::mat4 transform,std::string Donus,std::vector<unsigned int>& indices,std::vector<Vertex>& vertices,int kaplamaNo)
{
    m_Uzunluk = uzunluk;            //küpün kenar uzunlugu atanıyor
    m_Yon = DIR_RIGHT;              //küpün hareket edeceği yön atanıyor
    m_Position = pos;               //küpün çizileceği pozisyon bilgisi atanıyor  
    m_Donus = Donus;                //dönüş yönü bilgisi atanıyor, pozitif yada negatif şeklinde buna göre yukarı aşağı dönecek
    mtxTransform = transform;       //transform matrisi; küp ile ilgili döndürme, öteleme gibi işlemler küpe özel olan bu matris üzerinden yapılacak
    m_KaplamaNo = kaplamaNo;        //uygulanacak kaplama numarası

    kupOlustur(uzunluk,indices,vertices);
 
}

Kup::DIRECTION Kup::getYon()
{
    return m_Yon;
}

void Kup::setYon(Kup::DIRECTION dir)
{
    m_Yon = dir;
}

glm::vec3 Kup::getPosition()
{
    return m_Position;
}

glm::mat4 Kup::getTransform(){
    return mtxTransform;
}

void Kup::setTransform(float aci,float camPos){
    
    glm::vec3 camPosition(0.0f,0.0f,camPos);    //kameranın pozisyonu
    glm::vec3 camLookAt(0.0f,0.0f,0.0f);        //kameranın baktığı yer (orijin)
    glm::vec3 camUp(0.0f,1.0f,0.0f);            //kameranın yön vektörü (y eksenine paralel)

    //lookAt fonksiyonu ile kamera matrisi oluşturuluyor
    glm::mat4 mtxCam=glm::lookAt(camPosition,camLookAt,camUp);
    //perspective fonksiyonu ile 4 boyutlu projeksiyon matrisi oluşturuluyor
    glm::mat4 mtxProj= glm::perspective(glm::radians(90.0f),(800.0f/800.0f),1.0f,100.0f);
    //translate fonksiyonu ile öteleme matrisi oluşturuluyor
    glm::mat4 mtxTranslation=glm::translate(glm::mat4(1),m_Position);

    //rotate fonksiyonu ile şeklin döndürme matrisi oluşturuluyor
    //x ekseni etrafında döndürülüyor
    glm::mat4 mtxRotation = glm::rotate(glm::mat4(1),glm::radians(aci),glm::vec3(1.0f,0.0f,0.0f));

    //4 matrisin çarpılmasıyla dönüşüm matrisi oluşturuluyor
    mtxTransform = mtxProj*mtxCam*mtxTranslation*mtxRotation;
}

int Kup::getKaplamaNo(){
    return m_KaplamaNo;
}

std::string Kup::getRotation(){
    return m_Donus;
} 

void Kup::kupOlustur(float uzunluk,std::vector<unsigned int>& indices,std::vector<Vertex>& vertices){
    glm::vec3 v[8];     //8 farklı noktadan oluşan koordinat vektörü
    
    //küp vektörünün 3 boyutlu noktaları belirleniyor
    v[0] = glm::vec3(-uzunluk/2,-uzunluk/2,-uzunluk/2);
    v[1] = glm::vec3(uzunluk/2,-uzunluk/2,-uzunluk/2);
    v[2] = glm::vec3(uzunluk/2,-uzunluk/2,uzunluk/2);
    v[3] = glm::vec3(-uzunluk/2,-uzunluk/2,uzunluk/2);
    //y'ler pozitif olarak değiştiriliyor
    v[4] = glm::vec3(-uzunluk/2,uzunluk/2,-uzunluk/2);
    v[5] = glm::vec3(uzunluk/2,uzunluk/2,-uzunluk/2);
    v[6] = glm::vec3(uzunluk/2,uzunluk/2,uzunluk/2);
    v[7] = glm::vec3(-uzunluk/2,uzunluk/2,uzunluk/2);
    
    //6 farklı yüzeyde toplam 24 nokta ve 24 farklı kaplama koordinatı kullanılıyor
    Vertex tempVertices[24];
    //On Yuzey
    tempVertices[0].pos = v[7];
    tempVertices[1].pos = v[3];
    tempVertices[2].pos = v[2];
    tempVertices[3].pos = v[6];
    tempVertices[0].tex = glm::vec2(0.0f,1.0f);
    tempVertices[1].tex = glm::vec2(0.0f,0.0f);
    tempVertices[2].tex = glm::vec2(1.0f,0.0f);
    tempVertices[3].tex = glm::vec2(1.0f,1.0f);
    //Sag Yuzey
    tempVertices[4].pos = v[6];
    tempVertices[5].pos = v[2];
    tempVertices[6].pos = v[1];
    tempVertices[7].pos = v[5];
    tempVertices[4].tex = glm::vec2(0.0f,1.0f);
    tempVertices[5].tex = glm::vec2(0.0f,0.0f);
    tempVertices[6].tex = glm::vec2(1.0f,0.0f);
    tempVertices[7].tex = glm::vec2(1.0f,1.0f);
    //Ust Yuzey
    tempVertices[8].pos = v[4];
    tempVertices[9].pos = v[7];
    tempVertices[10].pos = v[6];
    tempVertices[11].pos = v[5];
    tempVertices[8].tex = glm::vec2(0.0f,1.0f);
    tempVertices[9].tex = glm::vec2(0.0f,0.0f);
    tempVertices[10].tex = glm::vec2(1.0f,0.0f);
    tempVertices[11].tex = glm::vec2(1.0f,1.0f);

    //Alt Yüzey
    tempVertices[12].pos = v[0];
    tempVertices[13].pos = v[3];
    tempVertices[14].pos = v[2];
    tempVertices[15].pos = v[1];
    tempVertices[12].tex = glm::vec2(0.0f,1.0f);
    tempVertices[13].tex = glm::vec2(0.0f,0.0f);
    tempVertices[14].tex = glm::vec2(1.0f,0.0f);
    tempVertices[15].tex = glm::vec2(1.0f,1.0f);
    //Arka Yuzey
    tempVertices[16].pos = v[4];
    tempVertices[17].pos = v[0];
    tempVertices[18].pos = v[1];
    tempVertices[19].pos = v[5];
    tempVertices[16].tex = glm::vec2(0.0f,1.0f);
    tempVertices[17].tex = glm::vec2(0.0f,0.0f);
    tempVertices[18].tex = glm::vec2(1.0f,0.0f);
    tempVertices[19].tex = glm::vec2(1.0f,1.0f);
    //Sol Yuzey
    tempVertices[20].pos = v[7];
    tempVertices[21].pos = v[3];
    tempVertices[22].pos = v[0];
    tempVertices[23].pos = v[4];
    tempVertices[20].tex = glm::vec2(0.0f,1.0f);
    tempVertices[21].tex = glm::vec2(0.0f,0.0f);
    tempVertices[22].tex = glm::vec2(1.0f,0.0f);
    tempVertices[23].tex = glm::vec2(1.0f,1.0f);

    //oluşturulan 24 nokta Vertex tipindeki vektöre kopyalanır 
    for(int i = 0; i < 24; i++)
        vertices.push_back(tempVertices[i]);

    //6 farklı yüzeyin 4'er farklı noktasına göre 2'şer üçgen koordinatı olarak index vektörüne ekleniyor
    for(int i = 0; i < 6; i++)
    {
        int startIndex = 4*i;
        indices.push_back(startIndex);
        indices.push_back(startIndex+1);
        indices.push_back(startIndex+2);
        
        indices.push_back(startIndex);
        indices.push_back(startIndex+2);
        indices.push_back(startIndex+3);
    }
} 

void Kup::move()
{
    switch(m_Yon)
    {
        case DIR_LEFT:
            m_Position -= glm::vec3(m_Uzunluk,0.0f,0.0f);
            break;
        case DIR_RIGHT:
            m_Position += glm::vec3(m_Uzunluk,0.0f,0.0f);
            break;
        case DIR_UP:
            m_Position += glm::vec3(0.0f,m_Uzunluk,0.0f);
            break;
        case DIR_DOWN:
            m_Position -= glm::vec3(0.0f,m_Uzunluk,0.0f);
            break;                        
    }
}

