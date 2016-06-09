#include "oglwidget.h"
#include "iostream"
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
    cy_x = 0;
    cy_z = 0;
    cu_x = -3;
    cu_z = -4;
    done = false;
    up = false;
    faa = 0; //Flipper arm alpha


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

    // Apply rotation angles
    glRotatef(rotx, 1.0f, 0.0f, 0.0f); // Rotate around x axis
    glRotatef(roty, 0.0f, 1.0f, 0.0f); // Rotate around y axis
    glRotatef(rotz, 0.0f, 0.0f, 1.0f); // Rotate around z axis

    // Apply scaling
    float scale = zoom/100.0;
    glScalef( scale, scale, scale ); // Scale along all axis



    glRotatef(30,1,0,0);
    paintTable(10,14, 3);
    glColor3f(0,1,0);
    glTranslatef(1,0,7);
    glRotatef(270,0,1,0);
    paintFlipperArm(1,3,1);
    glRotatef(90,0,1,0);
    glTranslatef(-1,0,-7);
    glColor3f(0.75,0,0.1);
    glTranslatef(cy_x, 0, cy_z);
    paintCylinder(0.6, 1);
    glTranslatef(-cy_x, 0, -cy_z);
    glColor3f(0.5,0,0.5);
    glTranslatef(cu_x, 0, cu_z);
    paintCube(1);
    glTranslatef(-cu_x, 0, -cu_z);


    glPushMatrix();

    glScalef(0.5,0.5,0.5);
    glScalef(2,2,2);
    glTranslatef(ox, 0, oz);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.75,0,0.1);
    drawSphere(a,35,35);
    glScalef(2,2,2);
    glTranslatef(-ox,0,-oz);
    glPopMatrix();

    //dx = (dx+dxN)*(0.5);
    //dy = (dy+dyN)*(0.5);

    if(done){
        if(-4.5 < ox+vx*0.1 && ox+vx*0.1 <4.5){

        }else{
            vx = -vx;
            //ax = -ax;
        }
        if(-6.5 < oz+vz*0.1 && oz+vz*0.1 <6.5){

        }else{
            vz = 0;
            az = 0;
        }
        if(done && cu_x -0.5 < ox+vx*0.1 && ox+vx*0.1 < cu_x +0.5 && cu_z -0.5 < oz+vz*0.1 && oz+vz*0.1 < cu_z + 0.5){
            //Zusammenstoß mit Cube eventuell
        }
        if(done && cy_x -0.5 < ox+vx*0.1 && ox+vx*0.1 < cy_x +0.5 && cy_z -0.5 < oz+vz*0.1 && oz+vz*0.1 < cy_z + 0.5){
            //Zusammenstoß mit Cylinder eventuell
        }



        //TODO if mit kollision jeder bande, vx und cz in geraden vektoren ändern :)
        //bande rechts
        double brxt = ((ox+vx*0.1)+1 - 0.5*3)/(0.5*10 - 0.5*3); //bande rechts x t-wert
        double brzt = ((oz+vz*0.1)+1 - (0.5*14 - 0.5))/((0.5*14-0.75*3) - (0.5*14-0.5)); //bande rechts z t-wert
        //std::cout<< "brxt: "<<brxt<<" brzt: "<<brzt<<std::endl;
        if(aufStrecke(brxt, brzt)){
            //std::cout<<vx<<" "<<vz<<std::endl;
            vx = -(0.5*10 - 0.5*3)*0.5;
            vz = -((0.5*14-0.75*3)-(0.5*14-0.5))*0.5;
            std::cout<<"bande rechts getroffen "<< brxt << " " <<brzt<< " " <<vx << " " <<vz<<std::endl;
        }
        //bande links
        double blxt = ((ox+vx*0.1)-1 + 0.5*3)/(-0.5*10 + 0.5*3); //bande links x t-wert
        double blzt = ((oz+vz*0.1)+1 - (0.5*14 - 0.5))/((0.5*14-0.75*3) - (0.5*14-0.5)); //bande links z t-wert
        //std::cout<<"blxt: " <<blxt<<" blzt: "<<blzt<<std::endl;
        if(aufStrecke(blxt, blzt)){
            vx = -(-0.5*10 + 0.5*3)*0.5;
            vz = -((0.5*14-0.75*3) - (0.5*14-0.5))*0.5;
            std::cout<<"bande links getroffen "<<blxt<< " "<< blzt<<vx << " " <<vz<<std::endl;
        }
        //flipper arm
        double paxt = ((ox+vx*0.1) - (- cos(faa)*0.5*3 + sin(faa)*0.5*14))/((cos(faa)*0.5*3+ sin(faa)*(0.5*14-0.5)) - (- cos(faa)*0.5*3 + sin(faa)*0.5*14));//pinball arm x t-wert
        double pazt = ((oz+vz*0.1)+1 - (sin(faa)*0.5*3 + cos(faa)*0.5*14))/((-sin(faa)*0.5*3+cos(faa)*(0.5*14-0.5)) - (sin(faa)*0.5*3 + cos(faa)*0.5*14));//pinball arm z t-wert
        if(aufStrecke(paxt, pazt)){
            vx = -((cos(faa)*0.5*3+ sin(faa)*(14-0.5)) - (- cos(faa)*0.5*3 + sin(faa)*7));
            vz = (-sin(faa)*0.5*3+cos(faa)*(14-0.5)) - (sin(faa)*0.5*3 + cos(faa)*7);
            std::cout<<"flipperarm getroffen "<<paxt<<" "<<pazt<<" "<<vx<<" "<<vz<<std::endl;
        }
        //kollision mit dem kleinen teil der linken bande
        if((ox+vx*0.1) <= -0.5*3 && (oz+vz*0.1) > ((0.5*14)-1)){
            std::cout<<"hallo, hier"<<(ox+vx*0.1)+1<<std::endl;
            vx = 0;
        }

        //TODO hier richtige schwerkraft einbauen

        //vx = vx + ax * 0.1;
        //vz = vz + az * 0.1;
        ox = ox + vx * 0.1;
        oz = oz + vz * 0.1;
    }
    update();
}

boolean OGLWidget::aufStrecke(float xt, float zt){
    boolean aufstrecke = false;
    //da die werte nur auf eine nachkommastellen übereinstimmen müssen, da wir sonst nie auf ein ergebnis kommen
    xt = (float)((int)(xt*100))/100;
    zt = (float)((int)(zt*100))/100;
    if(xt <= 1 && xt >= 0){
        aufstrecke = true;
    }else{
        return false;
    }
    if(xt == zt){
        return true && aufstrecke;
    }
    int xrest1 = (xt*10);
    int zrest1 = (zt*10);
    int xrest = (xt - (float)((int)(xt*10))/10)*100;
    int zrest = (zt - (float)((int)(zt*10))/10)*100;
    if(xrest1 == zrest1){
        return closeTo(xrest, zrest) && aufstrecke;
    }
    if(closeTo(xrest1,zrest1)){
        if(xrest == 0 && (zrest == 9 || zrest == 8)){
            return true && aufstrecke;
        }else if( zrest == 0 && (xrest == 9 || xrest == 8)){
            return true && aufstrecke;
        }else if(xrest == 1 && zrest == 9){
            return true && aufstrecke;
        }else if(zrest == 1 && xrest == 9){
            return true && aufstrecke;
        }else if(xrest == 0 && zrest == 0){
            return true && aufstrecke;
        }
    }
    return false;
}

boolean OGLWidget::closeTo(int i, int j){ // i und j sind ziffern
    switch(i){
    case 0: if(j == 1){
            return true;
            }
            break;
    case 1: if(j == 0 || j == 2 || j == 3){
            return true;
            }
            break;
    case 2: if(j == 0 || j == 1 || j == 3 || j == 4){
            return true;
            }
            break;
    case 3: if(j == 1 || j == 2 || j == 4 || j == 5){
            return true;
            }
            break;
    case 4: if(j == 2 || j == 3 || j == 5 || j == 6){
            return true;
            }
            break;
    case 5: if(j == 3 || j == 4 || j == 6 || j == 7){
            return true;
            }
            break;
    case 6: if(j == 4 || j == 5 || j == 7 || j == 8){
            return true;
            }
            break;
    case 7: if(j == 5 || j == 6 || j == 8 || j == 9){
            return true;
            }
            break;
    case 8: if(j == 6 || j == 7 || j == 9){
            return true;
            }
            break;
    case 9: if(j == 7 || j == 8){
            return true;
            }
            break;
    }
    return false;
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
    vz = 1;
    az = 0.01;
    done = true;
    //dxN = event->x() -lastpos.x();
    //dyN = event->y() -lastpos.y();
    //while(dx == event->x() -l) &&astpos.x( dy == event->y() -lastpos.y()){

   // }
//    dx = (dx+dxN)*(0.001);
//    dy = (dy+dyN)*(0.001);
//    update();
    if(dxN > 10){
        //dxN *= 0.1;
    }
    if(dxN > 10){
        //dxN *= 0.1;
    }
    if(dyN > 10){
        //dyN *= 0.1;
    }
    if(dyN > 10){
        //dyN *= 0.1;
    }

    //vx = dxN;
    //vz = dyN;
    //az = 0.5;
   // ax = 0.1;
}



void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    // This is the delta we want to use for rotating
    const int keyDelta = 10;

    switch(event->key())
    {
        case Qt::Key_Up:
            if(!done && cy_z-0.1 > -6.4){
                cy_z -= 0.1;
            }
            break;
        case Qt::Key_Down:
            if(!done&& cy_z+0.1 < 6.4){
                cy_z += 0.1;
            }
            break;
        case Qt::Key_Left:
            if(!done&& cy_x-0.1 > -4.5){
                cy_x -= 0.1;
            }
            break;
        case Qt::Key_Right:
            if(!done&& cy_x+0.1 < 4.5){
                cy_x += 0.1;
            }
            break;
        case Qt::Key_E:
            if(!done&& cu_z-0.1 > -6){
                cu_z -= 0.1;
            }
            break;
        case Qt::Key_D:
            if(!done&& cu_z+0.1 < 7){
                cu_z += 0.1;
            }
            break;
        case Qt::Key_S:
            if(!done&& cu_x-0.1 > -5){
                cu_x -= 0.1;
            }
            break;
        case Qt::Key_F:
            if(!done&& cu_x+0.1 < 4.1){
                cu_x += 0.1;
            }
            break;
        case Qt::Key_Return:
            if(!done){
                vz = 1;
                az = 0.01;
                done = true;
            }
        break;
        case Qt::Key_Space:
            if(!up){
                up = true;
                //positionsaenderung fuer flipperarm
            }
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
    int x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4, nx, ny, nz, v1x, v1y, v1z, v2x, v2y, v2z, xp, yp, zp;
    /**x1 = cos(0)*r;
    x2 = cos(alpha)*r;
    x3 = x2;
    x4 = x1;
    y1 = 0;
    y2 = y1;
    y3 = h;
    y4 = y3;
    z1 = 0;
    z2 = sin(alpha)*r;
    z3 = z2;
    z4 = z3;
    z2 = z1;*/
    paintCircle(r, alpha, 0);
    glTranslatef(0,h,0);
    paintCircle(r, alpha, h);
    glTranslatef(0,-h,0);
    for(int i = 0; i < amount; i++){
        /**glBegin(GL_QUADS);
            glVertex3f(x1,y1,z1);
            glVertex3f(x2,y2,z2);
            glVertex3f(x3,y3,z3);
            glVertex3f(x4,y4,z4);
            glNormal3f(nx,ny,nz);
        glEnd();
        //rotationsmatrix anwenden
        xp = cos(alpha)*x1 + 0*y1 + sin(alpha)*z1;
        yp = 0*x1 + 1*y1 + 0*z1;
        zp = (-sin(alpha))*x1 + 0*y1 + cos(alpha)*z1;
        x1 = xp;
        y1 = yp;
        z1 = zp;

        xp = cos(alpha)*x2 + 0*y2 + sin(alpha)*z2;
        yp = 0*x2 + 1*y2 + 0*z2;
        zp = (-sin(alpha))*x2 + 0*y2 + cos(alpha)*z2;
        x2 = xp;
        y2 = yp;
        z2 = zp;

        x3 = cos(alpha)*x3 + 0*y3 + sin(alpha)*z3;
        y3 = 0*x3 + 1*y3 + 0*z3;
        z3 = (-sin(alpha))*x3 + 0*y3 + cos(alpha)*z3;
        x3 = xp;
        y3 = yp;
        z4 = zp;

        x4 = cos(alpha)*x4 + 0*y4 + sin(alpha)*z4; 
        y4 = 0*x4 + 1*y4 + 0*z4;
        z4 = (-sin(alpha))*x4 + 0*y4 + cos(alpha)*z4;
        x4 = xp;
        y4 = yp;
        z4 = zp;

        v1x = x1 + (x2 - x1);
        v2x = x1 + (x3 - x1);
        v1y = y1 + (y2 - y1);
        v2y = y1 + (y3 - y1);
        v1z = z1 + (z2 - z1);
        v2z = z1 + (z3 - z1);

        nx = v1y*v2z - v2y*v1z;
        ny = v1z*v2x - v2z*v1x;
        nz = v1x*v2y - v2x*v1y;**/

        paintTriangle(r, alpha);
        paintRectangle(r, h, alpha);
        glRotatef(alpha * 1.0, 0.0,1.0,0.0);
    }
    //glRotatef(alpha,0,1,0);
    //glTranslatef(0,h,0);
    //for(int i = 0; i < amount; i++){
    //  paintTriangle(r, alpha);
    //    glRotatef(alpha, 0, 1, 0);
    //}
    //glTranslatef(0,-h,0);
    //glRotatef(alpha,0,1,0);

}

void OGLWidget::paintCircle(float r, int alpha, int y){ //Wahrscheinlicher fehler alpha in bogen oder gradmaß, das was halt falsch ists
    int amount = 360/alpha;
    alpha = alpha/360 * 2 * M_PI;
    int x1, x2, z1, z2;
    x1 = r;
    //x2 = cos(alpha)*r;
    z1 = 0;
    //z2 = -sin(alpha)*r;
    for(int i = 0; i < amount; i++){
        /**glBegin(GL_TRIANGLES);
            glVertex3f(0,y,0);
            glVertex3f(x1,y,z1);
            x2 = cos(alpha)*x1 + sin(alpha)*z1;
            z2 = -sin(alpha)*x1 + cos(alpha)*z1;
            x1 = x2;
            z1 = z2;
            glVertex3f(x1,y,z1);
            glNormal3f(0,1,0);
        glEnd();**/
        //rotationsmatrix anwenden
        //x1 = cos(alpha)*x1 + sin(alpha)*z1;
        //x2 = cos(alpha)*x2 + sin(alpha)*z2;
        //z1 = (-sin(alpha))*x1 + cos(alpha)*z1;
        //z2 = (-sin(alpha))*x2 + cos(alpha)*z2;
        paintTriangle(r, alpha);
        glRotatef(alpha * 1.0, 0.0,1.0,0.0);
    }
    //glRotatef(alpha, 0,1,0);
}

void OGLWidget::paintTriangle(float r, int alpha){
    glBegin(GL_TRIANGLES);
        glVertex3f(0,0,0);
        glVertex3f(cos(0)*r,0,0);
        glVertex3f(cos(alpha)*r, 0, sin(alpha)*r);
        glNormal3f(0,1,0);
    glEnd();
}

void OGLWidget::paintFlipperArm(float w, float l, float h){
    float r = w/2;
    glColor3f(1,0.5,0);
    paintCylinder(r,h);
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0);
        glVertex3f(r, 0, 0);
        glVertex3f(-r, 0,0);
        glVertex3f(0, 0, l-r);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0);
        glVertex3f(r, h, 0);
        glVertex3f(-r, h,0);
        glVertex3f(0, h, l-r);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(h*(l-r),0,-h*r);
        glVertex3f(0,0,l-r);
        glVertex3f(0,h,l-r);
        glVertex3f(-r,h,0);
        glVertex3f(-r,0,0);
     glEnd();
     glBegin(GL_QUADS);
         glNormal3f(h*(l-r),0,h*r);
         glVertex3f(0,0,l-r);
         glVertex3f(0,h,l-r);
         glVertex3f(r,h,0);
         glVertex3f(r,0,0);
      glEnd();
}

void OGLWidget::paintTable(float w, float h, float a){
    glColor3f(0,0.1,1);
    glBegin(GL_QUADS); //Tischplatte
        glNormal3f(0,1,0);
        glVertex3f(-0.5*w, 0, -0.5*h);
        glVertex3f(-0.5*w, 0, 0.5*h);
        glVertex3f(0.5*w, 0, 0.5*h);
        glVertex3f(0.5*w, 0, -0.5*h);
    glEnd();
    glColor3f(1,0,0);
    glBegin(GL_QUADS); //seite rechts
        glNormal3f(1,0,0);
        glVertex3f(0.5*w, 0, -0.5*h);
        glVertex3f(0.5*w, 0, 0.5*h);
        glVertex3f(0.5*w, 0.5, 0.5*h);
        glVertex3f(0.5*w, 0.5, -0.5*h);
    glEnd();
    glBegin(GL_QUADS); //seite links
        glNormal3f(1,0,0);
        glVertex3f(-0.5*w, 0, -0.5*h);
        glVertex3f(-0.5*w, 0, 0.5*h);
        glVertex3f(-0.5*w, 0.5, 0.5*h);
        glVertex3f(-0.5*w, 0.5, -0.5*h);
    glEnd();
    glBegin(GL_QUADS); //seite vorne
        glNormal3f(0,0,1);
        glVertex3f(0.5*w, 0, 0.5*h);
        glVertex3f(-0.5*w, 0, 0.5*h);
        glVertex3f(-0.5*w, 0.5, 0.5*h);
        glVertex3f(0.5*w, 0.5, 0.5*h);
    glEnd();
    glBegin(GL_QUADS); //seite links
        glNormal3f(1,0,0);
        glVertex3f(0.5*w, 0, -0.5*h);
        glVertex3f(-0.5*w, 0, -0.5*h);
        glVertex3f(-0.5*w, 0.5, -0.5*h);
        glVertex3f(0.5*w, 0.5, -0.5*h);
    glEnd();
    //paintRectangle(w, 0.5);
    //glRotatef(90, 0,1,0);
    //glTranslatef(0.5*h,0,0.5*w);
    //paintRectangle(h,0.5);
    //glRotatef(90, 0,1,0);
    //glTranslatef(0.5*w,0,0.5*h);
    //paintRectangle(w, 0.5);
    //glRotatef(90, 0,1,0);
    //glTranslatef(0.5*h,0,0.5*w);
    //paintRectangle(h,0.5);
    //glTranslatef(-1.5*h,0,-1.5*w);
    //glTranslatef(h,0,w);
    //glRotatef(90,0,1,0);
    //glRotatef(90, 1,0,0);
    //glColor3f(0,0.5,1);
    //paintRectangle(w,h);
    //glRotatef(270,1,0,0);
    //glColor3f(0,0.1,1);
    //glTranslatef(0,0,0.5*h);
    glBegin(GL_QUADS);
        glNormal3f(0.5-0.75*a,0,-0.5*w+0.5*a);
        glVertex3f(0.5*a,0,0.5*h-0.5);
        glVertex3f(0.5*a,0.5,0.5*h-0.5);
        glVertex3f(0.5*w,0.5,0.5*h-0.75*a);
        glVertex3f(0.5*w,0,0.5*h-0.75*a);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(0.5*a,0.5,0.5*h-0.5);
        glVertex3f(0.5*a,0.5,0.5*h);
        glVertex3f(0.5*w,0.5,0.5*h);
        glVertex3f(0.5*w,0.5,0.5*h-0.75*a);
     glEnd();
     glBegin(GL_QUADS);
        glNormal3f(0.5-0.75*a,0,0.5*w+0.5*a);
         glVertex3f(-0.5*a,0,0.5*h-0.5);
         glVertex3f(-0.5*a,0.5,0.5*h-0.5);
         glVertex3f(-0.5*w,0.5,0.5*h-0.75*a);
         glVertex3f(-0.5*w,0,0.5*h-0.75*a);
     glEnd();
     glBegin(GL_QUADS);
         glNormal3f(0,1,0);
         glVertex3f(-0.5*a,0.5,0.5*h-0.5);
         glVertex3f(-0.5*a,0.5,0.5*h);
         glVertex3f(-0.5*w,0.5,0.5*h);
         glVertex3f(-0.5*w,0.5,0.5*h-0.75*a);
     glEnd();
     //glTranslatef(0,0,-0.5*h);
}

void OGLWidget::paintRectangle(float r, float h, int alpha){
    glBegin(GL_QUADS);
        glNormal3f(cos(alpha/2), 0, sin(alpha/2));
        glVertex3f(r,0,0);
        glVertex3f(cos(alpha)*r, 0, sin(alpha)*r);
        glVertex3f(r,h,0);
        glVertex3f(cos(alpha)*r, h, sin(alpha)*r);
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

void OGLWidget::paintSquare(float s){
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(s,0,0);
        glVertex3f(s,s,0);
        glVertex3f(0,s,0);
    glEnd();
}


void OGLWidget::paintCube(float s){
        paintSquare(s);
        glRotatef(90,0,1,0);
        paintSquare(s);
        glTranslatef(s,0,s);
        glRotatef(90,0,1,0);
        paintSquare(s);
        glRotatef(90,0,1,0);
        paintSquare(s);
        glRotatef(180,0,1,0);
        glTranslatef(-s,0,-s);
        glRotatef(270,0,1,0);
        glRotatef(90,1,0,0);
        glTranslatef(0,-s,0);
        paintSquare(s);
        glTranslatef(0,0,-s);
        paintSquare(s);
        glTranslatef(0,s,s);
        glRotatef(270,1,0,0);
}
