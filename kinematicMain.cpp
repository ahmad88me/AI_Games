/* 
    Andrea Centeno
    carnet USB: 10-10138
    sep-dic 2017
*/    
#include "header.h"
#include "draw.cpp"
#include "movements/KinematicSeek.h"
#include "movements/KinematicFlee.h"
#include "movements/KinematicArrive.h"
#include "movements/KinematicWander.h"
    
GLfloat ang = glm::radians(1.0),
        trans = 0.3,
        pointSize=2.0;

GLfloat maxSpeed = 0.05;
GLfloat maxAcceleration = 0.05;

Static static_target = {{-16.0f,-4.0f},0.0f};
Static static_character = {{12.0f,8.0f},0.0f};

KinematicSeek kinematicSeek = {static_character,static_target};
KinematicFlee kinematicFlee = {static_character,static_target};
KinematicArrive kinematicArrive = {static_character,static_target};
KinematicWander kinematicWander = {static_character};

GLfloat oldTimeSinceStart = 0.0;

/************************* KEYBOARD **************************/
void controlKey (unsigned char key, int xmouse, int ymouse){   
    switch (key){
        case 'a': //Trasladarse a la izquierda.  
            static_target.position.x-=trans; break;
        case 'd': //Trasladarse a la derecha.  
            static_target.position.x+=trans; break;
        case 'w': //Trasladarse arriba. 
            static_target.position.y+=trans; break;
        case 's': //Trasladarse abajo.   
            static_target.position.y-=trans; break;
        case 'z': //Rotar anti horario.  
            static_target.orientation -=ang; break;
        case 'c': //Rotar horario. 
            static_target.orientation +=ang; break;
        default: break;
    }
    //glutPostRedisplay(); 
}

GLfloat getDeltaTime(){    
    GLfloat timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    GLfloat deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    return deltaTime;
}

void kinematicSeekMovement(){  
    KinematicSteeringOutput kso = kinematicSeek.getSteering();
    static_character.update(kso.velocity,kso.rotation,getDeltaTime());
}

void kinematicFleeMovement(){  
    KinematicSteeringOutput kso = kinematicFlee.getSteering();
    static_character.update(kso.velocity,kso.rotation,getDeltaTime());
}

void kinematicArriveMovement(){  
    KinematicSteeringOutput kso = kinematicArrive.getSteering();
    if(length(kso.velocity) != 0){
        static_character.update(kso.velocity,kso.rotation,getDeltaTime());
    }
}

void kinematicWanderMovement(){  
    KinematicSteeringOutput kso = kinematicWander.getSteering();
    static_character.update(kso.velocity,kso.rotation,getDeltaTime());
}

/************************** Display **************************/
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH); 
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 10, 0, 0, 1, 0);

    
    glLineWidth(pointSize);
    glColor3f(0,0.6,0.6);
    drawFace(static_target.position,static_target.orientation,pointSize);
    glColor3f(0.4,0.2,0.8);
    drawFace(static_character.position,static_character.orientation,pointSize);
    
    //kinematicSeekMovement();
    //kinematicFleeMovement();
    kinematicArriveMovement();
    //kinematicWanderMovement();

    glFlush();
    glutPostRedisplay();
}

/************************* Viewport **************************/
void reshape(int w, int h) {
    GLfloat aspectratio = (GLfloat) w / (GLfloat) h;
    GLfloat zoom = 30.0;

    glMatrixMode(GL_PROJECTION);   
    glLoadIdentity(); 
    glViewport(0, 0, w, h);
    /*
    gluPerspective( 45 ,        // ángulo de visión
      (float)h/(float)w, // Razón entre el largo y el ancho, para calcular la perspectiva
      1,                        // Cuan cerca se puede ver
      1000);
    */
    if (w <= h){
        glOrtho(-zoom, zoom, -zoom/aspectratio, zoom/aspectratio, -1.0, 1.0); 
    }else{
        glOrtho(-zoom*aspectratio, zoom*aspectratio, -zoom, zoom, -1.0, 1.0);
    }
    
}

/*************************** MAIN ***************************/
int main (int argc, char** argv) {
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("AI VideoGame - Kinematic");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutKeyboardFunc(controlKey);
    
    glutMainLoop();
    return 0;
}