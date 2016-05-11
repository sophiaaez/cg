#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

    // Setup the animation timer to fire every x msec
    animtimer = new QTimer(this);
    animtimer->start( 50 );

    // Everytime the timer fires, the animation is going one step forward
    connect(animtimer, SIGNAL(timeout()), this, SLOT(stepAnimation()));

    animstep = 0;

    rotx = 0;
    roty = 0;
    rotz = 0;
    zoom = 100;
    ox, oz, vx, vz, ax, az = 0;
    perspective = true;


}

OGLWidget::~OGLWidget()
{    drawable = true;

}

void OGLWidget::stepAnimation()
{
    animstep++;    // Increase animation steps
    update();      // Trigger redraw of scene with paintGL
}


void OGLWidget::setRotX(int newrx)
{
    rotx = newrx;
    update();
}

void OGLWidget::setRotY(int newry)
{
    roty = newry;
    update();
}

void OGLWidget::setRotZ(int newrz)
{
    rotz = newrz;
    update();
}

void OGLWidget::setZoom(int newzoom)
{
    zoom = newzoom;
    update();
}

void OGLWidget::setUnfold(int newunfold)
{
    unfold = newunfold;
    update();
}


void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // For wireframe replace GL_FILL with GL_LINE
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    GLfloat lp1[4]  = { 20,  0,  0, 0};
    GLfloat lp2[4]  = { -20, 0 , 0,  0};
    GLfloat red[4]  = {1.0, 0 , 0,  1};
    GLfloat green[4] = { 0, 1.0, 0.0,  1};
    //GLfloat blue[4] = { 0, 0.0, 1.0,  1};
    glClearColor(0,0,0,0); //HIER AENDERUNG 0 0 0 1
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);


}

void OGLWidget::paintGL()
{

    double a = animstep;   // rotate one degree with each step
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if(perspective){
        glOrtho(-10,10,-10,10,-100,100);
    }else{
        glFrustum(-5,+5,-5,+5,5,50);
    }

    glTranslatef(.0,.0,-5);

    // Apply rotation angles
    glRotatef(rotx, 1.0f, 0.0f, 0.0f); // Rotate around x axis
    glRotatef(roty, 0.0f, 1.0f, 0.0f); // Rotate around y axis
    glRotatef(rotz, 0.0f, 0.0f, 1.0f); // Rotate around z axis

    // Apply scaling
    float scale = zoom/100.0;
    glScalef( scale, scale, scale ); // Scale along all axis


    glRotatef(30,1,0,0);
    paintTable(10,14);
    glTranslatef(5,0,7);
    //glRotatef(90,0,0,1);
    //glRotatef(90,1,0,0);
    glRotatef(270,0,1,0);
    glTranslatef(7,0,4);
    paintFlipperArm(1,3,1);
    glTranslatef(-7,0,-4);
    glRotatef(90,0,1,0);
    //glRotatef(330,1,0,0);

    //glRotatef(270,1,0,0);
    //glRotatef(90,0,1,0);
    glPushMatrix();
    glTranslatef(-3,0.5, 0);
    glScalef(0.5,0.5,0.5);

    /**glTranslatef(-5,0,0);
    drawSphere(1,35,35);
    glTranslatef(5,0,0);**/
    glScalef(2,2,2);
    glTranslatef(ox, 0, oz);
   // glRotatef(a, 0.0f, 0.0f, 1.0f); // Rotate by a degrees around z axis
    glScalef(0.5,0.5,0.5);
    glColor3f(0.75,0,0.1);
    drawSphere(a,35,35);
    glPopMatrix();

    //dx = (dx+dxN)*(0.5);
    //dy = (dy+dyN)*(0.5);
   /** if(-5 < ox+vx*0.1 && ox+vx*0.1 <5 && -7 < oz+vz*0.1 && oz+vz*0.1 <7){
        vx = -vx;
        vz = -vz;
    }**/

    if(-6.5 < ox+vx*0.1 && ox+vx*0.1 <2.5){

    }else{
        vx = -vx;
        //ax = -ax;
    }
    if(-7 < oz+vz*0.1 && oz+vz*0.1 <7){

    }else{
        vz = 0;
        az = 0;
    }
    //vx = vx - ax * 0.1;
    vz = vz + az * 0.1;
    ox = ox + vx * 0.1;
    oz = oz + vz * 0.1;
    update();
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
    // Upon mouse pressed, we store the current position...
    lastpos = event->pos();
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    dxN = event->x() -lastpos.x();
    dyN = event->y() -lastpos.y();
    //while(dx == event->x() -lastpos.x() && dy == event->y() -lastpos.y()){

   // }
//    dx = (dx+dxN)*(0.001);
//    dy = (dy+dyN)*(0.001);
//    update();
    if(dxN > 10){
        dxN *= 0.1;
    }
    if(dxN > 10){
        dxN *= 0.1;
    }
    if(dyN > 10){
        dyN *= 0.1;
    }
    if(dyN > 10){
        dyN *= 0.1;
    }

    vx = dxN;
    vz = dyN;
    az = 0.5;
    ax = 0.1;
}



void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    // This is the delta we want to use for rotating
    const int keyDelta = 10;

    switch(event->key())
    {
        // Up/Down: Rotating around x axis
        case Qt::Key_Up:

            emit changeRotation( keyDelta, 0, 0 );
            break;
        case Qt::Key_Down:
            emit changeRotation( -keyDelta, 0, 0 );
            break;

        // Left/Right: Rotating around y axis
        case Qt::Key_Left:
            emit changeRotation( 0, keyDelta, 0 );
            break;
        case Qt::Key_Right:
            emit changeRotation( 0, -keyDelta, 0 );
            break;

        // Pg up/down: Rotating around z axis
        case Qt::Key_PageUp:
            emit changeRotation( 0, 0, keyDelta );
            break;
        case Qt::Key_PageDown:
            emit changeRotation( 0, 0, -keyDelta );
            break;
        case Qt::Key_P:
            perspective = !perspective;
            break;
        // All other will be ignored
        default:
            break;
    }
}

void OGLWidget::drawSphere(double r, int lats, int longs) {
    r = 0.1*animstep;   // rotate one degree with each step

        int i, j;

        for(i = 0; i <= lats; i++) {
            double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
            double z0  = sin(lat0);
            double zr0 =  cos(lat0);

            double lat1 = M_PI * (-0.5 + (double) i / lats);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

         glBegin(GL_QUAD_STRIP);
         for(j = 0; j <= longs; j++) {
             double lng = 2 * M_PI * (double) (j - 1) / longs;
             double x = cos(lng);
             double y = sin(lng);
             glNormal3f(x * zr0, y * zr0, z0);
             glVertex3f(x * zr0, y * zr0, z0);
             glNormal3f(x * zr1, y * zr1, z1);
             glVertex3f(x * zr1, y * zr1, z1);

           }
           glEnd();
       }
}

void OGLWidget::paintCylinder(float r, float h){
    int alpha = 1;
    int amount = 360/alpha;
    for(int i = 0; i < amount; i++){
        paintTriangle(r, alpha);
        paintRectangle(r, h, alpha);
        glRotatef(alpha * 1.0, 0.0,1.0,0.0);
    }
    //glRotatef(alpha,0,1,0);
    glTranslatef(0,h,0);
    for(int i = 0; i < amount; i++){
        paintTriangle(r, alpha);
        glRotatef(alpha, 0, 1, 0);
    }
    glTranslatef(0,-h,0);
    //glRotatef(alpha,0,1,0);

}

void OGLWidget::paintCircle(float r, int alpha){
    int amount = 360/alpha;
    for(int i = 0; i < amount; i++){
        paintTriangle(r, alpha);
        glRotatef(alpha * 1.0, 0.0,1.0,0.0);
    }
    //glRotatef(alpha, 0,1,0);
}

void OGLWidget::paintTriangle(float r, int alpha){
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.5, 0);
        glVertex3f(0,0,0);
        glVertex3f(cos(0)*r,0,0);
        glVertex3f(cos(alpha)*r, 0, sin(alpha)*r);
        glNormal3f(0,1,0);
    glEnd();
}

void OGLWidget::paintFlipperArm(float w, float l, float h){
    //glRotatef(90, 1,0,0);
    float r = w/2;
    glColor3f(1,0.5,0);
    paintCylinder(r,h);
    //glTranslatef(0,-w,0);
    glBegin(GL_TRIANGLES);
        glVertex3f(r, 0, 0);
        glVertex3f(-r, 0,0);
        glVertex3f(0, 0, l-r);
        glNormal3f(0,1,0);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(r, h, 0);
        glVertex3f(-r, h,0);
        glVertex3f(0, h, l-r);
        glNormal3f(0,1,0);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(0,0,l-r);
        glVertex3f(0,h,l-r);
        glVertex3f(-r,h,0);
        glVertex3f(-r,0,0);
        glNormal3f(h*(l-r),0,-h*r);
     glEnd();
     glBegin(GL_QUADS);
         glVertex3f(0,0,l-r);
         glVertex3f(0,h,l-r);
         glVertex3f(r,h,0);
         glVertex3f(r,0,0);
         glNormal3f(h*(l-r),0,h*r);
      glEnd();
      //glTranslatef(0,w,0);
      //glRotatef(270,1,0,0);
      //glTranslatef(h,0,0);
}

void OGLWidget::paintTable(float w, float h){
    glColor3f(0,0.5,1);
    paintRectangle(w, 0.5);
    glRotatef(90, 0,1,0);
    glTranslatef(0.5*h,0,0.5*w);
    paintRectangle(h,0.5);
    glRotatef(90, 0,1,0);
    glTranslatef(0.5*w,0,0.5*h);
    paintRectangle(w, 0.5);
    glRotatef(90, 0,1,0);
    glTranslatef(0.5*h,0,0.5*w);
    paintRectangle(h,0.5);
    glTranslatef(-1.5*h,0,-1.5*w);
    glTranslatef(h,0,w);
    glRotatef(90,0,1,0);
    glRotatef(90, 1,0,0);
    paintRectangle(w,h);
    //glTranslatef(-h,0,-w);
    glRotatef(270,1,0,0);
}

void OGLWidget::paintRectangle(float r, float h, int alpha){
    glBegin(GL_QUADS);
        glVertex3f(r,0,0);
        glVertex3f(cos(alpha)*r, 0, sin(alpha)*r);
        glVertex3f(r,h,0);
        glVertex3f(cos(alpha)*r, h, sin(alpha)*r);
        glNormal3f(cos(alpha/2), 0, sin(alpha/2));
    glEnd();
}

void OGLWidget::paintRectangle(float w, float h){
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glVertex3f(- 0.5*w,0,0);
        glVertex3f(0.5*w,0,0);
        glVertex3f(0.5*w,h,0);
        glVertex3f(-0.5*w,h,0);
    glEnd();
}
