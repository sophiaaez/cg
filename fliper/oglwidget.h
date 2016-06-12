#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <cmath>
#include<vector>
#include<QTimer>
#include<QObject>

class OGLWidget : public QOpenGLWidget,
                  protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();




    // Used to move sphere by mouse
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    // Used to rotate object by keys (requires focus for OpenGL widget)
    void keyPressEvent(QKeyEvent *event);

signals:
    // Emitted whenever a change of rotation angles is requested (e.g. by mouse or key)
    void changeRotation( int dx, int dy, int dz );

public slots:
    void stepAnimation();
    // Set the rotation angles
    void setRotX( int newrx );
    void setRotY( int newry );
    void setRotZ( int newrz );

    // Set zoom factor
    void setZoom( int newzoom );

    // Set unfolding position
    void setUnfold( int newunfold );

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    QTimer* animtimer;
    int animstep;
    double dx;
    double dy;
    double dxN;
    double dyN;
    std::vector<float> vX;
    std::vector<float> vY;
    double ox, oz, vx, vz, ax, az;
    boolean perspective;
    double cy_x, cy_z, cu_x, cu_z;
    boolean done;
    boolean up;

protected:
    int rotx;       // Rotation angles (0..360)
    int roty;
    int rotz;
    int zoom;       // Zoom factor (0..200, 100 for 1:1)
    int unfold;     // Unfolding position (0..100, 0 closed
    int faa;
    int punkte;
    boolean drawable;
    QPoint lastpos; // Last position of mouse pressed, used for dragging
    boolean slowmo;
    double speed;
    void drawSphere(double r, int lats, int longs);
    void paintTable(float w, float h, float a);
    void paintRectangle(float w, float h);
    void paintRectangle(float w, float h, int alpha);
    void paintCylinder(float r, float h);
    void paintCircle(float r, int alpha, int y);
    void paintTriangle(float r, int alpha);
    void paintFlipperArm(float w, float l, float h);
    void paintSquare(float s);
    void paintCube(float s);
    boolean aufStrecke(float xt, float zt);
    boolean closeTo(int i, int j);
};


#endif // OGLWIDGET_H
