/**   
 * @file  		    main.cpp
 * @description     Yılan hareketi (yön tuşları), çizim, kuyruğa eleman ekleme (space tuşu) yapıldı. 
 *                  Rastgele olarak dönüşüm işlemleri uygulandı ve kaplama giydirildi.
 * @course  	    Bilgisayar Grafiği
 * @assignment      Proje
 * @authors  	    Oğuzhan Tohumcu 2A B181210397  
 *				    Abdurrahman Doğan 1A B181210394
*/ 

#include "TextureManager.hpp"
#include "ShaderProgram.hpp"
#include "Kup.hpp"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include<glad/glad.h>
#include<glm/vec3.hpp>
#include<GLFW/glfw3.h>
#include<vector>
#include <glm/gtc/matrix_transform.hpp>     //3 boyutlu dönüşüm işlemleri için eklenen kütüphane
#include <time.h>
#include <cmath>

float aci = 0.0f;
float tersAci = 0.0f;
float camPos = 1.5f;                        //z ekseni için kamera pozisyonu
std::string m_Donus;
std::vector<Vertex>         vertices;
std::vector<unsigned int>   indices;
glm::mat4 mtxTransform(1);                  //4 boyutlu dönüşüm matrisi
glm::vec3 position;                         //3 boyutlu nokta

//kaplama resimleri string olarak tutuluyor
std::string textureNames[]={"./images/brick.jpg","./images/pusula.jpg","./images/container.jpg","./images/lisa.jpg","./images/saat.jpg"};
std::vector<Kup*>    snakeList;
//kaplamaları yönetmek için, TextureManager örneği oluşturuluyor
TextureManager* textureManager = TextureManager::getInstance();
float m_Length=0.2f;

//OpenGL nesnelerinin id değerlerini tutacak olan değişkenler
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;


//Shader programın açısına göre dönüşüm uygulayan ve kaplamayı aktif ederek çizimi yapan fonksiyon.
void Ciz(ShaderProgram& program)
{
    aci += 3;
    tersAci -= 3;
    //küp vektörü üzerinden dönüşüm işlemi yapılıyor
    for(auto next:snakeList)
    {
        if(next->getRotation() == "pozitif"){
            next->setTransform(aci,camPos);
        }
        else{
            next->setTransform(tersAci,camPos);
        }
        glm::mat4 transform = next->getTransform();
        program.setMat4("uMtxTransform", &transform);
        //kaplama aktif ediliyor
        textureManager->activateTexture(textureNames[next->getKaplamaNo()]);
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

//Yılanın herhangi bir yönde (sağ, sol, yukarı ve aşağı) hareketini sağlayan fonksiyon.
void moveSnake()
{
    for(auto next:snakeList)
    {
        next->move();
    }

    for(int i = snakeList.size()-1; i > 0; i--)
    {
        snakeList[i]->setYon(snakeList[i-1]->getYon());
    }      
}

//Daha önce hiç oluşturulmuş yılan küpü yoksa oluşturan; varsa yılanın kuyruğuna ekleyen fonksiyon.
//Bunu yaparken kaplamayı rastgele seçer ve yılanın oluşturulacağı pozisyon, yön ve dönüşüm değerlerini de hesaplar.
void addToSnakeTail()
{
    int elementCount = snakeList.size();
    int randNumber = rand() % 100;
    int randKaplamaNo = 0;
    randKaplamaNo = rand() % 5;

    if(randNumber % 2 == 0) {  
        m_Donus="pozitif";
    }
    else {
        m_Donus="negatif";
    }

    if(elementCount == 0)
    {
        snakeList.push_back(new Kup(m_Length,glm::vec3(0.0f,0.0f,0.0f),mtxTransform,m_Donus,indices,vertices,randKaplamaNo));
    }
    else
    {
        Kup* lastKup = snakeList[elementCount-1];

        glm::vec3 pos = lastKup->getPosition();

        Kup::DIRECTION dir = lastKup->getYon();

        switch(dir)
        {
            case Kup::DIR_RIGHT:
                pos-=glm::vec3(m_Length,0.0f,0.0f);
                break;
            case  Kup::DIR_LEFT:
                pos+=glm::vec3(m_Length,0.0f,0.0f);
                break;
            case  Kup::DIR_UP:
                pos-=glm::vec3(0.0f,m_Length,0.0f);
                break;    
            case  Kup::DIR_DOWN:
                pos+=glm::vec3(0.0f,m_Length,0.0f);
                break;              
        }
        Kup* newKup = new Kup(m_Length,pos,mtxTransform,m_Donus,indices,vertices,randKaplamaNo);

        newKup->setYon(dir);

        snakeList.push_back(newKup);
    }
}

//Yön tuşları kombinasyonlarının algılanmasının sağlandığı fonksiyon.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();   
    if(action==GLFW_PRESS)
    {
        if(snakeList.size()!=0)
        {
            Kup* first = snakeList[0];
            if(key == GLFW_KEY_A)
            {
                camPos-=0.2f;                        
            }
            if(key == GLFW_KEY_D)
            {
                camPos+=0.2f;
            }              
            if(key == GLFW_KEY_LEFT)  
            {
                first->setYon(Kup::DIR_LEFT);
            }
            if(key == GLFW_KEY_RIGHT) 
            {
                first->setYon(Kup::DIR_RIGHT);
            }    
            if(key == GLFW_KEY_UP)  
            {
                first->setYon(Kup::DIR_UP);
            }
            if(key == GLFW_KEY_DOWN) 
            {
                first->setYon(Kup::DIR_DOWN);
            }             
        }
        
        if(key == GLFW_KEY_SPACE)
            addToSnakeTail();          
    }
}

//Program ekranı ayarları, tuş kombinasyonları, kaplama yönetimi, yılan (küp) ekleme, shader bağlamaları,
//çeşitli gl fonksiyonlarıyla VAO ve VBO objelerinin buffer'a yüklenmesi ile Z-Tamponu ayarlamalarının yapıldığı ana fonksiyon. 
int main(int argc,char** argv)
{
    srand(time(NULL));
    if(!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800,800,"3DSnakeGame",NULL,NULL);
    if(window==NULL)
    {
        std::cout<<"Pencere Olusturulamadi"<<std::endl;

        glfwTerminate();

        return -1;
    }    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    textureManager->loadTexture(textureNames[0]);
    textureManager->loadTexture(textureNames[1]);
    textureManager->loadTexture(textureNames[2]);
    textureManager->loadTexture(textureNames[3]);
    textureManager->loadTexture(textureNames[4]);

    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();


    ShaderProgram program;

    program.attachShader("./shaders/simplevs.glsl",GL_VERTEX_SHADER);
    program.attachShader("./shaders/simplefs.glsl",GL_FRAGMENT_SHADER);
    program.link();

    program.addUniform("ourTexture");
    program.addUniform("uColor");
    program.addUniform("uMtxTransform");

    //vertex array object oluşturuluyor
    glGenVertexArrays(1, &VAO); 
    //vertex buffer object oluşturuluyor
    glGenBuffers(1,&VBO);

    glGenBuffers(1, &EBO);

    //vertex array object aktif edildi.
    glBindVertexArray(VAO);
    //vertex buffer nesnesi aktif edildi.
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //vertex buffer'a nokta bilgileri yollaniyor
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
    //gönderilen vertex'e ait özellikler etiketleniyor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0); 
    //kaplama bilgileri belirtiliyor
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3 * sizeof(float)));
       
    glEnableVertexAttribArray(1);   

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW); 

    glEnable(GL_DEPTH_TEST);    //Z-tamponu için derinlik testi yapılıyor
   
    while(!glfwWindowShouldClose(window))
    {
        //oluşturulacak resim başlangıç rengine boyanıyor
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        //çerçeve tamponu ve z-tamponu temizleniyor
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        program.use();
        glBindVertexArray(VAO);

        Ciz(program);
        
        moveSnake();

        std::this_thread::sleep_for (std::chrono::milliseconds(120));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

}