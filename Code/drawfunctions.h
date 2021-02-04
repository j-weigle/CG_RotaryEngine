#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

void glMultiDrawElements (GLenum, const GLsizei *, GLenum, const GLvoid *, GLsizei);

GLfloat sq (GLdouble);

void normalize (GLdouble *);

void unit_square (void);

void z_cylinder (GLfloat, GLfloat, GLfloat);

void x_cylinder (GLfloat, GLfloat, GLfloat);

void circle (GLfloat, GLfloat);

void zy_circle (GLfloat, GLfloat);

void inner_gear_surface (GLfloat, int);

void outer_gear_surface (GLfloat, int);

void gear_teeth_outline (int, int);

void eccentric_shaft (GLfloat, GLfloat);

void intake_exhaust (void);

void chambers (void);

void rotor (void);

void housing (void);

void spark_plug ();

void sparks (void);

void table (void);

void toolbox (void);

void overhead_light (void);

void flashlight (void);

void walls (void);

void ceiling (void);

void conc_floor (void);

void barrel (void);

#endif
