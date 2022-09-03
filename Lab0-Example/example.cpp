#define GL_SILENCE_DEPRECATION //gets rid of some warnings. 

#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glut.H>

#include <glm/glm.hpp>

using namespace std;

/* This bit of trickery is necessary. MyAppWindow is actually defined below. 
   See note on callback functions in MyAppWindow for more info. 
*/
class MyAppWindow;
MyAppWindow *win;

/* The canvas class inherits from FLTK and is the part of the window where the
   drawing actually happens. Note it inherits from "Fl_Gl_Window" -- so we're
   interfacing with OpenGL to draw stuff.
*/
class MyGLCanvas : public Fl_Gl_Window {
public:
    int segments;
    bool wireframe;
    glm::vec3 rotVec;

    MyGLCanvas(int x, int y, int w, int h, 
               const char *l = 0) : Fl_Gl_Window(x, y, w, h, l){
        mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
        segments = 10;
        wireframe = false;
        rotVec.x = 0.0;
        rotVec.y = 0.0;
    };

private:
    /* This function will be called by redraw. It clears the window, 
       updates the rotation vector, loads the identity matrix into ModelView, 
       applies translation and rotation to the matrix, and then draws the torus,
       either in wireframe or solid mode, as specified. 
    */
    void draw(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Rotate the object every time it's drawn */
        rotVec.x += 3.3f;
        rotVec.y += 4.7f;
        
        /* Called when the GL canvas is set up for the first time. */
        if (!valid()) { establish_gl_context(); }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glTranslatef(0.0f, 0.0f, -1.0f); 
        
        glRotatef(rotVec.x, 0.0, 1.0, 0.0); 
        glRotatef(rotVec.y, 1.0, 0.0, 0.0);
        
        if (wireframe)
          glutWireTorus(.2, .5, 16, segments);
        else
          glutSolidTorus(.2, .5, 16, segments);
    };
  
    void establish_gl_context(){
        printf("establishing GL context\n");
        setup_opengl_camera();
        setup_opengl_lights();
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_FLAT);
    };
  
    void setup_opengl_camera(){
        glViewport(0, 0, pixel_w(), pixel_h());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float xy_aspect = (float)w() / (float)h();
        glFrustum(-xy_aspect * .125, xy_aspect * .125, -.125, .125, .1, 15.0);
    };
  
    void setup_opengl_lights(){
        GLfloat light_ambient[]   = { 0.1, 0.1, 0.2, 1.0 };
        GLfloat light_diffuse[]   = { 0.7, 0.7, 0.7, 1.0 };
        GLfloat light_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_position0[] = { 1.0, 1.0, 1.0, 0.0 };
	
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
        
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
    };

    /* When cursor enters the window, make it a cross, when it leaves, set it 
       back to normal. */
    int handle(int e) {
        switch (e) {
	case FL_ENTER: cursor(FL_CURSOR_CROSS); break;
	case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
	}
	return Fl_Gl_Window::handle(e);
    };

    /* When the window is resized, call the backend FLTK resize fn. */
    void resize(int x, int y, int w, int h) {
      Fl_Gl_Window::resize(x, y, w, h);
      printf("resize called\n");
    }; 
};

/*
The MyAppWindow class also inherits from FLTK, but this class is responsible for
the window itself. It contains an instance of MyGlCanvas, and defines the
various buttons, sliders, etc., as well as their callback functions. Note this
inherits from Fl_Window, not FL_Gl_Window as above. 
*/
class MyAppWindow : public Fl_Window {

public: 
    /* Constructor creates the window, composed of the canvas and sidebar (pack)
       with the segment slider and buttons. Then sets the window to be 
       resizable, and adds the idle callback function. 
    */
    MyAppWindow(int W, int H, const char*L = 0) : Fl_Window(W, H, L) {
        begin(); //this, and end, for FLTK
    
        canvas = new MyGLCanvas(10, 10, w()-110, h()-20);

        create_control_panel();
    
        end();
	
        resizable(this); //make the window resizable

        Fl::add_idle((Fl_Idle_Handler)(void*)idleCB); //add idle callback fn
    };

  ~MyAppWindow() { delete canvas; }; 

private:
    MyGLCanvas* canvas;
    Fl_Pack*    ctrl_panel;
    Fl_Slider*  segmentSlider;
    Fl_Slider*  widthSlider;
    Fl_Button*  wireButton;
  
   /* 
      Note that the callback (CB) functions are static. This is so they can be
      passed as args to be callbacks for FLTK. Because of this, they must not
      affect any member variables of the class. Thus, they affect the canvas 
      of the global variable win. (Yes, this is rather clunky, but given that
      the FLTK callback functions are free functions, it's just the way is has 
      to be)...
   */
    static void idleCB() {
        win->canvas->redraw();
    };
  
    static void segmentCB(Fl_Widget *w) {
      win->canvas->segments = ((Fl_Slider*)w)->value();
    };

    static void wireCB(Fl_Widget* w) {
        win->canvas->wireframe = ((Fl_Button*)w)->value();
    };

    void create_check_button() {
        wireButton = new Fl_Check_Button(0, 100,
                                         ctrl_panel->w()-20, 30, "Wireframe");
        wireButton->value(canvas->wireframe);
        wireButton->callback(wireCB);
    };

    void create_segment_slider() {
        segmentSlider = new Fl_Value_Slider(0, 0,
                                            ctrl_panel->w()-20, 30, "Segments");
        segmentSlider->align(FL_ALIGN_TOP);
        segmentSlider->type(FL_HOR_SLIDER);
        segmentSlider->bounds(3, 100);
        segmentSlider->step(1);
        segmentSlider->value(canvas->segments);
        segmentSlider->callback(segmentCB);
    };
  
    void create_control_panel(){
      ctrl_panel = new Fl_Pack(w()-100, 30, 100, h(), "Control Panel");
      ctrl_panel->box(FL_DOWN_FRAME);
      ctrl_panel->labelfont(1);
      ctrl_panel->type(Fl_Pack::VERTICAL);
      ctrl_panel->spacing(30);
      ctrl_panel->begin();
      
      create_check_button();
      create_segment_slider();
      
      ctrl_panel->end();
    };
  
};

int main() {
    win = new MyAppWindow(600, 500, "Lab 0");
    win->show();
    int status = Fl::run();
    return status;
};
