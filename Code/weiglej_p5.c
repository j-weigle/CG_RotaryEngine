/****************************************************
 *                  Project 5                       *
 ****************************************************
 *  Author:     Justin Weigle                       *
 *  Edited:     06 Dec 2019                         *
 ****************************************************
 * Draws the 3d animation of a Wankel rotary engine *
 * as well as a scene for the engine to be in.      *
 * Allows viewing from all different directions.    *
 * Appropriate objects are textured using SOIL      *
 ****************************************************/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "SOIL/SOIL.h"
#include "drawfunctions.h"

// Animation
static clock_t RESET_TIME, ANIM_TIME = 0;
static GLfloat FPS = 60.0;
static GLfloat ECC_SHFT_I = 0.0;
static GLfloat ECC_SHFT_HEADING;
static GLint ROTATION_SPEED = 3;

// Interaction
static GLint RUN_ANIMATION = 0;

// Camera
static GLfloat CAM_X = 0;
static GLfloat CAM_Y = 0;
static GLfloat CAM_Z = 500;
static GLfloat ZOOM = 25;
static GLfloat CAM_ANGLE_X = 0;
static GLfloat CAM_ANGLE_Y = 0;

// Lights
static GLfloat light0_position[] = {-250.0, 450.0, 0.0, 1.0};
static GLfloat light0_direction[] = {0.0, -1.0, 0.0};

static GLfloat light1_position[] = {250.0, 450.0, 0.0, 1.0};
static GLfloat light1_direction[] = {0.0, -1.0, 0.0};

static GLfloat light2_position[] = {500.0, 500.0, 500.0, 1.0};
static GLfloat light2_direction[] = {-1.0, -1.0, -1.0};

static GLfloat LIGHT3_X = 0.0;
static GLfloat LIGHT3_Y = 0.0;
static GLfloat light3_z = 500.0;
static GLfloat light3_l = 1.0;
static GLfloat light3_direction[] = {0.0, 0.0, -1.0};

// Textures
static GLuint chrome_tex;
static GLuint wood_tex;
static GLuint conc_tex;
static GLuint wall_tex;
static GLuint ceil_tex;
static GLuint barr_refl;

void init (void)
{
    glLoadIdentity();

    // Light Property Arrays
    GLfloat global_ambient[] = {0.2, 0.2, 0.2, 1.0};
        // LIGHT0 (Overhead light left)
    GLfloat light0_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0_specular[] = {0.3, 0.3, 0.3, 1.0};
        // LIGHT1 (Overhead light right)
    GLfloat light1_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light1_specular[] = {0.3, 0.3, 0.3, 1.0};
        // LIGHT2 (spot top right back)
    GLfloat light2_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light2_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light2_specular[] = {1.0, 1.0, 1.0, 1.0};
        // LIGHT3 (interactive spot)
    GLfloat light3_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light3_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light3_specular[] = {1.0, 1.0, 1.0, 1.0};

    // Light Property Settings
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        // LIGHT0 (Overhead light left)
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        // LIGHT1 (Overhead light right)
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        // LIGHT2 (spot top right back)
    glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
        // LIGHT3 (interactive spot)
    glLightfv(GL_LIGHT3, GL_AMBIENT,  light3_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE,  light3_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);

    glClearColor(0.5, 0.5, 0.5, 0.0);
    glShadeModel(GL_SMOOTH);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    chrome_tex = SOIL_load_OGL_texture(
            "../Bitmaps/chrome_sphere.bmp",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS);
    if (!chrome_tex) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, chrome_tex);

    wood_tex = SOIL_load_OGL_texture(
            "../Bitmaps/wood.bmp",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
    if (!wood_tex) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, wood_tex);

    conc_tex = SOIL_load_OGL_texture(
            "../Bitmaps/concrete.bmp",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
    if (!conc_tex) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, conc_tex);

    wall_tex = SOIL_load_OGL_texture(
            "../Bitmaps/wall.bmp",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
    if (!conc_tex) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, wall_tex);

    ceil_tex = SOIL_load_OGL_texture(
            "../Bitmaps/ceiling.bmp",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
    if (!conc_tex) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, ceil_tex);

    barr_refl = SOIL_load_OGL_cubemap(
            "../Bitmaps/barrel_reflect1.png",
            "../Bitmaps/barrel_reflect2.png",
            "../Bitmaps/barrel_reflect3.png",
            "../Bitmaps/barrel_reflect4.png",
            "../Bitmaps/barrel_reflect5.png",
            "../Bitmaps/barrel_reflect6.png",
            SOIL_LOAD_RGBA,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
    if (!barr_refl) {
        printf("***NO BITMAP RETRIEVED***\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, barr_refl);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void reshape (int w, int h)
{
    if (w > h)
        glViewport (0, 0, (GLfloat) h, (GLfloat) h);
    else
        glViewport (0, 0, (GLfloat) w, (GLfloat) w);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective(75.0, 1.0, 5.0, 1000000000.0);

    glMatrixMode (GL_MODELVIEW);

    glLoadIdentity();
}


/****************************************************
 *                draw_environment                  *
 ****************************************************
 * Draws a bit of a workshop for the wankel engine  *
 ****************************************************/
void draw_environment (void)
{
    // ROOM
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glCullFace(GL_FRONT);
    glBindTexture(GL_TEXTURE_2D, wall_tex);
    walls();
    glBindTexture(GL_TEXTURE_2D, ceil_tex);
    ceiling();
    glCullFace(GL_BACK);
    glBindTexture(GL_TEXTURE_2D, conc_tex);
    conc_floor();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    // TABLES
    glBindTexture(GL_TEXTURE_2D, wood_tex);
    glPushMatrix();
        table();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(710, 0, 300);
        glRotatef(90, 0, 1, 0);
        table();
    glPopMatrix();

    // FLASHLIGHT
    glPushMatrix();
        glTranslatef(250, -173, 0);
        flashlight();
    glPopMatrix();

    // TOOLBOX
    glPushMatrix();
        toolbox();
    glPopMatrix();

    // OVERHEAD LIGHT FIXTURES
    glBindTexture(GL_TEXTURE_2D, chrome_tex);
    glPushMatrix();
        glTranslatef(-250, 450, 0);
        overhead_light();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(250, 450, 0);
        overhead_light();
    glPopMatrix();

    // BARREL
    glBindTexture(GL_TEXTURE_CUBE_MAP, barr_refl);
    glPushMatrix();
        glTranslatef(700, -742, 1000);
        barrel();
    glPopMatrix();
}


/****************************************************
 *                  draw_wankel                     *
 ****************************************************
 * Draws the wankel engine internals in 3d with     *
 * animation                                        *
 ****************************************************/
void draw_wankel (void)
{
    // INTAKE
    glPushMatrix();
        glTranslatef(-160, 77.0, -30.0);
        intake_exhaust();
    glPopMatrix();

    // EXHAUST
    glPushMatrix();
        glTranslatef(-160, -77.0, -30.0);
        intake_exhaust();
    glPopMatrix();

    ECC_SHFT_HEADING = -ECC_SHFT_I * 3.1415926535897932384626433832795 / 180.0;
    // ROTOR
    glPushMatrix();
        glTranslatef((cos(ECC_SHFT_HEADING) * 26), (sin(ECC_SHFT_HEADING) * 26), 0.0);
        glRotatef((-ECC_SHFT_I/3) + 30, 0.0, 0.0, 1.0);
        glScalef(1.65, 1.65, 1.0);
        rotor();
    glPopMatrix();

    // ECCENTRIC SHAFT
    glPushMatrix();
        eccentric_shaft(ECC_SHFT_HEADING, ECC_SHFT_I);
    glPopMatrix();

    // PINION
    glPushMatrix();
        glScalef(30.0, 30.0, 1.0);
        outer_gear_surface(0.0, 10);
        outer_gear_surface(-20.0, 10);
        gear_teeth_outline(10, 0);
    glPopMatrix();

    // CROWN GEAR
    glPushMatrix();
        glTranslatef((cos(ECC_SHFT_HEADING) * 26), (sin(ECC_SHFT_HEADING) * 26), 0.0);
        glRotatef((-ECC_SHFT_I/3), 0.0, 0.0, 1.0);
        glScalef(60.0, 60.0, 1.0);
        z_cylinder(-20, 20, 1.3);
        inner_gear_surface(0.0, 5);
        inner_gear_surface(-20.0, 5);
        gear_teeth_outline(5, 1);
    glPopMatrix();

    // HOUSING
    housing();

    // SPARK PLUGS
    glPushMatrix();
        // top plug
        glTranslatef(129.0, 28.0, -30.0);
        spark_plug();
        if (RUN_ANIMATION == 1)
            if (0 < ECC_SHFT_I/3 && ECC_SHFT_I/3 < 10)
            {
                sparks();
            }

        // bottom plug
        glTranslatef(0.0, -64.0, 0.0);
        spark_plug();
        if (RUN_ANIMATION == 1)
            if (0 < ECC_SHFT_I/3 && ECC_SHFT_I/3 < 10)
            {
                sparks();
            }
    glPopMatrix();
}


/****************************************************
 *                    display                       *
 ****************************************************/
void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
        gluLookAt(CAM_X, CAM_Y, CAM_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glRotatef(CAM_ANGLE_X, 0, 1, 0);
        glRotatef(CAM_ANGLE_Y, 1, 0, 0);
        // LIGHTS
        glPushMatrix();
            glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
            glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
            glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 50.0);
        glPopMatrix();
        glPushMatrix();
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
            glLighti( GL_LIGHT1, GL_SPOT_EXPONENT, 50.0);
        glPopMatrix();
        glPushMatrix();
            glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
            glLighti( GL_LIGHT2, GL_SPOT_EXPONENT, 80.0);
            glLighti( GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
        glPopMatrix();
        glPushMatrix();
            GLfloat light3_position[] = {LIGHT3_X, LIGHT3_Y, light3_z, light3_l};
            glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);
            glLighti( GL_LIGHT3, GL_SPOT_EXPONENT, 20.0);
            glLighti( GL_LIGHT3, GL_SPOT_CUTOFF, 90.0);
        glPopMatrix();
        draw_wankel();
        draw_environment();
    glPopMatrix();
    glutSwapBuffers();
}


/****************************************************
 *                    idle                          *
 ****************************************************/
void idle (void)
{
    if (RUN_ANIMATION == 1)
    {
        ANIM_TIME = clock();
        if (ANIM_TIME > RESET_TIME)
        {
            RESET_TIME = RESET_TIME + (1.0/FPS) * CLOCKS_PER_SEC;

            // ROTOR, CROWN GEAR, ECCENTRIC SHAFT, SPARK
            ECC_SHFT_I += (ROTATION_SPEED * 3);
            if (ECC_SHFT_I > (360 - ROTATION_SPEED * 3))
                ECC_SHFT_I = 0;

            glutPostRedisplay();
        }
    }
}


/****************************************************
 *                  menu_choice                     *
 ****************************************************
 * Menu options to reset or exit the program        *
 ****************************************************/
void menu_choice (int selection)
{
    // RESET
    if (selection == 1)
    {
        RUN_ANIMATION = 0;
        ECC_SHFT_I = 0;
        CAM_X = 0;
        CAM_Y = 0;
        CAM_Z = 500;
        CAM_ANGLE_X = 0;
        CAM_ANGLE_Y = 0;
        LIGHT3_X = 0.0;
        LIGHT3_Y = 0.0;
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glutPostRedisplay();
    }
    // EXIT
    if (selection == 2)
    {
        exit(0);
    }
}


/****************************************************
 *              toggle_animation                    *
 ****************************************************
 * Submenu options for turning animation on and off *
 ****************************************************/
void toggle_animation (int selection)
{
    // START ANIMATION
    if (selection == 1)
    {
        RUN_ANIMATION = 1;
    }
    // STOP ANIMATION
    if (selection == 2)
    {
        RUN_ANIMATION = 0;
        glutPostRedisplay();
    }
}


/****************************************************
 *              light_control                       *
 ****************************************************
 * Submenu options for turning lights on and off    *
 ****************************************************/
void light_control (int selection)
{
    // LIGHT 0
    if (selection == 1)
    {
        if(glIsEnabled(GL_LIGHT0))
            glDisable(GL_LIGHT0);
        else
            glEnable(GL_LIGHT0);
        glutPostRedisplay();
    }
    // LIGHT 1
    if (selection == 2)
    {
        if(glIsEnabled(GL_LIGHT1))
            glDisable(GL_LIGHT1);
        else
            glEnable(GL_LIGHT1);
        glutPostRedisplay();
    }
    // LIGHT 2
    if (selection == 3)
    {
        if(glIsEnabled(GL_LIGHT2))
            glDisable(GL_LIGHT2);
        else
            glEnable(GL_LIGHT2);
        glutPostRedisplay();
    }
    // LIGHT 3
    if (selection == 4)
    {
        if(glIsEnabled(GL_LIGHT3))
            glDisable(GL_LIGHT3);
        else
            glEnable(GL_LIGHT3);
        glutPostRedisplay();
    }
}


/****************************************************
 *              get_magnitude_cam                   *
 ****************************************************
 * Gets the magnitude of the camera's normal vector *
 * - used in keyboard                               *
 ****************************************************/
GLfloat get_magnitude_cam (void)
{
     return sqrt(sq(CAM_X) + sq(CAM_Y) + sq(CAM_Z));
}


/****************************************************
 *                      mouse                       *
 ****************************************************
 * Can zoom in and out with mouse wheel             *
 ****************************************************/
void mouse (int btn, int state, int x, int y)
{
    GLfloat m = get_magnitude_cam();
    if (state == GLUT_DOWN){
    switch (btn)
    {
        case 3:
            if ((CAM_X == 0.0) && (CAM_Y == 0.0) && (CAM_Z == 0.0))
            {
                printf("Full zoom, zoom out... if you want to\n");
                glutPostRedisplay();
                break;
            }
            CAM_X -= (CAM_X / m) * ZOOM;
            CAM_Y -= (CAM_Y / m) * ZOOM;
            CAM_Z -= (CAM_Z / m) * ZOOM;
            glutPostRedisplay();
            break;
        // ZOOM OUT
        case 4:
            if ((CAM_X == 0.0) && (CAM_Y == 0.0) && (CAM_Z == 0.0))
            {
                CAM_X = 0.0;
                CAM_Y = 0.0;
                CAM_Z = 50.0;
                glutPostRedisplay();
                break;
            }
            CAM_X += (CAM_X / m) * ZOOM;
            CAM_Y += (CAM_Y / m) * ZOOM;
            CAM_Z += (CAM_Z / m) * ZOOM;
            glutPostRedisplay();
            break;
    }
    }
}


/****************************************************
 *                  keyboard                        *
 ****************************************************
 * Used to control the camera                       *
 ****************************************************/
void keyboard (unsigned char key, int x, int y)
{
    GLfloat m = get_magnitude_cam();
    switch (key)
    {
        // ZOOM IN
        case 'E': case 'e':
            if ((CAM_X == 0.0) && (CAM_Y == 0.0) && (CAM_Z == 0.0))
            {
                printf("Full zoom, zoom out... if you want to\n");
                glutPostRedisplay();
                break;
            }
            CAM_X -= (CAM_X / m) * ZOOM;
            CAM_Y -= (CAM_Y / m) * ZOOM;
            CAM_Z -= (CAM_Z / m) * ZOOM;
            glutPostRedisplay();
            break;
        // ZOOM OUT
        case 'Q': case 'q':
            if ((CAM_X == 0.0) && (CAM_Y == 0.0) && (CAM_Z == 0.0))
            {
                CAM_X = 0.0;
                CAM_Y = 0.0;
                CAM_Z = 50.0;
                glutPostRedisplay();
                break;
            }
            CAM_X += (CAM_X / m) * ZOOM;
            CAM_Y += (CAM_Y / m) * ZOOM;
            CAM_Z += (CAM_Z / m) * ZOOM;
            glutPostRedisplay();
            break;
        // ROTATE CAMERA LEFT
        case 'A': case 'a':
            CAM_ANGLE_X += 5;
            if (CAM_ANGLE_X >= 360)
                CAM_ANGLE_X = 0;
            glutPostRedisplay();
            break;
        // ROTATE CAMERA RIGHT
        case 'D': case 'd':
            CAM_ANGLE_X -= 5;
            if (CAM_ANGLE_X <= 0)
                CAM_ANGLE_X = 360;
            glutPostRedisplay();
            break;
        // ROTATE CAMERA AROUND X CLOCKWISE
        case 'W': case 'w':
            CAM_ANGLE_Y += 5;
            if(CAM_ANGLE_Y >= 360)
                CAM_ANGLE_Y = 0;
            glutPostRedisplay();
            break;
        // ROTATE CAMERA AROUND X COUNTER CLOCKWISE
        case 'S': case 's':
            CAM_ANGLE_Y -= 5;
            if (CAM_ANGLE_Y <= 0)
                CAM_ANGLE_Y = 360;
            glutPostRedisplay();
            break;
        case 27: case 'f':
            exit(0);
        default:
            break;
    }
}


/****************************************************
 *                spot_control                      *
 ****************************************************
 * Used to control the spotlight                    *
 ****************************************************/
void spot_control (int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            LIGHT3_Y += 15;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            LIGHT3_X += 15;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            LIGHT3_Y -= 15;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            LIGHT3_X -= 15;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize (900, 900);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();

    // make cursor visible during recording
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

    // MENU CREATION
    int submenu = glutCreateMenu(toggle_animation);
    glutAddMenuEntry("Start Animation", 1);
    glutAddMenuEntry("Stop Animation", 2);

    int lightmenu = glutCreateMenu(light_control);
    glutAddMenuEntry("Toggle Light 0 (overhead left)", 1);
    glutAddMenuEntry("Toggle Light 1 (overhead right)", 2);
    glutAddMenuEntry("Toggle Light 2 (spotlight top right back)", 3);
    glutAddMenuEntry("Toggle Light 3 (interactive spot)", 4);

    glutCreateMenu(menu_choice);
    glutAddSubMenu("Toggle Animation", submenu);
    glutAddSubMenu("Light Control", lightmenu);
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Exit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(spot_control);
    glutIdleFunc(idle);
    glutMainLoop ();
    return 0;
}
