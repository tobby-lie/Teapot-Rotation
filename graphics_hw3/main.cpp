#define GL_SILENCE_DEPRECATION // silence deprecation warnings

#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

static int elevation = 0;
static int swing = 0;
static std::vector<float> look_vector = {0, 0, 10};
static int prev_mousex, prev_mousey = 0;
bool dragging = false;

// struct to hold x, y coordinates
struct Coordinates
{
    double x, y;
}; // end Coordinates

static std::vector<Coordinates> x_grid_coord_vec;

std::vector<float> cross_product(std::vector<float> v1, std::vector<float> v2)
{
    float uv_i = v1[1] * v2[2] - v2[1] * v1[2];
    float uv_j = v2[0] * v1[2] - v1[0] * v2[2];
    float uv_k = v1[0] * v2[1] - v2[0] * v1[1];
    
    std::vector<float> temp_vector;
    temp_vector.push_back(uv_i);
    temp_vector.push_back(uv_j);
    temp_vector.push_back(uv_k);
    
    return temp_vector;
}

std::vector<float> normalize_vector(std::vector<float> v)
{
    float magnitude = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
    
    std::vector<float> temp_vector;
    temp_vector.push_back(v[0]/magnitude);
    temp_vector.push_back(v[1]/magnitude);
    temp_vector.push_back(v[2]/magnitude);
    
    return temp_vector;
}

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

void myinit(void)
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
}

void display(void)
{
//    GLfloat low_ambient[] =
//    {0.1, 0.1, 0.1, 1.0};
    GLfloat more_ambient[] =
    {0.4, 0.4, 0.4, 1.0};
//    GLfloat most_ambient[] =
//    {1.0, 1.0, 1.0, 1.0};
    
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.196078, 0.6, 0.8);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /*  material has moderate ambient reflection */
    glMaterialfv(GL_FRONT, GL_AMBIENT, more_ambient);
    glPushMatrix();
    glLoadIdentity();

    std::vector<float> temp_look_vector = {0, 0, 10};
    std::vector<float>norm_vector_ptr = normalize_vector(temp_look_vector);

    std::vector<float> temp_up_vector = {0, 1, 0};
    std::vector<float> right_vector_ptr = cross_product(norm_vector_ptr, temp_up_vector);

    std::vector<float> look_vector_ptr = cross_product(right_vector_ptr, norm_vector_ptr);
    
    for(int i = 0; i < 3; i++){
        look_vector[i] = look_vector_ptr[i];
    }
    
    gluLookAt(0, 0, 10, 0, 0, -1, look_vector[0], look_vector[1], look_vector[2]);
    
    glRotatef(elevation, 1, 0, 0);
    glRotatef(swing, 0, 1, 0);
    glutSolidTeapot(1.0);
    
    glLineWidth(6.0);
    glBegin(GL_LINES);
    
    glColor3f(1, 0, 0);
    glVertex3f(0, 5.0f, 0.0f);
    glVertex3f(0, -5.0f, 0.0f);
    
    glColor3f(0, 1, 0);
    glVertex3f(5.0f, 0, 0.0f);
    glVertex3f(-5.0f, 0, 0.0f);
    
    glColor3f(0, 0, 1);
    glVertex3f(0.0f, 0, 5.0f);
    glVertex3f(0.0f, 0, -5.0f);
    
    glEnd();
    
    glLineWidth(1.0);
    glBegin(GL_LINES);
    
    glColor3f(1, 1, 1);
    for( float i = -5; i <= 5; i += 1)
    {
        glVertex3f(i, 5.0f, 0.0f);
        glVertex3f(i, -5.0f, 0.0f);
        
        glVertex3f(5.0f, i, 0.0f);
        glVertex3f(-5.0f, i, 0.0f);
        
        glVertex3f(0.0f, 5.0f, i);
        glVertex3f(0.0f, -5.0f, i);
        
        glVertex3f(0.0f, i, 5.0f);
        glVertex3f(0.0f, i, -5.0f);
        
        glVertex3f(0, 5.0f, 0.0f);
        glVertex3f(0, -5.0f, 0.0f);
    }
    
////    glColor3f(0.3, 0.3, 1.0);
//    glLineWidth(3.0);
//    glVertex3f(0, 5.0f, 0.0f);
//    glVertex3f(0, -5.0f, 0.0f);
    
    glEnd();
       
    glPopMatrix();
    
    
    glFlush();

    glutSwapBuffers();
}

void mouse (int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        dragging = true;
        prev_mousex = x;
        prev_mousey = y;
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        dragging = false;
    }
}

void motion (int mousex, int mousey)
{
    if (dragging)
    {
        glEnable(GL_COLOR_LOGIC_OP); // set GL_COLOR_LOGIC_OP for XOR
        
        if (prev_mousex > mousex)
        {
            swing = (swing - (mousex - prev_mousex)) % 360;
            glutPostRedisplay();
        }
        if (prev_mousex < mousex)
        {
            swing = (swing + (prev_mousex - mousex)) % 360;
            glutPostRedisplay();
        }
        if (prev_mousey > mousey)
        {
            elevation = (elevation - (mousey - prev_mousey)) % 360;
            glutPostRedisplay();
        }
        if (prev_mousey < mousey)
        {
            elevation = (elevation + (prev_mousey - mousey)) % 360;
            glutPostRedisplay();
        }
        prev_mousex = mousex;
        prev_mousey = mousey;
        
        glDisable(GL_COLOR_LOGIC_OP); // disable XOR
    }
    
    glFlush();
}
    

void myReshape(int w, int h)
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
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    
    myinit();
    
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    glutMainLoop();
    
    return 0;
}
