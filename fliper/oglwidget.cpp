#include "oglwidget.h"
#include <QOpenGLTexture>
#include "iostream"
#include <QMediaPlayer>
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
    QMediaPlayer * musicplayer = new QMediaPlayer();
    musicplayer->setMedia(QUrl("qrc:/sounds/kittenmusic.mp3"));
    musicplayer->play();
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

    // Apply scaling
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
    glTranslatef(ox, 0, oz);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.5,0,0.5);
    paintSphere(a,35,35);
    glScalef(2,2,2);
    glTranslatef(-ox,0,-oz);
    glPopMatrix();

    if(done){//hier berechnung der neuen richtung/bewegungsverktoren der kugel
        double laenge = sqrt((vx*vx + 0*0 + vz*vz));
        if(laenge != 0){//normalisieren
            vx = 1/laenge *vx;
            vz = 1/laenge *vz;
        }

        if(-4.5 < ox+vx*0.1 && ox+vx*0.1 <4.5){ //rechte oder linke wand des tisches

        }else{
            vx = -vx; //vereinfachte rechnung zum einfallswinkel = ausfallswinkel abprallen
            schwerkraft = true;
            //ax = -ax;
        }
        if(oz+vz*0.1 <6.5){ //untere wand des tisches

        }else{ //unten
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
        double abstandcx = ox - cu_x;
        double abstandcz = oz - cu_z;
        double abstandcx2 = cos(cu_a* M_PI/180)*abstandcx - sin(cu_a* M_PI/180)*abstandcz;
        double abstandcz2 = sin(cu_a* M_PI/180)*abstandcx + cos(cu_a* M_PI/180)*abstandcz;
        double cnx, cnz, cnx2, cnz2;
        double alpha;

        abstandcx = abstandcx2;
        abstandcz = abstandcz2;
        double abstandtotal = sqrt(abstandcx*abstandcx + abstandcz* abstandcz);
        if(abstandtotal >= 1 ){
            cu_just_hit = false;
        }

        //wenn der x-abstand zwischen -1 und 1 ist und der z abstand zwischen -1.6 und -1.5 ist
        if(-1 <= abstandcx && abstandcx <= 1 && -1.1 <= abstandcz && abstandcz <= -1.0 && !cu_just_hit){//oben
            schwerkraft = true;
            cu_just_hit = true;
            std::cout<<"würfel oben"<<std::endl;
            cnx = 0;
            cnz = -1;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz))); //winkel zwischen einfallsvektor und normalenvektor
            //winkel korrektur, da wir den normalenvektor eventuell nach links oder rechts um den winkel rotieren müssen
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //ausfallsvektor ist der normalenvektor um den korrigierten winkel gedreht
            double vxneu = cos(alpha)*cnx - sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx + cos(alpha)*cnz;
            //falls der winkel alpha den normalenvektor in die falsche richtung verschiebt,
            //so, dass der austrittswinkel etwa gleich der eintrittswinkel ist, obwohl der aufprallwinkel
            //nicht 0° war
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }


            vx = vxneu;
            vz = vzneu;

        } else if(-1 <= abstandcx && abstandcx <= 1 && 1.0 <= abstandcz && abstandcz <= 1.1 && !cu_just_hit){//unten
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
        }else if(-1.1 <= abstandcx && abstandcx <= -1.0 && -1 <= abstandcz && abstandcz <= 1 && !cu_just_hit){//links
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
        }else if(1.0 <= abstandcx && abstandcx <= 1.1 && -1 <= abstandcz && abstandcz <= 1 && !cu_just_hit){//rechts
            std::cout<<"würfel rechts"<<std::endl;
            schwerkraft = true;
            cu_just_hit = true;
            cnx = 1;
            cnz = 0;
            cnx2 = cos((360-cu_a)* M_PI/180)*cnx - sin((360-cu_a)* M_PI/180)*cnz;
            cnz2 = sin((360-cu_a)* M_PI/180)*cnx + cos((360-cu_a)* M_PI/180)*cnz;
            cnx = cnx2;
            cnz = cnz2;
            //std::cout<<cnx<<" "<<cnz<<std::endl;
            alpha = acos((vx*cnx+vz*(cnz))/(sqrt(vx*vx+vz*vz)*sqrt(cnx*cnx + cnz*cnz)));
            if(alpha >= 0.5*M_PI){
                alpha = (M_PI - alpha);
            }
            //std::cout<<cu_a<<" "<<alpha*180/M_PI<<std::endl;
            double vxneu = cos(alpha)*cnx-sin(alpha)*cnz;
            double vzneu = sin(alpha)*cnx+cos(alpha)*cnz;
            if(alpha != 0 && vx-0.01 <= -vxneu && -vxneu <= vx+0.01 && vz-0.01 <= -vzneu && -vzneu <= vz+0.01){
                vxneu = cos(2*M_PI-alpha)*cnx - sin(2*M_PI-alpha)*cnz;
                vzneu = sin(2*M_PI-alpha)*cnx + cos(2*M_PI-alpha)*cnz;
            }
            //std::cout<<vx<<" " <<vz << " "<<cnx<<" "<<cnz<<" "<< alpha*180/M_PI<<" "<<vxneu<<" "<<vzneu<<std::endl;

            vx = vxneu;
            vz = vzneu;
        }
        //Zusammenstoß mit wuerfel-ECKE
        //ecke rechts oben
        double axor = ox-(cu_x+0.5)+0.5; //abstandxeckeobenrechts
        double azor = oz-(cu_z-0.5)-0.5; //abstandzeckeobenrechts
        abstandcx2 = cos(cu_a* M_PI/180)*axor - sin(cu_a* M_PI/180)*azor;
        abstandcz2 = sin(cu_a* M_PI/180)*axor + cos(cu_a* M_PI/180)*azor;
        axor = abstandcx2;
        azor = abstandcz2;
        double daor = sqrt(axor*axor+azor*azor); // diagonalerabstandobenrechts
        if(daor <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            //kugel verschwindet
            vx = NAN;
            vz = NAN;
        }
        //ecke links oben
        double axol = ox-(cu_x-0.5)-0.5; //abstandxeckeobenlinks
        double azol = oz-(cu_z+0.5)-0.5; //abstandzeckeobenlinks
        abstandcx2 = cos(cu_a* M_PI/180)*axor - sin(cu_a* M_PI/180)*azor;
        abstandcz2 = sin(cu_a* M_PI/180)*axor + cos(cu_a* M_PI/180)*azor;
        axor = abstandcx2;
        azor = abstandcz2;
        double daol = sqrt(axol*axol+azol*azol); // diagonalerabstandobenlinks
        if(daol <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = NAN;
            vz = NAN;
        }
        //ecke links unten
        double axul = ox-(cu_x-0.5)-0.5; //abstandxeckeuntenlinks
        double azul = oz-(cu_z-0.5)+0.5; //abstandzeckeuntenlinks
        abstandcx2 = cos(cu_a* M_PI/180)*axor - sin(cu_a* M_PI/180)*azor;
        abstandcz2 = sin(cu_a* M_PI/180)*axor + cos(cu_a* M_PI/180)*azor;
        axor = abstandcx2;
        azor = abstandcz2;
        double daul = sqrt(axul*axul+azul*azul); // diagonalerabstanduntenlinks
        if(daul <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = NAN;
            vz = NAN;
        }
        //ecke rechts unten
        double axur = ox-(cu_x+0.5)+0.5; //abstandxeckeobenrechts
        double azur = oz-(cu_z+0.5)+0.5; //abstandzeckeobenrechts
        abstandcx2 = cos(cu_a* M_PI/180)*axor - sin(cu_a* M_PI/180)*azor;
        abstandcz2 = sin(cu_a* M_PI/180)*axor + cos(cu_a* M_PI/180)*azor;
        axor = abstandcx2;
        azor = abstandcz2;
        double daur = sqrt(axur*axur+azur*azur); // diagonalerabstandobenrechts
        if(daur <= 0.5){ //radius der kugel ist 0.5
            std::cout<<"Würfel ecke"<<std::endl;
            schwerkraft = true;
            vx = NAN;
            vz = NAN;
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
            double alpha = -135.0 * M_PI/180; //abprallwinkel immer 45° //umrechnung in radian notwendig!!
            double vxneu = cos(alpha)*vx-sin(alpha)*vz;
            double vzneu = sin(alpha)*vx+cos(alpha)*vz;
            //zu viele nachkommastellen vermeiden
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
       abstand = sqrt(abstandx*abstandx + abstandz*abstandz);
       //der radius des pinken kreises ist 1, also wenn der abstand kleiner ist...
       if(abstand <= 1){
            punkte += 1;
       }

        //bande rechts
        double brxt = ((ox+vx*0.1)+0.5 - 0.5*3)/(0.5*10 - 0.5*3); //bande rechts x t-wert
        double brzt = ((oz+vz*0.1)+0.5 - (0.5*14 - 0.5))/((0.5*14-0.75*3) - (0.5*14-0.5)); //bande rechts z t-wert
        if(aufStrecke(brxt, brzt)){ //prüft ob der nächste ortsvektor (mit berechnung durch richtungsvekoren) etwa auf der Strecke liegt
            schwerkraft = false;
            std::cout<<"bande rechts"<<std::endl;
            ox = ox+vx*0.1;
            oz = oz+vz*0.1;
            //richtungsvektoren werden an bande angepasst, da ein abprallen nicht wirklich statt findet
            //da die schwerkraft sie mehr nach unten zieht, als sie abprallen würden
            vx = -(0.5*10 - 0.5*3)*0.5;
            vz = -((0.5*14-0.75*3)-(0.5*14-0.5))*0.5;
            //normalisieren der vektoren
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
        //berechnung der aktuellen normalen zum flipperarm
        double nx = (-6)*cos((90+faa)*M_PI/180)-0.5*sin((90+faa)*M_PI/180);
        double nz = (-6)*sin((90+faa)*M_PI/180)+0.5*cos((90+faa)*M_PI/180);

        laenge = sqrt((nx*nx + 0*0 + nz*nz));
        if(laenge != 0){ //normalisieren
            nx = 1/laenge *nx;
            nz = 1/laenge *nz;
        }
        //abstand von flipperarm zur kugel
        double d = (nx*(ox+vx*0.1)+nz*(oz+vz*0.1)+7)/laenge;
        //paxt und pazt um zu prüfen, ob die kugel sich auch wirklich auf dem arm und nicht davor oder dahinterbefindet
        double paxt = ((ox+vx*0.1) - 3)/((-6)*cos(faa*M_PI/180)-0.5*sin(faa*M_PI/180));
        double pazt = ((oz+vz*0.1)+0.5 - 6.5)/((-6)*sin(faa*M_PI/180)+0.5*cos(faa*M_PI/180));
        //dazu müssen paxt und pazt jeweils zwischen 0 und 1 sein
        //der abstand muss weniger gleich 0.2 sein (nicht weniger gleich 0, weil es dann schon zu spät ist)
        if(d <= 0.2 && paxt <= 1 && paxt >= 0 && pazt <= 1 && pazt >= 0 ){
            std::cout<<"flipperarm"<<std::endl;
            schwerkraft = false;
            if(!up){//wenn der flipperarm unten ist
                //kugel runter rollen
                vx = (-6)*cos(faa*M_PI/180)-0.5*sin(faa*M_PI/180);
                vz = (-6)*sin(faa*M_PI/180)+0.5*cos(faa*M_PI/180);
            }else{
                //abprallen in richtung des aktuellen normalenvektors
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
        abstandxwand = sqrt(abstandxwand*abstandxwand); //bei dem x-abstand ist die richtung (- oder + unwichtig)
        //wenn der x-abstand zur wand kleiner gleich 0.5 und der z-abstand zwischen -0.6 und -0.65 ist
        if(abstandzwand <= -0.6 && -0.65 <= abstandzwand && abstandxwand <= 0.5){
            schwerkraft = false;
            std::cout<<"wand von oben"<<std::endl;
            vz = 0; //ball bleibt auf wand liegen
            vx = wandr;
        }
        if(abstandzwand <= 0.6 && 0.65 <= abstandzwand && abstandxwand <= 0.5){ //zusammenstoß von unten
            std::cout<<"wand von unten"<<std::endl;
            vz = 1; //ball prallt nach unten ab, dabei bleibt die x-richtung bestehend, deshalb einfallswinkel = aufallswinkel
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

//diese funktion kontrolliert, anhand der berechneten t-werte (oder lambda) durch das umstellen einer funktion,
//ob der punkt auf der strecke liegt
boolean OGLWidget::aufStrecke(float xt, float zt){
    boolean aufstrecke = false;
    //da die werte nur auf eine nachkommastellen übereinstimmen müssen, da wir sonst nie auf ein ergebnis kommen
    xt = (float)((int)(xt*100))/100;
    zt = (float)((int)(zt*100))/100;
    if(xt <= 1.1 && xt >= -0.1){ //die t-werte müssen zwischen 1 und 0 sein
        aufstrecke = true;
    }else{
        return false;
    }
    if(xt == zt){ //wenn die werte gleich sind
        return true && aufstrecke;
    }
    int xrest1 = (xt*10);
    int zrest1 = (zt*10);
    int xrest = (xt - (float)((int)(xt*10))/10)*100;
    int zrest = (zt - (float)((int)(zt*10))/10)*100;
    if(xrest1 == zrest1){ //wenn zweite stelle gleich ist
        return closeTo(xrest, zrest) && aufstrecke; //überprüfen auf mögliche rundung
    }
    if(closeTo(xrest1,zrest1)){ //sekundäreüberprüfung aufmögliche rundung
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

//überprüft ob die ziffern i und j möglich zu runden sind
//hier gelten nur ungefähre maßstäbe wie 1 ist in der nähe von 3
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
    glColor3f(1,1,1);
    //textur der katze auf die Tischplatte legen
    //um spiel schneller laufen zu lassen, diesen Teil auskommentieren
    QOpenGLTexture texture( QImage(":/pictures/kitten.png").mirrored() );
    texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture.setMagnificationFilter(QOpenGLTexture::Linear);
    float maxtexcoord = 1;
    texture.bind();

    glBegin(GL_QUADS);
     glNormal3f(0,1,0);
        glTexCoord2f(0,0);
        glVertex3f(0.5*w, 0, 0.5*h);

        glTexCoord2f(maxtexcoord,0);

        glVertex3f(-0.5*w, 0, 0.5*h);


        glTexCoord2f(maxtexcoord,maxtexcoord);

        glVertex3f(-0.5*w, 0, -0.5*h);

        glTexCoord2f(0,maxtexcoord);
        glVertex3f(0.5*w, 0, -0.5*h);
    glEnd();
    /**glBegin(GL_QUADS); //Tischplatte
        glNormal3f(0,1,0);
        glVertex3f(-0.5*w, 0, -0.5*h);
        glVertex3f(-0.5*w, 0, 0.5*h);
        glVertex3f(0.5*w, 0, 0.5*h);
        glVertex3f(0.5*w, 0, -0.5*h);
    glEnd();**/
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
