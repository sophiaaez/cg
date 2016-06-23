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
    ox, oz, vx, vz, ax, az, cu_x, cu_z, cy_x, cy_z = 0;
    oz =-6;
    perspective = true;
    done = false;
    up = false;
    faa = 0; //Flipper arm alpha
    fad = 0; //Flipper arm direction/richtung (-1 = nach unten zurück, +1 nach oben)
    punkte = 0;
    wandx = 3;
    wandz = -4;
    wandr = 0.1;//Richtung der Wand für links - und für rechts + werte))
    cy_just_hit = false;
    schwerkraft = true;
    light = 0;
}

OGLWidget::~OGLWidget()
{    drawable = true;

}

void OGLWidget::stepAnimation()
{
    animstep++;    // Increase animation steps
    update();      // Trigger redraw of scene with paintGL
}


void OGLWidget::setCubeX(int newrx)
{
    if(!done){
        cu_x = 0.1* newrx;
        update();
    }

}

void OGLWidget::setCubeZ(int newry)
{
    if(!done){
        cu_z = 0.1* newry;
        update();
    }

}

void OGLWidget::setCylinderX(int newrz)
{
    if(!done){
        cy_x = 0.1*newrz;
        update();
    }
}

void OGLWidget::setCylinderZ(int newrz)
{
    if(!done){
        cy_z = 0.1*newrz;
        update();
    }

}

void OGLWidget::setStart(bool bla){
    if(!done){
        done = true;
        vz = 1;
    }
    update();
}

void OGLWidget::setFlip(bool bla){
    if(!up){
        up = true;
        fad = +1;
        //positionsaenderung fuer flipperarm
    }
    update();
}

void OGLWidget::setPerspective(bool bla){
    perspective = !perspective;
    update();
}
void OGLWidget::setLight(int newlight){
    light = newlight;
    update();
}


void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();


    // Use depth testing and the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Calculate color for each pixel fragment
    glShadeModel(GL_SMOOTH);

    // Enable lighting in scene
    glEnable(GL_LIGHTING);

    // Set position of light source
    float light_pos[] = { 10.f, 5.f, 10.f, 0.f };
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos );

    // Set color for this light source
    // (We are only specifying a diffuse light source)
    float light_diffuse[] = { .8f, .8f, .8f, 1.f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse );

    // Turn on this light
    glEnable(GL_LIGHT1);

    // Use the color of an object for light calculation
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_COLOR_MATERIAL);
}

void OGLWidget::paintGL()
{
    double a = animstep;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if(perspective){
        glOrtho(-15,15,-15,15,-100,100);
        glRotatef(60,1,0,0);
    }else{
        glOrtho(-10,10,-10,10,-100,100);
    }
    // Change light position
    float light_pos[] = { 10.f * cosf(light*M_PI/180.f),
                           5.f,
                          10.f * sinf(light*M_PI/180.f), 0.f };
    glLightfv(GL_LIGHT1, GL_POSITION,  light_pos);

    // Apply scaling
    float scale = zoom/100.0;
    glScalef( scale, scale, scale ); // Scale along all axis

    glRotatef(30,1,0,0);
    paintTable(10,14, 3);
    glTranslatef(7,0,-14);
    glColor3f(255,255,255);
    paintScore(punkte);
    glTranslatef(-7,0,14);
    glTranslatef(1,0,7);
    glRotatef(270,0,1,0);
    if(up){
        if(fad > 0){ //arm nach oben
            if(faa > -50){
                faa -= 2;
            }else{
                fad = -1;
            }
        }else{
            if(faa < 0){
                faa += 2;
            }else{
                up = false;
            }
        }
    }
    glRotatef(faa, 0,1,0);
    paintFlipperArm(1,3,1);
    glRotatef(-faa,0,1,0);
    glRotatef(90,0,1,0);
    glTranslatef(-1,0,-7);
    glTranslatef(-3.5,0.1,-5.5);
    glColor3f(255,0,150);
    paintCircle(1,1);
    glTranslatef(3.5,-0.1,5.5);
    if(done){
        if(wandr > 0 && wandx+wandr < 4){
            wandx += wandr;
        }else if(wandr > 0 &&  wandx+wandr >= 4){
            wandr = -wandr;
        }else if(wandr < 0 &&  wandx+wandr >-4){
            wandx += wandr;
        }else if(wandr < 0 &&  wandx+wandr <= -4){
            wandr = -wandr;
        }
    }

    paintWall(0.2,2,wandx, wandz);
    glColor3f(0,0.75,0.75);
    glTranslatef(cy_x, 0, cy_z);
    paintCylinder(0.6, 1);
    glTranslatef(-cy_x, 0, -cy_z);
    glColor3f(0.1,0.3,0.5);
    glTranslatef(cu_x, 0, cu_z);
    paintCube(1);
    glTranslatef(-cu_x, 0, -cu_z);

    glPushMatrix();

    glTranslatef(ox, 0, oz);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.5,0,0.5);
    paintSphere(a,35,35);
    glScalef(2,2,2);
    glTranslatef(-ox,0,-oz);
    glPopMatrix();

    if(done){
        double laenge = sqrt((vx*vx + 0*0 + vz*vz));
        if(laenge != 0){//normalisieren
            vx = 1/laenge *vx;
            vz = 1/laenge *vz;
        }

        if(-4.5 < ox+vx*0.1 && ox+vx*0.1 <4.5){ //rechts oder links bande

        }else{
            vx = -vx;
            schwerkraft = true;
            //ax = -ax;
        }
        if(oz+vz*0.1 <6.5){

        }else{ //unten
            vz = 0;
            az = 0;
            schwerkraft = false;
        }
        if(-6.5 < oz+vz*0.1){

        }else{//oben
            vz = -vz;
            schwerkraft = true;
        }

        //Zusammenstoß mit Cube
        double abstandcx = ox - cu_x;
        double abstandcz = oz - cu_z;
        if(-1 <= abstandcx && abstandcx <= 1 && -1.6 <= abstandcz && abstandcz <= -1.5){//oben
            std::cout<<"würfel oben"<<std::endl;
            double alpha = acos((vx*0+vz*(-1))/sqrt(vx*vx+vz*vz)*(1));
            if(alpha >= 3.14){
                alpha = alpha - M_PI;
            }
            if(alpha >= 0.5*M_PI){
                alpha = alpha - 0.5*M_PI;
            }
            double vxneu = cos(alpha)*0 - sin(alpha)*(-1);
            double vzneu = sin(alpha)*0 + cos(alpha)*(-1);
            vx = vxneu;
            vz = vzneu;
        } else if(-1 <= abstandcx && abstandcx <= 1 && -0.5 <= abstandcz && abstandcz <= 0.5){//unten
            std::cout<<"würfel unten"<<std::endl;
            double alpha = acos((vx*0+vz*(1))/sqrt(vx*vx+vz*vz)*1);
            if(alpha >= 3.14){
                alpha = alpha - M_PI;
            }
            if(alpha >= 0.5*M_PI){
                alpha = alpha - 0.5*M_PI;
            }
            double vxneu = cos(alpha)*0-sin(alpha)*(1);
            double vzneu = sin(alpha)*0+cos(alpha)*(1);
            vx = vxneu;
            vz = vzneu;
        }else if(-1.6 <= abstandcx && abstandcx <= -1.5 && -1 <= abstandcz && abstandcz <= 1){//links
            std::cout<<"würfel links"<<std::endl;
            double alpha = acos((-vx)/sqrt(vx*vx+vz*vz)*1);
            if(alpha >= 3.14){
                alpha = alpha - M_PI;
            }
            if(alpha >= 0.5*M_PI){
                alpha = alpha - 0.5*M_PI;
            }
            double vxneu = cos(alpha)*(-1)-sin(alpha)*0;
            double vzneu = sin(alpha)*(-1)+cos(alpha)*0;
            vx = vxneu;
            vz = vzneu;
        }else if(1.5 <= abstandcx && abstandcx <= 1.6 && -1 <= abstandcz && abstandcz <= 1){//rechts
            std::cout<<"würfel rechts"<<std::endl;
            double alpha = acos((vx*1 + vz*0)/sqrt(vx*vx+vz*vz)*1);
            if(alpha >= 3.14){
                alpha = alpha - M_PI;
            }
            if(alpha >= 0.5*M_PI){
                alpha = alpha - 0.5*M_PI;
            }
            double vxneu = cos(alpha)*(1)-sin(alpha)*0;
            double vzneu = sin(alpha)*(1)+cos(alpha)*0;
            vx = vxneu;
            vz = vzneu;
        }
        //Zusammenstoß mit Cube-ECKE
        //ecke rechts oben
        double axor = ox-(cu_x+1); //abstandxeckeobenrechts
        double azor = oz-(cu_z-1); //abstandzeckeobenrechts
        double daor = sqrt(axor*axor+azor*azor); // diagonalerabstandobenrechts
        if(daor <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = -vx;
            vz = -vz;
        }
        //ecke links oben
        double axol = ox-(cu_x); //abstandxeckeobenlinks
        double azol = oz-(cu_z-1); //abstandzeckeobenlinks
        double daol = sqrt(axol*axol+azol*azol); // diagonalerabstandobenlinks
        if(daol <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = -vx;
            vz = -vz;
        }
        //ecke links unten
        double axul = ox-(cu_x); //abstandxeckeuntenlinks
        double azul = oz-(cu_z); //abstandzeckeuntenlinks
        double daul = sqrt(axul*axul+azul*azul); // diagonalerabstanduntenlinks
        if(daul <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = -vx;
            vz = -vz;
        }
        //ecke rechts unten
        double axur = ox-(cu_x+1); //abstandxeckeobenrechts
        double azur = oz-(cu_z); //abstandzeckeobenrechts
        double daur = sqrt(axur*axur+azur*azur); // diagonalerabstandobenrechts
        if(daur <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = -vx;
            vz = -vz;
        }
         //Zusammenstoß mit Cylinder
        double abstandx = cy_x - ox+vx*0.1;
        double abstandz = cy_z - oz+vz*0.1;
        double abstand = sqrt(abstandx*abstandx + abstandz*abstandz);
        if(abstand > 1.2){
            cy_just_hit = false;
        }
        if(abstand <= 1.2 && !cy_just_hit){
            std::cout<<"zylinder"<<std::endl;
            cy_just_hit = true;
            schwerkraft = true;
            double alpha = -135.0 * M_PI/180;
            double vxneu = cos(alpha)*vx-sin(alpha)*vz;
            double vzneu = sin(alpha)*vx+cos(alpha)*vz;
            int vxxx = (vxneu*1000);
            vxneu = vxxx/1000.0;
            int vzzz = (vzneu*1000);
            vzneu = vzzz/1000.0;
            vx = vxneu;
            vz = vzneu;

        }

        //Punkte durch pinken Kreis
       abstandx = (-3.5) - ox+vx*0.1;
       abstandz = (-5.5) - oz+vz*0.1;
       if(abstandx >= -1 && abstandx <= 1 && abstandz >= -1 && abstandz <= 1){
            punkte += 1;
       }

        //bande rechts
        double brxt = ((ox+vx*0.1)+0.5 - 0.5*3)/(0.5*10 - 0.5*3); //bande rechts x t-wert
        double brzt = ((oz+vz*0.1)+0.5 - (0.5*14 - 0.5))/((0.5*14-0.75*3) - (0.5*14-0.5)); //bande rechts z t-wert
        if(aufStrecke(brxt, brzt)){
            schwerkraft = false;
            std::cout<<"bande rechts"<<std::endl;
            ox = ox+vx*0.1;
            oz = oz+vz*0.1;
            vx = -(0.5*10 - 0.5*3)*0.5;
            vz = -((0.5*14-0.75*3)-(0.5*14-0.5))*0.5;
            laenge = sqrt((vx*vx + 0*0 + vz*vz));
            if(laenge != 0){//normalisieren
                vx = 1/laenge *vx;
                vz = 1/laenge *vz;
            }
        }

        //bande links
        double blxt = ((ox+vx*0.1)-0.5 + 0.5*3)/(-0.5*10 + 0.5*3); //bande links x t-wert
        double blzt = ((oz+vz*0.1)+0.5 - (0.5*14 - 0.5))/((0.5*14-0.75*3) - (0.5*14-0.5)); //bande links z t-wert
        if(aufStrecke(blxt, blzt)){
            std::cout<<"bande links"<<std::endl;
            schwerkraft = false;
            ox = ox+vx*0.1;
            oz = oz+vz*0.1;
            vx = -(-0.5*10 + 0.5*3)*0.5;
            vz = -((0.5*14-0.75*3) - (0.5*14-0.5))*0.5;
            laenge = sqrt((vx*vx + 0*0 + vz*vz));
            if(laenge != 0){//normalisieren
                vx = 1/laenge *vx;
                vz = 1/laenge *vz;
            }
            std::cout<<"bande links getroffen "<<blxt<< " "<< blzt<<vx << " " <<vz<<std::endl;
        }

        //flipper arm
        //normale zum flipperarm
        double nx = (-6)*cos(90+faa)-0.5*sin(90+faa);
        double nz = (-6)*sin(90+faa)+0.5*cos(90+faa);
        laenge = sqrt((nx*nx + 0*0 + nz*nz));
        if(laenge != 0){ //normalisieren
            nx = 1/laenge *nx;
            nz = 1/laenge *nz;
        }
        double d = (nx*(ox+vx*0.1)+nz*(oz+vz*0.1)+7)/laenge;
        double paxt = ((ox+vx*0.1) - 3)/((-6)*cos(faa)-0.5*sin(faa));
        double pazt = ((oz+vz*0.1)+0.5 - 6.5)/((-6)*sin(faa)+0.5*cos(faa));
        if(d <= 0.2 && paxt <= 1 && paxt >= 0 && pazt <= 1 && pazt >= 0 ){
            std::cout<<"flipperarm"<<std::endl;
            schwerkraft = false;
            if(!up){
                //runter rollen
                vx = (-6)*cos(faa)-0.5*sin(faa);
                vz = (-6)*sin(faa)+0.5*cos(faa);
            }else{
                //abprallen
                vx = nx;
                vz = nz;
                schwerkraft = true;
            }
            laenge = sqrt((vx*vx + 0*0 + vz*vz));
            if(laenge != 0){ //normalisieren
                vx = 1/laenge *vx;
                vz = 1/laenge *vz;
            }
        }

        //kollision mit dem kleinen teil der linken bande
        if((ox+vx*0.1)-0.5 <= -0.5*3 && (oz+vz*0.1) > ((0.5*14)-1)){
            schwerkraft = false;
            std::cout<<"kleiner teil der linken bande"<<std::endl;
            vx = 0;
            vz = 0;
        }
        if(up && (ox)-0.6 <= -0.5*3 && (oz)+0.5 > ((0.5*14)-1)){
            std::cout<<"bla"<<std::endl;
            vx = 0;
            vz = -1;
        }
        //wand
        double abstandzwand = oz - wandz;
        double abstandxwand = ox - wandx;
        abstandxwand = sqrt(abstandxwand*abstandxwand);
        if(abstandzwand <= -0.6 && -0.65 <= abstandzwand && abstandxwand <= 0.5){
            schwerkraft = false;
            std::cout<<"wand von oben"<<std::endl;
            vz = 0;
            vx = wandr;
        }
        if(abstandzwand <= 0.6 && 0.65 <= abstandzwand && abstandxwand <= 0.5){ //zusammenstoß von unten
            std::cout<<"wand von unten"<<std::endl;
            vz = 1;
        }


        laenge = sqrt((vx*vx + 0*0 + vz*vz));
        if(laenge != 0){//normalisieren
                   vx = 1/laenge *vx;
                   vz = 1/laenge *vz;
        }
        ox = ox + vx * 0.1;
        oz = oz + vz * 0.1;
        if(schwerkraft){
            vz = vz +0.05;
        }

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
    if(!done){
        vz = 1;
        az = 0.01;
    }
    done = true;
}



void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    // This is the delta we want to use for rotating
    const int keyDelta = 10;

    switch(event->key())
    {
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
                fad = +1;
                //positionsaenderung fuer flipperarm
            }
        break;
        case Qt::Key_M:
            std::cout<<"ETWA HIER! MARKER!!"<<std::endl;
        break;
        case Qt::Key_P:
            perspective = !perspective;
            break;
        // All other will be ignored
        default:
            break;
    }
}

void OGLWidget::paintSphere(double r, int b, int l) {
    //r = radius, b = breitengrade, l = laengengrade
         int i, j;

        for(i = 0; i <= b; i++) {
            double lat0 = M_PI * (-0.5 + (double) (i - 1) / b);
            double z0  = sin(lat0);
            double zr0 =  cos(lat0);

            double lat1 = M_PI * (-0.5 + (double) i / b);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

         glBegin(GL_QUAD_STRIP);
         for(j = 0; j <= l; j++) {
             double lng = 2 * M_PI * (double) (j - 1) / l;
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
    paintCircle(r, alpha);
    glTranslatef(0,h,0);
    paintCircle(r, alpha);
    glTranslatef(0,-h,0);
    for(int i = 0; i < amount; i++){
        paintTriangle(r, alpha);
        paintRectangle(r, h, alpha);
        glRotatef(alpha * 1.0, 0.0,1.0,0.0);
    }
}

void OGLWidget::paintCircle(float r, int alpha){ //Wahrscheinlicher fehler alpha in bogen oder gradmaß, das was halt falsch ists
    int amount = 360/alpha;
    for(int i = 0; i < amount; i++){
        paintTriangle(r, alpha);
        glRotatef(alpha, 0.0,1.0,0.0);
    }
}

void OGLWidget::paintTriangle(float r, int alpha){
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(cos(0)*r,0,0);
        glVertex3f(cos(alpha)*r, 0, sin(alpha)*r);
    glEnd();
}

void OGLWidget::paintFlipperArm(float w, float l, float h){
    float r = w/2;
    glColor3f(0.7,0,0.5);
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
    glColor3f(0.5,0.2,1);
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

void OGLWidget::paintScore(int i){
    int ziffer1, ziffer2, ziffer3, ziffer4;
    glColor3f(255,255,255);
    //wir gehen davon aus, dass der maximale score 9999 ist,
    //das wären im besten fall einige dutzend minuten spielzeit
    int ziffer = i % 10;
    ziffer4 = ziffer;
    paintFigure(ziffer4);
    glTranslatef(-2,0,0);
    i = i - ziffer;
    ziffer = i % 100;
    ziffer = ziffer/10;
    ziffer3 = ziffer;
    paintFigure(ziffer3);
    glTranslatef(-2,0,0);
    i = i - ziffer;
    ziffer = i % 1000;
    ziffer = ziffer/100;
    ziffer2 = ziffer;
    paintFigure(ziffer2);
    glTranslatef(-2,0,0);
    i = i - ziffer;
    ziffer = i % 10000;
    ziffer = ziffer/1000;
    ziffer1 = ziffer;
    paintFigure(ziffer1);
    glTranslatef(6,0,0);
}

void OGLWidget::paintLine(double h, double w){
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(w,0,0);
        glVertex3f(w,0,h);
        glVertex3f(0,0,h);
    glEnd();
}

void OGLWidget::paintFigure(int i){
    switch(i){
    case 0:
        paintLine(4,0.2);
        glTranslatef(1,0,0);
        paintLine(4,0.2);
        glTranslatef(-1,0,0);
        paintLine(0.2,1);
        glTranslatef(0,0,4);
        paintLine(0.2,1);
        glTranslatef(0,0,-4);
        break;
    case 1:
        glTranslatef(1,0,0);
        paintLine(4,0.2);
        glTranslatef(-1,0,0);
        break;
    case 2:
        paintLine(0.2,1);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        paintLine(2,0.2);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        glTranslatef(0,0,-4);
        glTranslatef(1,0,0);
        paintLine(2,0.2);
        glTranslatef(-1,0,0);
        break;
    case 3:
        paintFigure(1);
        paintLine(0.2,1);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        glTranslatef(0,0,-4);
        break;
    case 4:
        paintFigure(1);
        paintLine(2,0.2);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        glTranslatef(0,0,-2);
        break;
    case 5:
        glRotatef(180,1,0,0);
        glTranslatef(0,0,-4);
        paintFigure(2);
        glTranslatef(0,0,4);
        glRotatef(-180,1,0,0);
        break;
    case 6:
        paintFigure(5);
        glTranslatef(0,0,2);
        paintLine(2,0.2);
        glTranslatef(0,0,-2);
        break;
    case 7:
        paintFigure(1);
        paintLine(0.2, 1);
        break;
    case 8:
        paintFigure(0);
        glTranslatef(0,0,2);
        paintLine(0.2,1);
        glTranslatef(0,0,-2);
        break;
    case 9:
        glRotatef(180,0,1,0);
        glTranslatef(-1,0,-4);
        paintFigure(6);
        glTranslatef(1,0,4);
        glRotatef(-180,0,1,0);
        break;
    }
}
void OGLWidget::paintWall(double h, double w, float x, float z){
    glTranslatef(x,0,z);
    glColor3f(222,42,255);
    glBegin(GL_QUADS); //unten
        glNormal3f(0,1,0);
        glVertex3f(-0.5*w,0,-0.5*h);
        glVertex3f(0.5*w,0,-0.5*h);
        glVertex3f(0.5*w,0,0.5*h);
        glVertex3f(-0.5*w,0,0.5*h);
    glEnd();
    glBegin(GL_QUADS); //oben
        glColor3f(0,165,158);
        glNormal3f(0,1,0);
        glVertex3f(-0.5*w,0.5,-0.5*h);
        glVertex3f(0.5*w,0.5,-0.5*h);
        glVertex3f(0.5*w,0.5,0.5*h);
        glVertex3f(-0.5*w,0.5,0.5*h);
    glEnd();
    glBegin(GL_QUADS); //vorne
        glNormal3f(0,0,1);
        glVertex3f(-0.5*w,0,0.5*h);
        glVertex3f(0.5*w,0,0.5*h);
        glVertex3f(0.5*w,0.5,0.5*h);
        glVertex3f(-0.5*w,0.5,0.5*h);
    glEnd();
    glBegin(GL_QUADS); //hinten
        glNormal3f(0,0,1);
        glVertex3f(-0.5*w,0,-0.5*h);
        glVertex3f(0.5*w,0,-0.5*h);
        glVertex3f(0.5*w,0.5,-0.5*h);
        glVertex3f(-0.5*w,0.5,-0.5*h);
    glEnd();
    glBegin(GL_QUADS); //links
        glNormal3f(1,0,0);
        glVertex3f(-0.5*w,0,0.5*h);
        glVertex3f(-0.5*w,0,-0.5*h);
        glVertex3f(-0.5*w,0.5,-0.5*h);
        glVertex3f(-0.5*w,0.5,0.5*h);
    glEnd();
    glBegin(GL_QUADS); //rechts
        glNormal3f(1,0,0);
        glVertex3f(0.5*w,0,0.5*h);
        glVertex3f(0.5*w,0,-0.5*h);
        glVertex3f(0.5*w,0.5,-0.5*h);
        glVertex3f(0.5*w,0.5,0.5*h);
    glEnd();
    glTranslatef(-x,0,-z);
}
