/*
********************************************************************************
Vectron - map editor for Armagetron Advanced.
Copyright (C) 2014  Tristan Whitcher    (tristan.whitcher@gmail.com)
                    David Dubois        (ddubois@jotunstudios.com)
********************************************************************************

This file is part of Vectron.

Vectron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Vectron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Vectron.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "stdafx.h"

#include "Grid.h"
#include "Input.h"
#include "Screen.h"
#include "Zone.h"

using namespace std;

void error_callback( int error, const char *description );
void key_callback( GLFWwindow *window, int key, int scancode, int action, 
    int mods );

void addZone();
void drawCursor();


forward_list<Zone*> zones;

int main(void) {

    Screen s = Screen(640, 480);

    glfwSetErrorCallback(error_callback);


    zones.push_front( new Zone( 4, 4, 1 ) );

    Grid *g = new Grid();


    glfwSetKeyCallback(s.window, key_callback);

    glfwSetFramebufferSizeCallback(s.window, Screen::_framebuffer);
    glfwSetWindowSizeCallback(s.window, Screen::_size);

    /* Initial Before callback */

    glfwGetFramebufferSize(s.window, &Screen::pxWidth, &Screen::pxHeight);
    glViewport(0, 0, Screen::pxWidth, Screen::pxHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Screen::width, 0, Screen::height, 0, 1);


    glfwSetCursorPosCallback( s.window, Input::_mousePos );
    glfwSetMouseButtonCallback( s.window, Input::_mouseButton );
    glfwSetScrollCallback( s.window, Input::_scroll );

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(2.0f);

    while (!glfwWindowShouldClose(s.window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        g->draw( Screen::pxWidth, Screen::pxHeight );

        for( Zone *z : zones ) {
            z->draw();
        }
                
        Input::drawCursor();

        glfwSwapBuffers(s.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(s.window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* Where should we put error handling? */

void error_callback( int error, const char *description ) {
    fputs( description, stderr );
}

void key_callback( GLFWwindow *window, int key, int scancode, int action, 
    int mods ) {
    if( key == GLFW_KEY_Z && action == GLFW_RELEASE ) {
        zones.push_front( 
            new Zone( Input::mouseX/Grid::spacing, Input::mouseY/Grid::spacing, 1 ));
    } else if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, GL_TRUE );
    }
}
