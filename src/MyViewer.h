#ifndef MYVIEWER_H
#define MYVIEWER_H

// Mesh stuff:
#include "Mesh.h"

//Points stuff
#include "points.h"

// Parsing:
#include "BasicIO.h"

// opengl and basic gl utilities:
#define GL_GLEXT_PROTOTYPES
#include <gl/openglincludeQtComp.h>
#include <GL/glext.h>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>
#include <QGLViewer/qglviewer.h>

#include <gl/GLUtilityMethods.h>

// Qt stuff:
#include <QFormLayout>
#include <QToolBar>
#include <QColorDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QInputDialog>
#include <QLineEdit>


#include "qt/QSmartAction.h"


class MyViewer : public QGLViewer , public QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

    Mesh mesh;
    Points points;
    bool drawing;
    bool pen_down;
    bool control_points;

    QWidget * controls;

public :

    MyViewer(QGLWidget * parent = NULL) : QGLViewer(parent) , QOpenGLFunctions_4_3_Core() {
    }



    void add_actions_to_toolBar(QToolBar *toolBar)
    {
        // Specify the actions :
        DetailedAction * open_mesh = new DetailedAction( QIcon("./icons/open.png") , "Open Mesh" , "Open Mesh" , this , this , SLOT(open_mesh()) );
        DetailedAction * save_mesh = new DetailedAction( QIcon("./icons/save.png") , "Save model" , "Save model" , this , this , SLOT(save_mesh()) );
        DetailedAction * help = new DetailedAction( QIcon("./icons/help.png") , "HELP" , "HELP" , this , this , SLOT(help()) );
        DetailedAction * saveCamera = new DetailedAction( QIcon("./icons/camera.png") , "Save camera" , "Save camera" , this , this , SLOT(saveCamera()) );
        DetailedAction * openCamera = new DetailedAction( QIcon("./icons/open_camera.png") , "Open camera" , "Open camera" , this , this , SLOT(openCamera()) );
        DetailedAction * saveSnapShotPlusPlus = new DetailedAction( QIcon("./icons/save_snapshot.png") , "Save snapshot" , "Save snapshot" , this , this , SLOT(saveSnapShotPlusPlus()) );
        DetailedAction * draw = new DetailedAction( QIcon("./icons/draw.png") , "Drawing mode" , "Drawing mode" , this , this , SLOT(toggle_drawing()) );
        DetailedAction * showControlPoints = new DetailedAction( QIcon("./icons/points.png") , "Control points" , "Control points" , this , this , SLOT(toggle_control_points()) );

        // Add them :
        toolBar->addAction( open_mesh );
        toolBar->addAction( save_mesh );
        toolBar->addAction( help );
        toolBar->addAction( saveCamera );
        toolBar->addAction( openCamera );
        toolBar->addAction( saveSnapShotPlusPlus );
        toolBar->addAction( draw );
        toolBar->addAction( showControlPoints );
    }


    void draw() {
        glEnable(GL_DEPTH_TEST);
        glEnable( GL_LIGHTING );
        glColor3f(0.5,0.5,0.8);
        glBegin(GL_TRIANGLES);
        for( unsigned int t = 0 ; t < mesh.triangles.size() ; ++t ) {
            point3d const & p0 = mesh.vertices[ mesh.triangles[t][0] ].p;
            point3d const & p1 = mesh.vertices[ mesh.triangles[t][1] ].p;
            point3d const & p2 = mesh.vertices[ mesh.triangles[t][2] ].p;
            point3d const & n = point3d::cross( p1-p0 , p2-p0 ).direction();
            glNormal3f(n[0],n[1],n[2]);
            glVertex3f(p0[0],p0[1],p0[2]);
            glVertex3f(p1[0],p1[1],p1[2]);
            glVertex3f(p2[0],p2[1],p2[2]);
        }

        glEnd();



        glPointSize(5);
        glColor3f(0.9,0.5,0.3);
        glBegin(GL_LINE_STRIP);

        if (points.size > 0) {
            unsigned int n = 0;
            while (n < points.size){
                for (int k = 0; k < 51; ++k) {
                    point3d c = points.deCasteljau(n, 0.02 * k);
                    glVertex3f(c[0], c[1], c[2]);
                }
                n += 3;
            }
        }
        glEnd();

        if (control_points) {
            glPointSize(5);
            glColor3f(0.0,0.5,0.3);
            glBegin(GL_POINTS);
            for (point3d p : points.positions) {
                glVertex3f(p[0], p[1], p[2]);
            }
            glEnd();
        }

    }

    void pickBackgroundColor() {
        QColor _bc = QColorDialog::getColor( this->backgroundColor(), this);
        if( _bc.isValid() ) {
            this->setBackgroundColor( _bc );
            this->update();
        }
    }

    void adjustCamera( point3d const & bb , point3d const & BB ) {
        point3d const & center = ( bb + BB )/2.f;
        setSceneCenter( qglviewer::Vec( center[0] , center[1] , center[2] ) );
        setSceneRadius( 1.5f * ( BB - bb ).norm() );
        showEntireScene();
    }


    void init() {
        makeCurrent();
        initializeOpenGLFunctions();

        setMouseTracking(true);// Needed for MouseGrabber.

        setBackgroundColor(QColor(255,255,255));

        // Lights:
        GLTools::initLights();
        GLTools::setSunsetLight();
        GLTools::setDefaultMaterial();

        //
        glShadeModel(GL_SMOOTH);
        glFrontFace(GL_CCW); // CCW ou CW

        glEnable(GL_DEPTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CLIP_PLANE0);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_COLOR_MATERIAL);

        //
        setSceneCenter( qglviewer::Vec( 0 , 0 , 0 ) );
        setSceneRadius( 10.f );
        showEntireScene();

        points.clear();
        drawing = true;
        pen_down = false;
        control_points = true;
    }

    QString helpString() const {
        QString text("<h2>Our cool project</h2>");
        text += "<p>";
        text += "This is a research application, it can explode.";
        text += "<h3>Participants</h3>";
        text += "<ul>";
        text += "<li>jmt</li>";
        text += "<li>...</li>";
        text += "</ul>";
        text += "<h3>Basics</h3>";
        text += "<p>";
        text += "<ul>";
        text += "<li>H   :   make this help appear</li>";
        text += "<li>Ctrl + mouse right button double click   :   choose background color</li>";
        text += "<li>Ctrl + T   :   change window title</li>";
        text += "</ul>";
        return text;
    }

    void updateTitle( QString text ) {
        this->setWindowTitle( text );
        emit windowTitleUpdated(text);
    }

    void keyPressEvent( QKeyEvent * event ) {
        if( event->key() == Qt::Key_H ) {
            help();
        }
        else if (event->key() == Qt::Key_D) {
            points.make3d();
            update();
        }
        else if (event->key() == Qt::Key_F) {
            points.filter();
            update();
        }
        else if( event->key() == Qt::Key_T ) {
            if( event->modifiers() & Qt::CTRL )
            {
                bool ok;
                QString text = QInputDialog::getText(this, tr(""), tr("title:"), QLineEdit::Normal,this->windowTitle(), &ok);
                if (ok && !text.isEmpty())
                {
                    updateTitle(text);
                }
            }
        }
    }

    void mouseDoubleClickEvent( QMouseEvent * e )
    {
        if( (e->modifiers() & Qt::ControlModifier)  &&  (e->button() == Qt::RightButton) )
        {
            pickBackgroundColor();
            return;
        }

        if( (e->modifiers() & Qt::ControlModifier)  &&  (e->button() == Qt::LeftButton) )
        {
            showControls();
            return;
        }

        QGLViewer::mouseDoubleClickEvent( e );
    }

    void mousePressEvent(QMouseEvent* e ) {
        if (drawing){
            bool found;
            points.add(camera()->pointUnderPixel(e->pos(), found));
            pen_down = true;
        }
        else {
            QGLViewer::mousePressEvent(e);
        }
    }

    void mouseMoveEvent(QMouseEvent* e  ){
        if (drawing && pen_down){
            point3d last_point = points.positions.back();
            bool found;
            point3d new_point = camera()->pointUnderPixel(e->pos(), found);
            if (pow(last_point[0] - new_point[0], 2) + pow(last_point[1] - new_point[1], 2) > 0.1) {
                points.add(new_point);
            }
            update();
        }
          QGLViewer::mouseMoveEvent(e);
    }

    void mouseReleaseEvent(QMouseEvent* e  ) {
        pen_down = false;
        QGLViewer::mouseReleaseEvent(e);
    }

signals:
    void windowTitleUpdated( const QString & );

public slots:
    void open_mesh() {
        bool success = false;
        QString fileName = QFileDialog::getOpenFileName(NULL,"","");
        if ( !fileName.isNull() ) { // got a file name
            if(fileName.endsWith(QString(".off")))
                success = OFFIO::openTriMesh(fileName.toStdString() , mesh.vertices , mesh.triangles );
            else if(fileName.endsWith(QString(".obj")))
                success = OBJIO::openTriMesh(fileName.toStdString() , mesh.vertices , mesh.triangles );
            if(success) {
                std::cout << fileName.toStdString() << " was opened successfully" << std::endl;
                point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
                for( unsigned int v = 0 ; v < mesh.vertices.size() ; ++v ) {
                    bb = point3d::min(bb , mesh.vertices[v]);
                    BB = point3d::max(BB , mesh.vertices[v]);
                }
                adjustCamera(bb,BB);
                update();
            }
            else
                std::cout << fileName.toStdString() << " could not be opened" << std::endl;
        }
    }

    void save_mesh() {
        bool success = false;
        QString fileName = QFileDialog::getOpenFileName(NULL,"","");
        if ( !fileName.isNull() ) { // got a file name
            if(fileName.endsWith(QString(".off")))
                success = OFFIO::save(fileName.toStdString() , mesh.vertices , mesh.triangles );
            else if(fileName.endsWith(QString(".obj")))
                success = OBJIO::save(fileName.toStdString() , mesh.vertices , mesh.triangles );
            if(success)
                std::cout << fileName.toStdString() << " was saved" << std::endl;
            else
                std::cout << fileName.toStdString() << " could not be saved" << std::endl;
        }
    }

    void showControls()
    {
        // Show controls :
        controls->close();
        controls->show();
    }

    void saveCameraInFile(const QString &filename){
        std::ofstream out (filename.toUtf8());
        if (!out)
            exit (EXIT_FAILURE);
        // << operator for point3 causes linking problem on windows
        out << camera()->position()[0] << " \t" << camera()->position()[1] << " \t" << camera()->position()[2] << " \t" " " <<
                                          camera()->viewDirection()[0] << " \t" << camera()->viewDirection()[1] << " \t" << camera()->viewDirection()[2] << " \t" << " " <<
                                          camera()->upVector()[0] << " \t" << camera()->upVector()[1] << " \t" <<camera()->upVector()[2] << " \t" <<" " <<
                                          camera()->fieldOfView();
        out << std::endl;

        out.close ();
    }

    void openCameraFromFile(const QString &filename){

        std::ifstream file;
        file.open(filename.toStdString().c_str());

        qglviewer::Vec pos;
        qglviewer::Vec view;
        qglviewer::Vec up;
        float fov;

        file >> (pos[0]) >> (pos[1]) >> (pos[2]) >>
                                                    (view[0]) >> (view[1]) >> (view[2]) >>
                                                                                           (up[0]) >> (up[1]) >> (up[2]) >>
                                                                                                                            fov;

        camera()->setPosition(pos);
        camera()->setViewDirection(view);
        camera()->setUpVector(up);
        camera()->setFieldOfView(fov);

        camera()->computeModelViewMatrix();
        camera()->computeProjectionMatrix();

        update();
    }


    void openCamera(){
        QString fileName = QFileDialog::getOpenFileName(NULL,"","*.cam");
        if ( !fileName.isNull() ) {                 // got a file name
            openCameraFromFile(fileName);
        }
    }
    void saveCamera(){
        QString fileName = QFileDialog::getSaveFileName(NULL,"","*.cam");
        if ( !fileName.isNull() ) {                 // got a file name
            saveCameraInFile(fileName);
        }
    }

    void saveSnapShotPlusPlus(){
        QString fileName = QFileDialog::getSaveFileName(NULL,"*.png","");
        if ( !fileName.isNull() ) {                 // got a file name
            setSnapshotFormat("PNG");
            setSnapshotQuality(100);
            saveSnapshot( fileName );
            saveCameraInFile( fileName+QString(".cam") );
        }
    }
    void toggle_drawing(){
        drawing = !drawing;
    }

    void toggle_control_points() {
        control_points = !control_points;
        update();
    }
};




#endif // MYVIEWER_H
