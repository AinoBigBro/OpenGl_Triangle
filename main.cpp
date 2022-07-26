#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>


class render{
private:
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;

    unsigned int shaderProgram;
    unsigned int vao;

    unsigned int shader(){
        const char* vertexShaderSource="#version 330 core\n"
            "layout (location=0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
            "}\0";
        const char* fragmentShaderSource="#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor=vec4(1.0f,0.4f,0.4f,1.0f);"
            "}\0";
        
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
        glCompileShader(vertexShader);


        unsigned int fragmentShader;
        fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
        glCompileShader(fragmentShader);

        unsigned int ret = glCreateProgram();

        glAttachShader(ret,vertexShader);
        glAttachShader(ret, fragmentShader);
        glLinkProgram(ret);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return ret;
    }

    unsigned int vertexBufInit(){
        static const GLfloat vertex[]={
            -0.5f, -0.5f, 0.4f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
        };


        unsigned int buf;
        unsigned int arr;

        glGenVertexArrays(1,&arr);
        glGenBuffers(1,&buf);

        glBindVertexArray(arr);
        glBindBuffer(GL_ARRAY_BUFFER, buf);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex),vertex, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  
        return arr;
    }


public:
    render(int x,int y){
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

        window=SDL_CreateWindow("boop",100,100,800,600,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        context=SDL_GL_CreateContext(window);

        gladLoadGLLoader(SDL_GL_GetProcAddress);
    }

    void init(){
        shaderProgram=shader();
        vao=vertexBufInit();
    }
    
    int draw(){

        SDL_PollEvent(&event);
        if(event.type==SDL_QUIT){
            return 1;
        }

        glClearColor(0.3f,0.0f,0.3f,1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);

        SDL_GL_SwapWindow(window);
        return 0;
    }
    

};


int main(int argc, char* argv[]){
    render b=render(800,600);
    b.init();
    while(b.draw()!=1);
    SDL_Quit();
    return 0;
}

//g++ -std=c++17 main.cpp glad/src/glad.c -I ./glad/include -lmingw32 -lsdl2main -lsdl2
