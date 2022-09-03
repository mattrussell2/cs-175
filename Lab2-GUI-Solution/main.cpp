/*  =================== File Information =================
    File Name: main.cpp
    Description:
    Author: Michael Shah

    Purpose: Driver for 3D program to load .ply models
    Usage:
    ===================================================== */

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/names.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

#include "ply.h"

using namespace std;

class MyAppWindow;

MyAppWindow *win;


class MyGLCanvas : public Fl_Gl_Window {
public:
    int   wireframe, filled;
    int   rotX, rotY, rotZ;
    float posX, posY, posZ;
    float red, green, blue;
    /****************************************/
    /*         PLY Object                   */
    /****************************************/
    ply *myPLY = NULL;

    MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
    ~MyGLCanvas();

private:
    void draw();
    int  handle(int);
    void resize(int x, int y, int w, int h);
};

MyGLCanvas::MyGLCanvas(int x, int y, int w, int h, const char *l) :
    Fl_Gl_Window(x, y, w, h, l) {
    mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
    wireframe = 0;
    filled    = 1;
    rotX = rotY = rotZ = 0;
    posX               = 0.0f;
    posY               = 0.0f;
    posZ               = -0.75f;
    red = green = blue = 0.5f;
    myPLY              = new ply();
}

MyGLCanvas::~MyGLCanvas() {
    delete myPLY;
}

void MyGLCanvas::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!valid()) { // this is called when the GL canvas is set up for the first
                    // time...
        puts("establishing GL context");

        float xy_aspect;
        xy_aspect = (float)w() / (float)h();
        //
        glViewport(0, 0, w(), h());
        // Determine if we are modifying the camera(GL_PROJECITON) matrix(which
        // is our viewing volume) Otherwise we could modify the object
        // transormations in our world with GL_MODELVIEW
        glMatrixMode(GL_PROJECTION);
        // Reset the Projection matrix to an identity matrix
        glLoadIdentity();
        // The frustrum defines the perspective matrix and produces a
        // perspective projection. It works by multiplying the current matrix(in
        // this case, our matrix is the GL_PROJECTION) and multiplies it.
        glFrustum(-xy_aspect * .125, xy_aspect * .125, -.125, .125, .1, 15.0);

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glShadeModel(GL_FLAT);

        GLfloat light_pos0[] = {0.0f, 0.0f, 1.0f, 0.0f};
        GLfloat ambient[]    = {0.7f, 0.7f, 0.7f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        /****************************************/
        /*          Enable z-buferring          */
        /****************************************/

        glEnable(GL_DEPTH_TEST);
        glPolygonOffset(1, 1);
    }

    GLfloat diffuse[] = {red, green, blue, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);


    // Clear the buffer of colors in each bit plane.
    // bit plane - A set of bits that are on or off (Think of a black and white
    // image)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the mode so we are modifying our objects.
    glMatrixMode(GL_MODELVIEW);
    // Load the identify matrix which gives us a base for our object
    // transformations (i.e. this is the default state)
    glLoadIdentity();

    // here we are moving the camera back by 0.5 on the z-axis
    glTranslatef(posX, posY, posZ);

    // allow for user controlled rotation
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotZ, 0.0, 0.0, 1.0);

    // draw the axes
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(1.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0.0, 1.0, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1.0);
    glEnd();
    glPopMatrix();

    if (filled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glColor3f(0.6, 0.6, 0.6);
        glPolygonMode(GL_FRONT, GL_FILL);
        myPLY->render();
    }

    if (wireframe) {
        glDisable(GL_LIGHTING);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glColor3f(1.0, 1.0, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        myPLY->render();
    }
    // no need to call swap_buffer as it is automatically called
}

int MyGLCanvas::handle(int e) {
    // printf("Event was %s (%d)\n", fl_eventnames[e], e);
    switch (e) {
    case FL_ENTER: cursor(FL_CURSOR_HAND); break;
    case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
    case FL_KEYUP:
        printf("keyboard event: key pressed: %c\n", Fl::event_key());
        switch (Fl::event_key()) {
        case 'w': posY += 0.05; break;
        case 'a': posX -= 0.05f; break;
        case 's': posY -= 0.05; break;
        case 'd': posX += 0.05f; break;
        }
        break;
    case FL_MOUSEWHEEL:
        printf("mousewheel: dx: %d, dy: %d\n", Fl::event_dx(), Fl::event_dy());
        posZ += Fl::event_dy() * -0.05f;
        break;
    }

    return Fl_Gl_Window::handle(e);
}

void MyGLCanvas::resize(int x, int y, int w, int h) {
    Fl_Gl_Window::resize(x, y, w, h);
    puts("resize called");
}


class MyAppWindow : public Fl_Window {
public:
    // slider widgets for rotation
    Fl_Slider *rotXSlider;
    Fl_Slider *rotYSlider;
    Fl_Slider *rotZSlider;

    // slider widgets for color
    Fl_Slider *redSlider;
    Fl_Slider *greenSlider;
    Fl_Slider *blueSlider;

    Fl_Button  *wireButton;
    Fl_Button  *fillButton;
    Fl_Button  *openFileButton;
    MyGLCanvas *canvas;

public:
    // APP WINDOW CONSTRUCTOR
    MyAppWindow(int W, int H, const char *L = 0);

    static void idleCB(void *userdata) { win->canvas->redraw(); }

private:
    // Someone changed one of the sliders
    static void rotateCB(Fl_Widget *w, void *userdata) {
        int value          = ((Fl_Slider *)w)->value();
        *((int *)userdata) = value;
    }

    static void colorCB(Fl_Widget *w, void *userdata) {
        float value          = ((Fl_Slider *)w)->value();
        *((float *)userdata) = value;
    }

    static void wireCB(Fl_Widget *w, void *userdata) {
        win->canvas->wireframe = ((Fl_Button *)w)->value();
    }

    static void fillCB(Fl_Widget *w, void *userdata) {
        win->canvas->filled = ((Fl_Button *)w)->value();
    }

    static void loadFileCB(Fl_Widget *w, void *data) {
        Fl_File_Chooser G_chooser("", "", Fl_File_Chooser::MULTI, "");
        G_chooser.show();
        // Block until user picks something.
        //     (The other way to do this is to use a callback())
        //
        G_chooser.directory("./data");
        while (G_chooser.shown()) {
            Fl::wait();
        }

        // Print the results
        if (G_chooser.value() == NULL) {
            printf("User cancelled file chooser\n");
            return;
        }

        cout << "Loading new ply file from: " << G_chooser.value() << endl;
        // Reload our model
        win->canvas->myPLY->reload(G_chooser.value());
        // Print out the attributes
        win->canvas->myPLY->printAttributes();

        win->canvas->redraw();
    }
};


MyAppWindow::MyAppWindow(int W, int H, const char *L) : Fl_Window(W, H, L) {
    begin();
    // OpenGL window

    canvas = new MyGLCanvas(10, 10, w() - 110, h() - 20);

    Fl_Pack *overallPack = new Fl_Pack(w() - 100, 30, 100, h(), "");
    overallPack->box(FL_DOWN_FRAME);
    overallPack->labelfont(1);
    overallPack->type(Fl_Pack::VERTICAL);
    overallPack->spacing(30);
    overallPack->begin();


    Fl_Pack *pack = new Fl_Pack(w() - 100, 30, 100, h(), "Control Panel");
    pack->box(FL_DOWN_FRAME);
    pack->labelfont(1);
    pack->type(Fl_Pack::VERTICAL);
    pack->spacing(0);
    pack->begin();

    openFileButton = new Fl_Button(0, 100, pack->w() - 20, 20, "Load File");
    openFileButton->callback(loadFileCB, (void *)this);

    wireButton = new Fl_Check_Button(0, 100, pack->w() - 20, 20, "Wireframe");
    wireButton->callback(wireCB, (void *)this);
    wireButton->value(canvas->wireframe);

    fillButton = new Fl_Check_Button(0, 100, pack->w() - 20, 20, "Fill");
    fillButton->callback(fillCB, (void *)this);
    fillButton->value(canvas->filled);

    // slider for controlling rotation
    Fl_Box *rotXTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateX");
    rotXSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    rotXSlider->align(FL_ALIGN_TOP);
    rotXSlider->type(FL_HOR_SLIDER);
    rotXSlider->bounds(-359, 359);
    rotXSlider->step(1);
    rotXSlider->value(canvas->rotX);
    rotXSlider->callback(rotateCB, (void *)(&(canvas->rotX)));

    Fl_Box *rotYTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateY");
    rotYSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    rotYSlider->align(FL_ALIGN_TOP);
    rotYSlider->type(FL_HOR_SLIDER);
    rotYSlider->bounds(-359, 359);
    rotYSlider->step(1);
    rotYSlider->value(canvas->rotY);
    rotYSlider->callback(rotateCB, (void *)(&(canvas->rotY)));

    Fl_Box *rotZTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateZ");
    rotZSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    rotZSlider->align(FL_ALIGN_TOP);
    rotZSlider->type(FL_HOR_SLIDER);
    rotZSlider->bounds(-359, 359);
    rotZSlider->step(1);
    rotZSlider->value(canvas->rotZ);
    rotZSlider->callback(rotateCB, (void *)(&(canvas->rotZ)));

    pack->end();


    Fl_Pack *colorPack = new Fl_Pack(w() - 100, 30, 100, h(), "Color Panel");
    colorPack->box(FL_DOWN_FRAME);
    colorPack->labelfont(1);
    colorPack->type(Fl_Pack::VERTICAL);
    colorPack->spacing(0);
    colorPack->begin();
    // color control
    Fl_Box *redTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "Red");
    redSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    redSlider->align(FL_ALIGN_TOP);
    redSlider->type(FL_HOR_SLIDER);
    redSlider->bounds(0, 1);
    redSlider->value(canvas->red);
    redSlider->callback(colorCB, (void *)(&(canvas->red)));

    Fl_Box *greenTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "Green");
    greenSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    greenSlider->align(FL_ALIGN_TOP);
    greenSlider->type(FL_HOR_SLIDER);
    greenSlider->bounds(0, 1);
    greenSlider->value(canvas->green);
    greenSlider->callback(colorCB, (void *)(&(canvas->green)));

    Fl_Box *blueTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "Blue");
    blueSlider          = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
    blueSlider->align(FL_ALIGN_TOP);
    blueSlider->type(FL_HOR_SLIDER);
    blueSlider->bounds(0, 1);
    blueSlider->value(canvas->blue);
    blueSlider->callback(colorCB, (void *)(&(canvas->blue)));


    colorPack->end();
    overallPack->end();

    end();
}


/**************************************** main() ********************/
int main(int argc, char **argv) {
    win = new MyAppWindow(600, 500, "User Interface");
    win->resizable(win);
    Fl::add_idle(MyAppWindow::idleCB);
    win->show();
    return (Fl::run());
}