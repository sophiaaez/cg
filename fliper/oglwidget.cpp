#include "oglwidget.h"
#include <QOpenGLTexture>
#include "iostream"
#include <QMediaPlayer>
#include <cmath>
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
    cu_just_hit = false;
    fl_just_hit = false;
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

void OGLWidget::setCubeA(int newa){
    if(!done){
        cu_a = newa;
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    //licht aktivieren
    glEnable(GL_LIGHTING);

    //position der lichtquelle setzen
    float light_pos[] = { 10.f, 5.f, 10.f, 0.f };
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos );
    float light_diffuse[] = { .8f, .8f, .8f, 1.f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse );

    //licht anschalten
    glEnable(GL_LIGHT1);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_COLOR_MATERIAL);
    //Texturen aktivieren
    glEnable(GL_TEXTURE_2D);

    //musik abspielen //um Spiel schneller zu machen, diesen Teil auskommentieren
    /**QMediaPlayer * musicplayer = new QMediaPlayer();
    musicplayer->setMedia(QUrl("qrc:/sounds/kittenmusic.mp3"));
    musicplayer->play();**/
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

    float light_pos[] = { 10.f * cosf(light*M_PI/180.f),
                           5.f,
                          10.f * sinf(light*M_PI/180.f), 0.f };
    glLightfv(GL_LIGHT1, GL_POSITION,  light_pos);

    float scale = zoom/100.0;
    glScalef( scale, scale, scale ); // Scale along all axis

    //Fläche um 30° drehen (damit es auch aussieht wie ein Flippertisch)
    glRotatef(30,1,0,0);
    paintTable(10,14, 3);
    glTranslatef(7,0,-14);
    glColor3f(255,255,255);
    paintScore(punkte);
    glTranslatef(-7,0,14);
    glTranslatef(1,0,7);
    glRotatef(270,0,1,0);
    if(up){ //wenn up true ist (leertaste oder flip gedrückt)
        if(fad > 0){ //arm nach oben
            if(faa > -50){ //arm dreht sich bis um 50° bzw -50°
                faa -= 2; //drehwinkel verändern
            }else{
                fad = -1; //richtung umkehren
            }
        }else{
            if(faa < 0){
                faa += 2; //drehwinkel verändern immer um 2°
            }else{
                up = false; //wenn faa wieder = 0, dann fertig gedreht
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
    if(done){ //wenn das spiel angefangen hat
        //bewegt sich die wand in x richtung zwischen 0 und 4
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
    //verschiebung von wuerfel und zylinder
    paintWall(0.2,2,wandx, wandz);
    glColor3f(0,0.75,0.75);
    glTranslatef(cy_x, 0, cy_z);
    paintCylinder(0.6, 1);
    glTranslatef(-cy_x, 0, -cy_z);
    glColor3f(0.1,0.3,0.5);
    glTranslatef(cu_x, 0, cu_z);
    glRotatef(cu_a,0,1,0);
    paintCube(1);
    glRotatef(360-cu_a,0,1,0);
    glTranslatef(-cu_x, 0, -cu_z);

    glPushMatrix();
    //bewegung der kugel
    glTranslatef(ox, 0.5, oz);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.5,0,0.5);
    paintSphere(a,35,35);
    glScalef(2,2,2);
    glTranslatef(-ox,-0.5,-oz);
    glPopMatrix();

    if(done){//hier berechnung der neuen richtung/bewegungsverktoren der kugel
        double laenge = sqrt((vx*vx + 0*0 + vz*vz));
        if(laenge != 0){//normalisieren
            vx = 1/laenge *vx;
            vz = 1/laenge *vz;
        }

        if(-4.5 < ox+vx*0.1 && ox+vx*0.1 <4.5){

        }else{//rechte oder linke wand des tisches
            vx = -vx; //vereinfachte rechnung zum einfallswinkel = ausfallswinkel abprallen
            schwerkraft = true;
            if(ox <= -4.5){
                ox = -4.4;
            }else if(ox >= 4.5){
                ox = 4.4;
            }
        }
        if(oz+vz*0.1 <6.5){

        }else{ //untere wand des tisches
            vz = 0;
            az = 0;
            schwerkraft = false;
        }
        if(-6.5 < oz+vz*0.1){

        }else{//obere wand des tisches
            vz = -vz;
            schwerkraft = true;
        }

        //Zusammenstoß mit wuerfel
        double cnx, cnz, cnx2, cnz2;
        double alpha;
        //Radius des Umkreises
        double r = sqrt(0.5*0.5+0.5*0.5);
        //Ecke Rechts Oben
        double erox = cu_x+(cos((315-cu_a)*M_PI/180))*r;
        double eroz = cu_z+(sin((315-cu_a)*M_PI/180))*r;
        //Ecke Links Oben
        double elox = cu_x+(cos((225-cu_a)*M_PI/180))*r;
        double eloz = cu_z+(sin((225-cu_a)*M_PI/180))*r;
        //Ecke Links Unten
        double elux = cu_x+(cos((135-cu_a)*M_PI/180))*r;
        double eluz = cu_z+(sin((135-cu_a)*M_PI/180)*r);
        //Ecke Rechts Unten
        double erux = cu_x+(cos((45-cu_a)*M_PI/180))*r;
        double eruz = cu_z+(sin((45-cu_a)*M_PI/180))*r;
        //Abstand zur oberen Kante des Würfels
        double dob = pointLineDistance(erox, eroz, elox, eloz, ox, oz);
        //Abstand zur linken Kante des Würfels
        double dli = pointLineDistance(elox, eloz, elux, eluz, ox, oz);
        //Abstand zur unteren Kante des Würfels
        double dun = pointLineDistance(elux, eluz, erux, eruz, ox, oz);
        //Abstand zur linken Kante des Würfels
        double dre = pointLineDistance(erux, eruz, erox, eroz, ox, oz);
        //wuerfel kann erst wieder getroffen werden, wenn der abstand zu jeder gerade größerals 0.5 ist
        if(dob > 0.5 && dli > 0.5 && dun > 0.5 && dre > 0.5){
            cu_just_hit = false;
        }
        //oben
        //wenn der abstand zu oben kleiner ist als 0.25
        //und der Ball zwischen links und rechts liegt
        //-> der abstand zu links und rechts kleiner als 1 ist
        if(dob < 0.5 && dre <= 1 && dli <= 1 && !cu_just_hit){
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel oben"<<std::endl;
            //normaler Normalenvekor
            cnx = 0;
            cnz = -1;
            //gedrehter normalenvektor
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            //Berechnung des Winkels zwischen Normalenvektor und Einfallsvektor
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            //winkel korrektur, da wir den normalenvektor eventuell nach links oder rechts um den winkel rotieren müssen
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //ausfallsvektor ist der normalenvektor um den korrigierten winkel gedreht
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            //falls der winkel alpha den normalenvektor in die falsche richtung verschiebt,
            //so, dass der austrittswinkel etwa gleich der eintrittswinkel ist, obwohl der aufprallwinkel
            //nicht 0° war
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //links
        if(dli < 0.5 && dob <= 1 && dun <= 1){
            std::cout<<"würfel links"<<std::endl;
            schwerkraft = true;
            cu_just_hit = true;
            cnx = -1;
            cnz = 0;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }

            double vxneu = cos(alpha)*cnx - sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx + cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //unten
        if(dun < 0.5 && dli <= 1 && dre <= 1){
            std::cout<<"würfel unten"<<std::endl;
            schwerkraft = true;
            cu_just_hit = true;
            cnx = 0;
            cnz = 1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            double vxneu = cos(alpha)*cnx - sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx + cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //rechts
        if(dre < 0.5 && dob <= 1 && dun <= 1){
            std::cout<<"würfel rechts"<<std::endl;
            schwerkraft = true;
            cu_just_hit = true;
            cnx = 1;
            cnz = 0;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }


        //Referenzpunkt Ecke Rechts Oben
        double rerox = cu_x+(cos((315-cu_a)*M_PI/180)*(r+0.5));
        double reroz = cu_z+(sin((315-cu_a)*M_PI/180)*(r+0.5));
        //Referenzpunkt Ecke Links Oben
        double relox = cu_x+(cos((225-cu_a)*M_PI/180)*(r+0.5));
        double reloz = cu_z+(sin((225-cu_a)*M_PI/180)*(r+0.5));
        //Referenzpunkt Ecke Links Unten
        double relux = cu_x+(cos((135-cu_a)*M_PI/180)*(r+0.5));
        double reluz = cu_z+(sin((135-cu_a)*M_PI/180)*(r+0.5));
        //Referenzpunkt Ecke Rechts Unten
        double rerux = cu_x+(cos((45-cu_a)*M_PI/180)*(r+0.5));
        double reruz = cu_z+(sin((45-cu_a)*M_PI/180)*(r+0.5));

        //Abstand zur Ecke Oben Rechts
        double dero = sqrt((erox-ox)*(erox-ox)+(eroz-oz)*(eroz-oz));
        //Abstand zur Ecke Oben Links
        double delo = sqrt((elox-ox)*(elox-ox)+(eloz-oz)*(eloz-oz));
        //Abstand zur Ecke Unten Links
        double delu = sqrt((elux-ox)*(elux-ox)+(eluz-oz)*(eluz-oz));
        //Abstand zur Ecke Unten Rechts
        double deru = sqrt((erux-ox)*(erux-ox)+(eruz-oz)*(eruz-oz));

        //Abstand zum Referenzpunkt Oben Rechts
        double rdero = sqrt((rerox-ox)*(rerox-ox)+(reroz-oz)*(reroz-oz));
        //Abstand zum Referenzpunkt Oben Links
        double rdelo = sqrt((relox-ox)*(relox-ox)+(reloz-oz)*(reloz-oz));
        //Abstand zum Referenzpunkt Unten Links
        double rdelu = sqrt((relux-ox)*(relux-ox)+(reluz-oz)*(reluz-oz));
        //Abstand zum Referenzpunkt Unten Rechts
        double rderu = sqrt((rerux-ox)*(rerux-ox)+(reruz-oz)*(reruz-oz));

        //Ecke Oben Rechts
        if(dero <= 0.5 && rdero <= 0.5){
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel oben rechts"<<std::endl;
            cnx = 1;
            cnz = -1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            //winkel korrektur, da wir den normalenvektor eventuell nach links oder rechts um den winkel rotieren müssen
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //ausfallsvektor ist der normalenvektor um den korrigierten winkel gedreht
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            //falls der winkel alpha den normalenvektor in die falsche richtung verschiebt,
            //so, dass der austrittswinkel etwa gleich der eintrittswinkel ist, obwohl der aufprallwinkel
            //nicht 0° war
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //Ecke Oben Links
        if(delo <= 0.5 && rdelo <= 0.5){
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel oben links"<<std::endl;
            cnx = -1;
            cnz = -1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //Ecke Unten Links
        if(delu <= 0.5 && rdelu <= 0.5){
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel oben links"<<std::endl;
            cnx = -1;
            cnz = -1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }
        //Ecke Unten Links
        if(deru <= 0.5 && rderu <= 0.5){
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel unten links"<<std::endl;
            cnx = -1;
            cnz = -1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            vx = vxneu;
            vz = vzneu;
        }

         //Zusammenstoß mit Cylinder
        double abstandx = cy_x - ox+vx*0.1;
        double abstandz = cy_z - oz+vz*0.1;
        double abstand = sqrt(abstandx*abstandx + abstandz*abstandz);
        if(abstand > 1.2){ //erneute kollision erst wieder möglich, wenn abstand groß genug ist
            cy_just_hit = false;
        }
        if(abstand <= 1.2 && !cy_just_hit){ //wenn nicht gerade getroffen wurde und der abstand klein genug ist
            std::cout<<"zylinder"<<std::endl;
            cy_just_hit = true;
            schwerkraft = true;
            //Berechnung des Auftreffpunkts auf dem Cylinder
            double apcx = ox + vx*0.5;
            double apcz = oz + vz*0.5;
            //Berechnung des Vektors von Ortsvektor der Kugel zur Mitte des Zylinders
            double vkcx = ox - cy_x;
            double vkcz = oz - cy_z;
            //Berechnung des Normalenvektors im Auftreffpunkt
            double nax = apcx-cy_x;
            double naz = apcz-cy_z;
            //Berechnung des Einfallswinkels in relation zum Mittelpunkt des Zylinders
            double alpha = acos((vx*vkcx+vz*(vkcz))/(sqrt(vx*vx+vz*vz)*sqrt(vkcx*vkcx + vkcz*vkcz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //Drehen des Normalenvektors damit er der Aufallswinkel ist
            double vxneu = cos(alpha)*nax-sin(alpha)*naz;
            double vzneu = sin(alpha)*nax+cos(alpha)*naz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*nax - sin(2*M_PI-alpha)*naz;
                vzneu = sin(2*M_PI-alpha)*nax + cos(2*M_PI-alpha)*naz;
            }
            vx = vxneu;
            vz = vzneu;
        }

        //Punkte durch pinken Kreis
       abstandx = (-3.5) - ox+vx*0.1;
       abstandz = (-5.5) - oz+vz*0.1;
       abstand = sqrt(abstandx*abstandx + abstandz*abstandz);
       //der radius des pinken kreises ist 1, also wenn der abstand kleiner ist...
       if(abstand <= 1){
            punkte += 1;
       }

        //bande rechts
       //Linker Referenzpunkt der Bande
        double lrrx = 1.5;
        double lrrz = 6.5;
        //Rechter Referenzpunkt der Bande
        double rrrx = 5;
        double rrrz = 4.75;
        //Abstand zur Rechten Bande
        double drb = pointLineDistance(lrrx, lrrz, rrrx, rrrz, ox, oz);
        //wenn der Abstand zur Bande weniger gleich 0.5 (Radius der Kugel) ist
        //und die x-Koordinate der Kugel zwischen Anfang und Ende der Bande liegt
        if(drb <= 0.5 && lrrx <= ox && ox <= rrrx){
            schwerkraft = true;
            std::cout<<"bande rechts"<<std::endl;
            double nx = (0.5-0.75*3);
            double nz = -0.5*10+0.5*3;
            alpha = acos((vx*nx+vz*(nz))/(sqrt(vx*vx+vz*vz)*sqrt(nx*nx + nz*nz)));
            //winkel korrektur, da wir den normalenvektor eventuell nach links oder rechts um den winkel rotieren müssen
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //ausfallsvektor ist der normalenvektor um den korrigierten winkel gedreht
            double vxneu = cos(2*M_PI-alpha)*nx - sin(2*M_PI-alpha)*nz;
            double vzneu = sin(2*M_PI-alpha)*nx + cos(2*M_PI-alpha)*nz;
            vx = vxneu;
            vz = 0.5*vzneu;
        }

        //bande links
        //Linker Referenzpunkt der Bande
         double llrx = -5;
         double llrz = 4.75;
         //Rechter Referenzpunkt der Bande
         double rlrx = -1.5;
         double rlrz = 6.5;
         //Abstand zur Linken Bande
         double dlb = pointLineDistance(llrx, llrz, rlrx, rlrz, ox, oz);
         //wenn der Abstand zur Bande weniger gleich 0.5 (Radius der Kugel) ist
         //und die x-Koordinate der Kugel zwischen Anfang und Ende der Bande liegt
         if(dlb <= 0.5 && llrx <= ox && ox <= rlrx){
            schwerkraft = true;
            std::cout<<"bande links"<<std::endl;
            double nx = -(0.5-0.75*3);
            double nz = -0.5*10+0.5*3;
            alpha = acos((vx*nx+vz*(nz))/(sqrt(vx*vx+vz*vz)*sqrt(nx*nx + nz*nz)));
            //winkel korrektur, da wir den normalenvektor eventuell nach links oder rechts um den winkel rotieren müssen
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //ausfallsvektor ist der normalenvektor um den korrigierten winkel gedreht
            double vxneu = cos(alpha)*nx-sin(alpha)*nz;
            double vzneu = sin(alpha)*nx+cos(alpha)*nz;
            //falls der winkel alpha den normalenvektor in die falsche richtung verschiebt,
            //so, dass der austrittswinkel etwa gleich der eintrittswinkel ist, obwohl der aufprallwinkel
            //nicht 0° war
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*nx - sin(2*M_PI-alpha)*nz;
                vzneu = sin(2*M_PI-alpha)*nx + cos(2*M_PI-alpha)*nz;
            }
            vx = vxneu;
            vz = 0.5*vzneu;
        }

        //flipper arm
        //Dreh Referenzpunkt Flipperarm
        double drfx = +1;
        double drfz = 7;
        //Linker Referenzpunkt Flipperarm in Abhängigkeit zum Drehpunkt
        double lrfx = -2.5;
        double lrfz = 0;
        //Rechter Referenzpunkt Flipperarm in Abhängigkeit zum Drehpunkt
        double rrfx = +0.5;
        double rrfz = -0.5;
        //Berechnen der gedrehten absoluten Referenzpunkte
        double lrfxx = drfx + lrfx*cos(faa*M_PI/180)-lrfz*sin(faa*M_PI/180);
        double lrfzz = drfz - lrfx*sin(faa*M_PI/180)+lrfz*cos(faa*M_PI/180);
        lrfx = lrfxx;
        lrfz = lrfzz;
        double rrfxx = drfx + rrfx;
        double rrfzz = drfz + rrfz;
        rrfx = rrfxx;
        rrfz = rrfzz;
        //Berechnen des gedrehten Normalenvektors
        double nx = lrfx-rrfx;
        double nz = lrfz-rrfz;
        double nrx = -(cos(270*M_PI/180)*nx-sin(270*M_PI/180)*nz);
        double nrz = -(sin(270*M_PI/180)*nx+cos(270*M_PI/180)*nz);
        laenge = sqrt((nrx*nrx + 0*0 + nrz*nrz));
        //Abstand
        double df = pointLineDistance(lrfx, lrfz, rrfx, rrfz, ox, oz);
        if(df >= 1){
            fl_just_hit = false;
        }
        if(df <= 0.5&& !fl_just_hit){
            fl_just_hit = true;
            std::cout<<"flipperarm"<<std::endl;
            schwerkraft = false;
            if(!up){//wenn der flipperarm unten ist
                //kugel runter rollen
                std::cout<<"!up"<<std::endl;
                vx = (-6)*cos(faa*M_PI/180)-0.5*sin(faa*M_PI/180);
                vz = (-6)*sin(faa*M_PI/180)+0.5*cos(faa*M_PI/180);
            }else{
                //abprallen in richtung des aktuellen normalenvektors
                vx = nrx;
                vz = nrz;
                schwerkraft = true;
            }
            laenge = sqrt((vx*vx + 0*0 + vz*vz));
            if(laenge != 0){ //normalisieren
                vx = 1/laenge *vx;
                vz = 1/laenge *vz;
            }
            std::cout<<vx<<" "<<vz<<std::endl;
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
        //Ecke Wand Oben Rechts
        double eworx = wandx+1;
        double eworz = wandz-0.1;
        //Ecke Wand Oben Links
        double ewolx = wandx-1;
        double ewolz = wandz-0.1;
        //Ecke Wand Unten Links
        double ewulx = wandx-1;
        double ewulz = wandz+0.1;
        //Ecke Wand Unten Rechts
        double ewurx = wandx+1;
        double ewurz = wandz+0.1;

        //Abstand zu Oben
        double dao = pointLineDistance(eworx, eworz, ewolx, ewolz, ox, oz);
        //Abstand zu Unten
        double dau = pointLineDistance(ewulx, ewulz, ewurx, ewurz, ox, oz);
        //Abstand zu Rechts
        double dar = pointLineDistance(ewurx, ewurz, eworx, eworz, ox, oz);
        //Abstand zu Links
        double dal = pointLineDistance(ewolx, ewolz, ewulx, ewulz, ox, oz);
        //Zusammenstoß von Oben
        if(dao <= 0.5 && dar <= 2 && dal <= 2){
            schwerkraft = true;
            std::cout<<"wand von oben"<<std::endl;
            vz = -0.5;
        }
        //Unten
        if(dau <= 0.5 && dar <= 2 && dal <= 2){
            schwerkraft = true;
            std::cout<<"wand von unten"<<std::endl;
            vz = 1; //ball prallt nach unten ab, dabei bleibt die x-richtung bestehend, deshalb einfallswinkel = aufallswinkel
        }
        //Rechts
        if(dar <= 0.5 && dau <= 0.1 && dao <= 0.1){
            schwerkraft = true;
            std::cout<<"wand von rechts"<<std::endl;
            vx = 1;
        }
        //Links
        if(dal <= 0.5 && dau <= 0.1 && dao <= 0.1){
            schwerkraft = true;
            std::cout<<"wand von links"<<std::endl;
            vx = -1;
        }

        laenge = sqrt((vx*vx + 0*0 + vz*vz));
        if(laenge != 0){//normalisieren
                   vx = 1/laenge *vx;
                   vz = 1/laenge *vz;
        }
        //anwenden des richtungsvektors
        ox = ox + vx * 0.1;
        oz = oz + vz * 0.1;
        //falls gerade schwerkraft benötigt wird (bei dem fall, auf der wand oder auf dem Flipperarm zum beispiel nicht, sonst immer
        if(schwerkraft){
            vz = vz +0.05;
        }

    }
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

void OGLWidget::paintCylinder(float r, float h){ //r = radius, h = hoehe
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

void OGLWidget::paintCircle(float r, int alpha){
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

void OGLWidget::paintTable(float w, float h, float a){//w = weite, h = hoehe, a = weite des flipperArms
    glColor3f(2,4,7);
    //textur der katze auf die Tischplatte legen
    //um spiel schneller laufen zu lassen, diesen Teil auskommentieren
    /**QOpenGLTexture texture( QImage(":/pictures/kitten.png").mirrored() );
    texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture.setMagnificationFilter(QOpenGLTexture::Linear);
    float maxtexcoord = 1;
    texture.bind();**/
    glColor3f(0,0,1);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        //glTexCoord2f(0,0);
        glVertex3f(0.5*w, 0, 0.5*h);

        //glTexCoord2f(maxtexcoord,0);

        glVertex3f(-0.5*w, 0, 0.5*h);


        //glTexCoord2f(maxtexcoord,maxtexcoord);

        glVertex3f(-0.5*w, 0, -0.5*h);

        //glTexCoord2f(0,maxtexcoord);
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
    glBegin(GL_QUADS);//rechte Bande
        glNormal3f((0.5-0.75*a),0,-0.5*w+0.5*a);
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
     glBegin(GL_QUADS);//linke Bande
        glNormal3f(-(0.5-0.75*a),0,-0.5*w+0.5*a);
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
        glTranslatef(-0.5*s,0,+0.5*s);
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
        glTranslatef(+0.5*s,0,-0.5*s);
}

void OGLWidget::paintScore(int i){
    //wir gehen davon aus, dass der maximale score 9999 ist,
    //das wären im besten fall einige dutzend minuten spielzeit
    if(i < 9999){
        int ziffer1, ziffer2, ziffer3, ziffer4;
        glColor3f(255,255,255);
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
    }else{
        paintScore(9999);
    }

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

//malt eine Zahl auf den Bildschirm
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
    //h = hoehe, w = weite, x = x-koordinate, z = z-koordinate
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

double OGLWidget::pointLineDistance(double x1, double z1, double x2, double z2, double px, double pz){
    //zweite koordinate relativ zur ersten
    x2 -= x1;
    z2 -= z1;

    px -= x1;
    pz -= z1;

    double dot = px * x2 + pz * z2;
    double pro = dot * dot/ (x2 * x2 + z2 * z2);
    double length = px * px + pz * pz - pro;
    length = sqrt(length*length);
    return length;
}
