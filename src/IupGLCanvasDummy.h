/*
 * File:   IupGLCanvasDummy.h
 * Author: jeferson
 *
 * Created on August 31, 2014, 9:28 AM
 */

#ifndef IUPGLCANVASDUMMY_H
#define IUPGLCANVASDUMMY_H

#include "OpenGLMatrixManager.h"
#include "GraphicsShader.h"
#include <string>
#include <iup.h>
#include <vector>
#include <iostream>

class IupGLCanvasDummy
{
public:

    typedef std::pair< float, float> Point;

    struct Vertex {
        float x;
        float y;
        float z;
    };

    typedef struct Vertex Vertex;

    struct Face {
        int numberOfVertexes;
        int vertexOneIndex;
        int vertexTwoIndex;
        int vertexThreeIndex;
    };

    typedef struct Face Face;

    struct Normal {
        float x;
        float y;
        float z;
    };

    typedef struct Normal Normal;

    /**
     * Construtor default da classe.
     */
    IupGLCanvasDummy( );

    /**
     * Destrutor da classe.
     */
    virtual ~IupGLCanvasDummy( );

    /**
     * Exibe a janela.
     */
    void show( );

    /**
     * Oculta a janela.
     */
    void hide( );

    void readOFF( );

    void computeNormals( );

    void calculateTriangleNormal(int faceId, float &x, float &y, float &z);

    void computeCrossProduct( float x1, float y1, float z1,
                                               float x2, float y2, float z2,
                                               float &x, float &y, float &z );

    float calculateNorma(float x, float y, float z);

    std::vector< Vertex > _vertexes;
    std::vector< Face > _faces;
    std::vector< Normal > _vertexNormals;
    int _numVertex;
    int _numFaces;

    float* _coords;
    unsigned int* _triangleMesh;
    float* _normals;

private:

    /**
     * Ponteiro para o dialogo.
     */
    Ihandle *_dialog;

    /**
     * Matriz de projecao OpenGL
     */
    OpenGLMatrixManager _projectionMatrix;

    /**
     * Matriz de MODELVIEW OpenGL
     */
    OpenGLMatrixManager _modelViewMatrix;

    /**
     * A shader pointer.
     */
    GraphicsShader* _shader;

private:

    /**
     * Cria janela da IUP e define suas configuracoes e callbacks.
     */
    void createWindow( );

    /**
     * Incializa algumas propriedades do canvas OpenGL.
     */
    void initializeCanvas( );

    /**
     * Realiza o redesenho da cena OpenGL.
     */
    void drawScene( );

    /**
     * Trata evento de redimensionar o canvas OpenGL.
     * @param width - nova larguda do canvas.
     * @param height - nova altura do canvas.
     */
    void resizeCanvas( int width, int height );

    /**
     * Le arquivo de shader.
     * @param name - path do arquivo.
     * @return - string com o arquivo.
     */
    std::string readFile( const char* name );

private:
    /**
     * Callback do botao de fechar a janela.
     * @param button - ihandle do botao de sair.
     * @return - retorna IUP_CLOSE para que a janela seja fechada.
     */
    static int exitButtonCallback( Ihandle *button );


    static int zoomModeButtonCallback( Ihandle* button );

    /**
     * Callback responsavel por receber evento de redesenho do canvas.
     * @param canvas - ponteiro para o canvas.
     * @return  - IUP_DEFAULT.
     */
    static int actionCanvasCallback( Ihandle *canvas );

    /**
     * Callback responsavel por receber eventos do whell do mouse no canvas para
     * realizar a operacao de Zoom.
     * @param canvas - ponteiro para o canvas.
     * @param delta - vale -1 ou 1 e indica a direcao da rotacao do botao whell.
     * @param x - posicao x do mouse na tela.
     * @param y - posicao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT.
     */
    static int wheelCanvasCallback( Ihandle *canvas, float delta, int x,
                                    int y, char *status );

    static int keyCanvasCallback( Ihandle* canvas, int key, int press);
    /**
     * Callback responsavel por receber eventos de resize do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param width - nova largura, em pixeis, da janela.
     * @param heigth - nova altura, em pixeis, da janela.
     * @return - IUP_DEFAULT.
     */
    static int resizeCanvasCallback( Ihandle *canvas, int width, int height );

    /**
     * Callback responsavel por receber eventos de teclado do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param button - identificador do botao, podem ser BUTTON_1, BUTTON_2, ...
     * @param pressed - 1 para o caso do botao esta sendo pressionado e 0 caso
     * contrario.
     * @param x - posicao x do mouse na tela.
     * @param y - posiao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT.
     */
    static int buttonCanvasCallback( Ihandle* canvas, int button, int pressed,
                                     int x, int y, char* status );

};

#endif /* IUPGLCANVASDUMMY_H */

