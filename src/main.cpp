/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jcoelho
 *
 * Created on October 17, 2016, 9:38 AM
 */

#include <cstdlib>
#include <cstdio>
#include <iup.h>
#include <iupgl.h>

#include "IupGLCanvasDummy.h"

using namespace std;



/*
 * 
 */
int main( int argc, char** argv )
{
    //Inicializa a IUP.
    IupOpen( &argc, &argv );

    //Inicializa a OpenGL na IUP.
    IupGLCanvasOpen( );

    //Cria objeto.
    IupGLCanvasDummy *window = new IupGLCanvasDummy( );

    printf( "IUP: %s\n", IupVersion( ) );

    //Exibe a janela.
    window->show( );

    //Coloca a IUP em loop.
    IupMainLoop( );

    //Deleta o obejto alocado.
    delete window;

    //Fecha a IUP.
    IupClose( );
    
    return 0;
}

