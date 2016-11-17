/*
 * File:   IupGLCanvasDummy.cpp
 * Author: jeferson
 *
 * Created on August 31, 2014, 9:28 AM
 */

#include "IupGLCanvasDummy.h"

#include <cstdlib>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iup.h>
#include <iupgl.h>
#include <iosfwd>
#include <string>
#include <fstream>
#include <math.h>

float zoomX = 1.0;
float zoomY = 1.0;
float zoomZ = 1.0;

int zoomMode = 0;

//float eyeX = 0.0;
//float eyeY = 4.0;
//float eyeZ = 15.0;

float eyeX = 0.0;
float eyeY = 0.0;
float eyeZ = 0.0;

float rotateX = 0.0;
float rotateY = 0.0;
float rotateZ = 0.0;
float angle = 5.0;

float ySum = 0.0;
float zSum = 0.0;

float xmin = 100000.0;
float xmax = -1000000.0;
float ymin = 100000.0;
float ymax= -1000000.0;
float zmin = 100000.0;
float zmax= -1000000.0;

IupGLCanvasDummy::IupGLCanvasDummy( )
{
    //Cria janela e define suas configuracoes.
    readOFF();
    computeNormals();

    createWindow( );
}

void IupGLCanvasDummy::createWindow( )
{
    //Cria botao de sair.
    Ihandle *exitButton = IupButton( "Sair", NULL );

    //Cria botão de zoom mode
    Ihandle *zoomModeButton = IupButton( "ZoomMode", NULL );

    //Cria canvas.
    Ihandle *canvas = IupGLCanvas( NULL );

    //Cria composicao para o botao de sair.
    Ihandle *hboxButton = IupHbox( IupFill( ), exitButton, NULL );

    //Cria composicao para o botao de zoom mode.
    Ihandle *hboxButton2 = IupHbox( IupFill( ), zoomModeButton, NULL );

    //Cria composicao final.
    Ihandle *vboxFinal = IupVbox( canvas, hboxButton, hboxButton2, NULL );

    //Cria dialogo.
    _dialog = IupDialog( vboxFinal );

    //Define os atributos do botao de sair
    IupSetAttribute( exitButton, IUP_RASTERSIZE, "80x32" );
    IupSetAttribute( exitButton, IUP_TIP, "Fecha a janela." );

    //Define os atributos do botão de zoom mode
    IupSetAttribute( zoomModeButton, IUP_RASTERSIZE, "80x32" );
    IupSetAttribute( zoomModeButton, IUP_TIP, "Muda o modo do zoom" );

    //Define os atributos do canvas.
    IupSetAttribute( canvas, IUP_RASTERSIZE, "600x600" );
    IupSetAttribute( canvas, IUP_BUFFER, IUP_DOUBLE );
    IupSetAttribute( canvas, IUP_EXPAND, IUP_YES );

    //Define propriedades do dialogo.
    IupSetAttribute( _dialog, IUP_MARGIN, "10x10" );
    IupSetAttribute( _dialog, IUP_TITLE, "OpenGL + Canvas Dummy" );
    IupSetAttribute( _dialog, "THIS", ( char* ) this );

    //Define callbacks do botao de sair.
    IupSetCallback( exitButton, IUP_ACTION, ( Icallback ) exitButtonCallback );

    //Define callbacks do botao.
    IupSetCallback( zoomModeButton, IUP_ACTION, ( Icallback ) zoomModeButtonCallback );

    //Define as callbacks do canvas.
    IupSetCallback( canvas, IUP_ACTION, ( Icallback ) actionCanvasCallback );
    IupSetCallback( canvas, IUP_RESIZE_CB, ( Icallback ) resizeCanvasCallback );
    IupSetCallback( canvas, IUP_BUTTON_CB, ( Icallback ) buttonCanvasCallback );
    IupSetCallback( canvas, IUP_WHEEL_CB, ( Icallback ) wheelCanvasCallback );

    IupSetCallback( canvas, IUP_KEYPRESS_CB, ( Icallback ) keyCanvasCallback );

    //Mapeia o dialogo.
    IupMap( _dialog );

    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Incialia propriedades dos canvas.
    initializeCanvas( );
}



IupGLCanvasDummy::~IupGLCanvasDummy( )
{
    IupDestroy( _dialog );
}



void IupGLCanvasDummy::show( )
{
    IupShow( _dialog );
}



void IupGLCanvasDummy::hide( )
{
    IupHide( _dialog );
}



std::string IupGLCanvasDummy::readFile( const char* name )
{
    std::ifstream in( name );
    std::string shader;

    if (in.fail( ))
    {
        return "";
    }

    char a;
    while (in.get( a ) && a != EOF)
    {
        shader += a;
    }
    shader += '\0';
    return shader;
}



void IupGLCanvasDummy::initializeCanvas( )
{
    glClearColor( 0.4, 0.4, 0.4, 1.0 );
    glEnable( GL_DEPTH_TEST );
    //std::cout << "entrei na initializeCanvas" << std::endl;
    //std::string fragS = "#version 410 \n in vec4 v_Color; \n out vec4 fragmentColor; \n void main(void) { fragmentColor = v_Color; } \n";
    //std::string vertexS = "#version 410 \n uniform mat4 mvp; \n uniform mat4 mvm; \n uniform vec3 light; \n in vec4 vtx; \n in vec3 normal; \n out vec4 v_Color; \n void main(void){ vec3 modelViewVertex = vec3(mvm * vtx); vec3 modelViewNormal = vec3(mvm * vec4(normal, 0.0)); float distance = length(light - modelViewVertex); vec3 lightVector = normalize(light - modelViewVertex); float diffuse = max(dot(modelViewNormal, light), 0.1); v_Color = vec4(1.0, 0.0, 0.0, 1.0) * diffuse; gl_Position = mvp * vtx; } \n";

    //Aloca shader.
    _shader = new GraphicsShader( );

    std::string vertexShader = readFile( "perFrag.vert" );
    _shader->setVertexProgram( vertexShader.c_str( ), vertexShader.size( ) );

    std::string fragmentShader = readFile( "perFrag.frag" );
    _shader->setFragmentProgram( fragmentShader.c_str( ), fragmentShader.size( ) );
}



void IupGLCanvasDummy::drawScene( )
{
    //std::cout << "entrei na drawScene" << std::endl;

    //Limpa a janela.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    GLenum err = glGetError( );

    if (err != GL_NO_ERROR)
    {
        printf( "Error : %s\n", gluErrorString( err ) );
    }

    std::vector<float> coordsVec;
    for(int i = 0; i < _numVertex; i++)
    {
        coordsVec.push_back(_vertexes[i].x);
        coordsVec.push_back(_vertexes[i].y);
        coordsVec.push_back(_vertexes[i].z);
    }
    _coords = &coordsVec[0];

    std::vector< unsigned int > triangleMeshVec;
    for(int i = 0; i < _numFaces; i++)
    {
        triangleMeshVec.push_back(_faces[i].vertexOneIndex);
        triangleMeshVec.push_back(_faces[i].vertexTwoIndex);
        triangleMeshVec.push_back(_faces[i].vertexThreeIndex);
    }
     _triangleMesh = &triangleMeshVec[0];

    std::vector< float > normalsVec;
    for(int i =0 ; i < _vertexNormals.size(); i++)
    {
        normalsVec.push_back(_vertexNormals[i].x);
        normalsVec.push_back(_vertexNormals[i].y);
        normalsVec.push_back(_vertexNormals[i].z);
    }
     _normals = &normalsVec[0];

     std::vector< float > lightsVec;

     lightsVec.push_back((xmax-xmin)/2);
     lightsVec.push_back((ymax-ymin)/2);
     lightsVec.push_back(zmax-zmin);
     lightsVec.push_back((xmax-xmin)/2);
     lightsVec.push_back((ymax-ymin)/2);
     lightsVec.push_back(-(zmax-zmin));

     lightsVec.push_back((xmax-xmin)/2);
     lightsVec.push_back(ymax-ymin);
     lightsVec.push_back((zmax-zmin)/2);
     lightsVec.push_back((xmax-xmin)/2);
     lightsVec.push_back(-(ymax-ymin));
     lightsVec.push_back((zmax-zmin)/2);

     lightsVec.push_back((xmax-xmin));
     lightsVec.push_back((ymax-ymin)/2);
     lightsVec.push_back((zmax-zmin)/2);
     lightsVec.push_back(-(xmax-xmin));
     lightsVec.push_back((ymax-ymin)/2);
     lightsVec.push_back((zmax-zmin)/2);

/*
     lightsVec.push_back(3*(xmax-xmin));
     lightsVec.push_back(0.0);
     lightsVec.push_back(0.0);

     lightsVec.push_back(-3*(xmax-xmin));
     lightsVec.push_back(0.0);
     lightsVec.push_back(0.0);
*/
    float* lights = &lightsVec[0];

     eyeX = 3*(xmax-xmin);
    //Aplica uma transformacao de escala.
    _modelViewMatrix.loadIdentity();
    _modelViewMatrix.lookAt( eyeX, 0.0, 0.0, (xmax-xmin)/2.0, (ymax-xmin)/2.0, (zmax-zmin)/2.0, 0.0, 1.0, 0.0 );

    _modelViewMatrix.scale(zoomX, zoomY, zoomZ);

    //_modelViewMatrix.translate( 0.0, ySum, zSum);
    _modelViewMatrix.rotate( angle, rotateX, rotateY, rotateZ );
    //_modelViewMatrix.translate( 0.0, -ySum, -zSum);

    //compila o shader se este nao tiver sido compilado ainda
    if (!_shader->isAllocated( ))
        _shader->compileShader( );

    //Carrega o programa na placa.
    _shader->load( );

    unsigned int glShader = _shader->getShaderIndex( );

    //Transfere os vertices para a placa.
    int vertexParam = glGetAttribLocation( glShader, "vtx" );
    glVertexAttribPointer( vertexParam, 3, GL_FLOAT, GL_FALSE, 0, _coords );
    glEnableVertexAttribArray( vertexParam );

    //Transfere os normais dos vertices para a placa.
    int normalParam = glGetAttribLocation( glShader, "normal" );
    glVertexAttribPointer( normalParam, 3, GL_FLOAT, GL_FALSE, 0, _normals );
    glEnableVertexAttribArray( normalParam );

    //Let there be Light
    int lightParam = glGetAttribLocation( glShader, "light" );
    glUniform3fv( lightParam, 6, lights );

    //Let there be Eye
    int eyeParam = glGetAttribLocation( glShader, "eye" );
    glUniform4f( eyeParam, eyeX, eyeY, eyeZ, 1.0f );

    //Obtem a modelview projection (mvp)
    {
        _projectionMatrix.push( );
        _modelViewMatrix.push( );

        //Transfere a matriz para a placa.
        int matrixParamMVM = glGetUniformLocation( glShader, "mvm" );
        glUniformMatrix4fv( matrixParamMVM, 1, GL_FALSE, ( float* ) _modelViewMatrix );

        //Multiplica a modelview pela projection.
        _projectionMatrix.multMatrix( ( float* ) _modelViewMatrix );

        //Transfere a matriz para a placa.
        int matrixParamMVP = glGetUniformLocation( glShader, "mvp" );
        glUniformMatrix4fv( matrixParamMVP, 1, GL_FALSE, ( float* ) _projectionMatrix );

        _projectionMatrix.pop( );
        _modelViewMatrix.pop( );
    }

    //Desenha os elementos.
    glDrawElements( GL_TRIANGLES, _numFaces, GL_UNSIGNED_INT, _triangleMesh );

    //Descarrega o programa da placa.
    _shader->unload( );
}



void IupGLCanvasDummy::resizeCanvas( int width, int height )
{
    //Define o viewport.
    glViewport( 0, 0, width, height );
   // std::cout << "entrei na resizeCanvas" << std::endl;
    _projectionMatrix.loadIdentity( );
    //_projectionMatrix.ortho( -10, 10, -10, 10, 0, 20);
    //_projectionMatrix.frustum( xmin, xmax, ymin, ymax, zmin, zmax );
    _projectionMatrix.perspective( 60, (float)width / (float)height, 1, 1000);
}



int IupGLCanvasDummy::exitButtonCallback( Ihandle* button )
{
    return IUP_CLOSE;
}

int IupGLCanvasDummy::zoomModeButtonCallback( Ihandle* button )
{
    if (zoomMode == 0)
        zoomMode = 1;
    else
        zoomMode = 0;

    return IUP_DEFAULT;
}

int IupGLCanvasDummy::actionCanvasCallback( Ihandle* canvas )
{
    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->drawScene( );

    //Troca os buffers.
    IupGLSwapBuffers( canvas );

    return IUP_DEFAULT;
}



int IupGLCanvasDummy::resizeCanvasCallback( Ihandle *canvas, int width, int height )
{
    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->resizeCanvas( width, height );

    //Marca o canvas para ser redesenhado.
    IupUpdate( canvas );

    return IUP_DEFAULT;
}



int IupGLCanvasDummy::buttonCanvasCallback( Ihandle* canvas, int button, int pressed,
                                            int x, int y, char* status )
{
    return IUP_DEFAULT;
}



int IupGLCanvasDummy::wheelCanvasCallback( Ihandle* canvas, float delta, int x,
                                           int y, char* status )
{
    if(zoomMode == 0)
    {
        zoomX += 1.0;
        zoomY += 1.0;
        zoomZ += 1.0;
    }
    else if(zoomMode == 1 && zoomX > 0.0 && zoomY > 1.0 && zoomZ > 0.0)
    {
        zoomX -= 1.0;
        zoomY -= 1.0;
        zoomZ -= 1.0;
    }

    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->drawScene( );

    //Troca os buffers.
    IupGLSwapBuffers( canvas );

    return IUP_DEFAULT;
}

int IupGLCanvasDummy::keyCanvasCallback( Ihandle* canvas, int key, int press)
{
    //key: identifier of typed key. Please refer to the Keyboard Codes table for a list of possible values.
    //press: 1 is the user pressed the key or 0 otherwise.

    if(key == K_g)
    {
        rotateX = 0.0;
        rotateY = 1.0;
        rotateZ = 0.0;
        angle += 5.0;
    }

    else if(key == K_h)
    {
        rotateX = 0.0;
        rotateY = 1.0;
        rotateZ = 0.0;
        angle -= 5.0;
    }

    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->drawScene( );

    //Troca os buffers.
    IupGLSwapBuffers( canvas );

    return IUP_DEFAULT;
}

void IupGLCanvasDummy::readOFF()
{
    int lineCounter = 0;
    int wordCounter = 0;

    std::vector< std::string > vertexFaceSplit;
    std::vector< std::string > numVertexFaceSplit;

    //Open file model.off
    std::ifstream file ("apple.off");

    std::string line;
    std::string delimiter = " ";
    std::string token;
    size_t pos = 0;

    if(file.is_open())
    {

        while(getline (file, line))
        {
            while ((pos = line.find(delimiter)))
            {
                //se é a ultima palavra (:
                if(pos == std::string::npos)
                {
                    token = line;
                    vertexFaceSplit.push_back(token);
                    wordCounter++;
                    break;
                }

                token = line.substr(0, pos);

                line.erase(0, pos + delimiter.length());


                if(lineCounter == 1)
                {
                    if(wordCounter == 0)
                        _numVertex = std::atoi(token.c_str());
                    else if (wordCounter == 1)
                        _numFaces = std::atoi(token.c_str());
                }

                vertexFaceSplit.push_back(token);

                wordCounter++;
            }

            if(wordCounter == 3 && lineCounter != 1)
            {
                Vertex iVertex;
                iVertex.x = std::atof(vertexFaceSplit[0].c_str());
                iVertex.y = std::atof(vertexFaceSplit[1].c_str());
                iVertex.z = std::atof(vertexFaceSplit[2].c_str());

                ySum += iVertex.y;
                zSum += iVertex.z;

                if(iVertex.x < xmin)
                    xmin = iVertex.x;
                if(iVertex.x > xmax )
                    xmax = iVertex.x;
                if(iVertex.y < ymin)
                    ymin = iVertex.y;
                if(iVertex.y > ymax )
                    ymax = iVertex.y;
                if(iVertex.z < zmin)
                    zmin = iVertex.z;
                if(iVertex.z > zmax )
                    zmax = iVertex.z;

               _vertexes.push_back(iVertex);
            }


            if(wordCounter == 4)
            {
                Face iFace;

                iFace.numberOfVertexes = std::atoi(vertexFaceSplit[0].c_str());
                iFace.vertexOneIndex = std::atoi(vertexFaceSplit[1].c_str());
                iFace.vertexTwoIndex = std::atoi(vertexFaceSplit[2].c_str());
                iFace.vertexThreeIndex = std::atoi(vertexFaceSplit[3].c_str());

                _faces.push_back(iFace);
            }

            lineCounter++;
            wordCounter = 0;
            vertexFaceSplit.clear();
        }

        /*
        for(unsigned int i= 0; i < _faces.size() ; i++)
        {
            std::cout << _faces[i].vertexOneIndex << " " << _faces[i].vertexTwoIndex << " " <<_faces[i].vertexThreeIndex << std::endl;
        }
        */

        file.close();
    }
    else
    {
        printf("oi");
    }


}

void IupGLCanvasDummy::computeNormals()
{

    for(unsigned i = 0; i < _vertexes.size(); i++)
    {
        Normal iNormal;
        float norma;
        iNormal.x = 0.0;
        iNormal.y = 0.0;
        iNormal.z = 0.0;

        for(unsigned int j = 0; j < _faces.size(); j++ )
        {
            if( _faces[j].vertexOneIndex == i || _faces[j].vertexTwoIndex == i || _faces[j].vertexThreeIndex == i )
            {
                float x;
                float y;
                float z;

                calculateTriangleNormal(j, x, y, z);
                iNormal.x += x;
                iNormal.y += y;
                iNormal.z += z;
            }
        }

        norma = calculateNorma(iNormal.x, iNormal.y, iNormal.z);
        iNormal.x = iNormal.x / norma;
        iNormal.y = iNormal.y / norma;
        iNormal.z = iNormal.z / norma;

        _vertexNormals.push_back(iNormal);
    }

}

void IupGLCanvasDummy::calculateTriangleNormal(int faceId, float &x, float &y, float &z)
{
    //Apenas organiza bonitinho os vértices do trianglo
    int first = _faces[faceId].vertexOneIndex;
    int second = _faces[faceId].vertexTwoIndex;
    int third = _faces[faceId].vertexThreeIndex;

    Vertex A;
    A.x = _vertexes[first].x;
    A.y = _vertexes[first].y;
    A.z = _vertexes[first].z;

    Vertex B;
    B.x = _vertexes[second].x;
    B.y = _vertexes[second].y;
    B.z = _vertexes[second].z;

    Vertex C;
    C.x = _vertexes[third].x;
    C.y = _vertexes[third].y;
    C.z = _vertexes[third].z;

    //Usando o struct Vertex por conveniencia, mas é um vetor :3
    Vertex BA;
    BA.x = B.x - A.x;
    BA.y = B.y - A.y;
    BA.z = B.z - A.z;

    Vertex CA;
    CA.x = C.x - A.x;
    CA.y = C.y - A.y;
    CA.z = C.z - A.z;

    float normalx;
    float normaly;
    float normalz;

    computeCrossProduct( BA.x, BA.y, BA.z, CA.x, CA.y, CA.z, normalx, normaly , normalz );

    x = normalz;
    y = normaly;
    z = normalz;
}

void IupGLCanvasDummy::computeCrossProduct( float x1, float y1, float z1,float x2, float y2, float z2, float &x, float &y, float &z )
{
    x = y1 * z2 - z1 * y2;
    y = z1 * x2 - x1 * z2;
    z = x1 * y2 - y1 * x2;
}

float IupGLCanvasDummy::calculateNorma(float x, float y, float z)
{
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float norma;

    norma = sqrt(x2 + y2 + z2);

    return norma;
}