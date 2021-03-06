/*
 Tobby Lie
 Intro to Graphics HW 3
 2/27/20
*/

#define GL_SILENCE_DEPRECATION // silence deprecation warnings

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

static float elevation, swing = 0.0; // variables for elevation and swing
static float prev_mousex, prev_mousey = 0.0; // variables for last position of mouse coordinates before motion registered
static bool dragging = false; // flag for dragging mouse if left mouse button pressed down
static bool grid_toggle = true; // flag for grid toggle
static const float z_distance = 10.0; // z distance away from origin or teapot
std::vector<float> up_vector = {0, 1, 0}; // up vector for glulookat

#define M_PI   3.14159265358979323846264338327950288 // constant for pi

/*
    Function: frame_buffer_coordinates
 
    Description: From the viewport, gets the frame buffer
    width and height.
 
    Parameters: None
 
    Pre-Conditions: None
 
    Post-Conditions: None
 
    Returns: Tuple of GLint, GLint. The first value being
    frame buffer width, second being frame buffer height.
 */
std::tuple<GLint, GLint> frame_buffer_coordinates()
{
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint fbWidth = dims[2];
    GLint fbHeight = dims[3];
    
    return std::make_tuple(fbWidth, fbHeight);
} // end frame_buffer_coordinates

/*
   Function: init

   Description: Initialization function

   Parameters: None

   Pre-Conditions: None

   Post-Conditions: Settings for program are intizialized

   Returns: Nothing
*/
void init(void)
{
    GLfloat light_ambient[] =
    {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] =
    {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] =
    {1.0, 1.0, 1.0, 1.0};
    
    /* light_position is NOT default value */
    GLfloat light_position[] =
    {1.0, 1.0, 0.0, 0.0};
    GLfloat global_ambient[] =
    {0.75, 0.75, 0.75, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    
    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
} // end init
 
/*
    Function: display
 
    Description: Based on global values, display certain
    information to the window.
 
    Parameters: None
 
    Pre-Conditions: Different value should carry information as
        to how much objects need to be rotated.
 
    Post-Conditions: Display on window is updated.
 
    Returns: Nothing
*/
void display(void)
{
    GLfloat more_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f}; // ambience array
    
    glEnable(GL_COLOR_MATERIAL); // change color fo teapot
    glColor3f(0.196078f, 0.6f, 0.8f); // SkyBlue

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

    /*  material has moderate ambient reflection */
    glMaterialfv(GL_FRONT, GL_AMBIENT, more_ambient);
    
    glLoadIdentity();
    
    // Calculate the camera position using the distance and angles
    float look_x = z_distance * -sinf(swing*(M_PI / 180)) * cosf((elevation)*(M_PI / 180));
    float look_y = z_distance * -sinf((elevation)*(M_PI / 180));
    float look_z = -z_distance * cosf((swing)*(M_PI / 180)) * cosf((elevation)*(M_PI / 180));
    
    // if absolute value of elevation within bounds of [90, 270) flip the up vector
    if (abs(elevation) >= 90 && abs(elevation) < 270)
    {
        up_vector = {0, -1, 0};
    }
    else
        up_vector = {0, 1, 0};
    
    gluLookAt(-look_x, -look_y, look_z, 0, 0, 0, up_vector[0], up_vector[1], up_vector[2]); // set gluLookAt up vecor to look_vector_cross
    
    glutSolidTeapot(1.0f); // display teapot
    
    if (grid_toggle)
    {
        glLineWidth(6.0f); // thicker lines for x, y, z axis lines
        glBegin(GL_LINES);
        
        /* draw lines for the x, y, z axis */
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 2.0f, 0.0f);
        glVertex3f(0.0f, -2.0f, 0.0f);
        
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(2.0f, 0.0f, 0.0f);
        glVertex3f(-2.0f, 0.0f, 0.0f);
        
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 2.0f);
        glVertex3f(0.0f, 0.0f, -2.0f);
        /* finish drawing axis lines */
        
        glEnd();
        
        glLineWidth(1.0f); // Change to thinner lines for rest of grid lines
        glBegin(GL_LINES);
        
        glColor3f(0.658824f, 0.658824f, 0.658824f); // change these lines to light gray
        for(float i = -2; i <= 2.2; i += .2) // iterate through -5 to 5 in steps of 1 to draw grid lines
        {
            glVertex3f(i, 2.0f, 0.0f);
            glVertex3f(i, -2.0f, 0.0f);
            
            glVertex3f(2.0f, i, 0.0f);
            glVertex3f(-2.0f, i, 0.0f);
            
            glVertex3f(0.0f, 2.0f, i);
            glVertex3f(0.0f, -2.0f, i);
            
            glVertex3f(0.0f, i, 2.0f);
            glVertex3f(0.0f, i, -2.0f);
            
            glVertex3f(0.0f, 2.0f, 0.0f);
            glVertex3f(0.0f, -2.0f, 0.0f);
            
            glVertex3f(i, 0.0f, 2.0f);
            glVertex3f(i, 0.0f, -2.0f);
            
            glVertex3f(2.0f, 0.0f, i);
            glVertex3f(-2.0f, 0.0f, i);
        }
    }
    
    glEnd();

    glutSwapBuffers();
} // end display

/*
    Function: mouse
 
    Description: Handle logic for mouse input
 
    Parameters: None
 
    Pre-Conditions: None
 
    Post-Conditions: Update dragging flag and mouse positions
 
    Returns: Nothing
*/
void mouse (int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // if left mouse button held down, we are dragging
    {
        dragging = true;
        prev_mousex = float(x); // reassign previous x and y
        prev_mousey = float(y);
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) //  if left moues button released, we are not dragging
    {
        dragging = false;
    }
} // end mouse

/*
    Function: motion
 
    Description: Handles logic for motion of mouse
 
    Parameters: None
 
    Pre-Conditions: None
 
    Post-Conditions: Update swing and elevation based on mouse motion
 
    Returns: Nothing
*/
void motion (int mousex, int mousey)
{
    if (dragging) // if mouse is dragging then update swing and elevation accordingly
    {
        glEnable(GL_COLOR_LOGIC_OP); // set GL_COLOR_LOGIC_OP for XOR
        
        // we take the difference of x and previous x and etc in order to provide gradual rate of rotation
        if (prev_mousex > mousex)
        {
            swing = fmod((swing - (float(mousex) - prev_mousex)), 360.0); // fmod to allow modulus for floats
            glutPostRedisplay();
        }
        if (prev_mousex < mousex)
        {
            swing = fmod((swing - (float(mousex) - prev_mousex)), 360.0);
            glutPostRedisplay();
        }
        if (prev_mousey > mousey)
        {
            elevation = fmod((elevation - (float(mousey) - prev_mousey)), 360.0);
            glutPostRedisplay();
        }
        if (prev_mousey < mousey)
        {
            elevation = fmod((elevation + (prev_mousey - float(mousey))), 360.0);
            glutPostRedisplay();
        }
        prev_mousex = float(mousex); // update previous x and y
        prev_mousey = float(mousey);
        
        glDisable(GL_COLOR_LOGIC_OP); // disable XOR
    }
    
    glFlush();
} // end motion
    
/*
    Function: reshape
 
    Description: Handles logic for reshaping window and objects in scene
 
    Parameters: int w, int h
 
    Pre-Conditions: None
 
    Post-Conditions: objects in window are rescaled
 
    Returns: Nothing
*/
void reshape(int w, int h)
{
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();

    glViewport (0, 0, (GLsizei) fbWidth, (GLsizei) fbHeight);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(30, (GLfloat) fbWidth/(GLfloat) fbHeight, 1.0, 23.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
} // end reshape

/*
    Function: main_menu_select
 
    Description: Handles logic for drop-down menu selection
 
    Parameters: int value
 
    Pre-Conditions: None
 
    Post-Conditions: menu options are executed
 
    Returns: Nothing
*/
void main_menu_select(int value)
{
    if (value == 1) // toggle grid on and off
    {
        grid_toggle = !grid_toggle;
    }
    else if (value == -1) // exit program
    {
        exit(0);
    }
    glutPostRedisplay();
} // end main_menu_select

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    glutCreateMenu(main_menu_select);
    glutAddMenuEntry("Toggle Grid", 1);
    glutAddMenuEntry("Quit", -1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();
    
    return 0;
}
