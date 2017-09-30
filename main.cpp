/* 
    Andrea Centeno
    carnet USB: 10-10138
    sep-dic 2017
*/    
#include "header.h"
#include "draw.cpp"
#include "movements/Seek.h"
#include "movements/Flee.h"
#include "movements/Arrive.h"
#include "movements/Align.h"
#include "movements/VelocityMatch.h"
#include "movements/Pursue.h"
#include "movements/Evade.h"
#include "movements/Face.h"
#include "movements/LookWhereYoureGoing.h"
#include "movements/Wander.h"
#include "movements/Separation.h"
#include "movements/CollisionAvoidance.h"


GLfloat oldTimeSinceStart = 0.0;
GLfloat pointSize=1.5;

GLfloat targetRotation = glm::radians(10.0);
GLfloat targetVelocity = 10;

GLfloat maxSpeed = 8;
GLfloat maxAcceleration = 15;
GLfloat maxPrediction = 1.2;

Kinematic target = {{-16.0f,-4.0f}};
Kinematic character = {{10.0f,-5.0f}};

bool iniListTargets = false;
list<Kinematic*> targets;

Seek seek = {character,target,maxAcceleration};
Flee flee = {character,target,maxAcceleration};
Arrive arrive = {character,target,3,5,maxAcceleration,maxSpeed};
//Align -> {&character,&target,maxAngularAcceleration,maxRotation,slowRadius,targetRadius}
Align align = {character,target,20,30,5,2};
VelocityMatch velocityMatch = {character,target,maxAcceleration};

/**************** Delegated Behaviors ****************/
Pursue pursue = {character,target,maxAcceleration,maxPrediction};
Evade evade = {character,target,maxAcceleration,maxPrediction};
Face face = {character,target,10,30,5,2}; // Align()
LookWhereYoureGoing lookWhereYoureGoing = {character,target,10,30,5,2}; // Align()
//Wander -> {Face(),wanderOffset,wanderRadius,wanderRate,wanderOrientation,maxAcceleration}
Wander wander = {character,20,30,5,2, -1,6,2,30,10}; 
//Separatio -> {character,targets,threshold,decayCoefficient,maxAcceleration}
Separation separation = {character,targets,6,10,30};
//CollisionAvoidance -> {character,targets,maxAcceleration,radius}
CollisionAvoidance collisionAvoidance = {character,targets,4,2};

void initializeListTargets(){
    for(int i ; i< 10; i++){
        targets.push_back(new Kinematic({-30.0 + i*7,8.0f},0.0,{0.0,1}));
    }
    iniListTargets = true;
}

void moveListTargets(GLfloat deltaTime){
    for (list<Kinematic*>::iterator t=targets.begin(); t != targets.end(); ++t){
        glColor3f(0,0.6,0.6);
        drawFace((*t)->position,(*t)->orientation,pointSize);
    }

    for (list<Kinematic*>::iterator t=targets.begin(); t != targets.end(); ++t){
        if((*t)->position.y > 10 || (*t)->position.y < -10) (*t)->velocity.y *= (-1);
        (*t)->updatePosition(deltaTime);
    }
}

/******************************* MOVEMENTS *****************************/
void SeekMovement(GLfloat deltaTime){  
    SteeringOutput so = seek.getSteering();
    character.update(so,maxSpeed,deltaTime);
}

void FleeMovement(GLfloat deltaTime){  
    SteeringOutput so = flee.getSteering();
    character.update(so,maxSpeed,deltaTime);
}

void ArriveMovement(GLfloat deltaTime){  
    SteeringOutput so = arrive.getSteering();
    if(length(so.linear) != 0) character.update(so,maxSpeed,deltaTime);
}

void AlignMovement(GLfloat deltaTime){  
    SteeringOutput so = align.getSteering();
    if( so.angular != 0.0 ) character.update(so,maxSpeed,deltaTime);
}

void VelocityMatchMovement(GLfloat deltaTime){  
    SteeringOutput so = velocityMatch.getSteering();
    character.update(so,maxSpeed,deltaTime);
}

void PursueMovement(GLfloat deltaTime){  
    SteeringOutput so = pursue.getSteering();
    character.update(so,maxSpeed,deltaTime);
}

void EvadeMovement(GLfloat deltaTime){  
    SteeringOutput so = evade.getSteering();
    character.update(so,maxSpeed,deltaTime);
}

void FaceMovement(GLfloat deltaTime){  
    SteeringOutput so = face.getSteering();
    if(so.angular != 0.0) character.update(so,maxSpeed,deltaTime);
}

void LookWhereYoureGoing(GLfloat deltaTime){  
    SteeringOutput so = lookWhereYoureGoing.getSteering();
    if(so.angular != 0.0) character.update(so,maxSpeed,deltaTime);
}

void WanderMovement(GLfloat deltaTime){  
    SteeringOutput so = wander.getSteering();
    if(so.angular != 0.0) character.update(so,maxSpeed,deltaTime);
}

void SeparationMovement(GLfloat deltaTime){  
    SteeringOutput so = separation.getSteering();
    if(length(so.linear) != 0) character.update(so,maxSpeed,deltaTime);
}

void CollisionAvoidance(GLfloat deltaTime){  
    SteeringOutput so = collisionAvoidance.getSteering();
    if(length(so.linear) != 0) character.update(so,maxSpeed,deltaTime);
}

/******************************* KEYBOARD *****************************/
void controlKey (unsigned char key, int xmouse, int ymouse){  
    switch (key){
        case 'a': 
            target.rotation = targetRotation;
        break;
        case 'd': 
            target.rotation = -targetRotation;
        break;
        default: break;
    }  
}

void controlKeyReleased (unsigned char key, int xmouse, int ymouse){   
    switch (key){
        case 'a': 
        case 'd': 
            target.rotation = 0.0;
        break;
        default: break;
    } 
}

void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            target.velocity = {-targetVelocity,0.0};
        break;
        case GLUT_KEY_RIGHT:
            target.velocity = {targetVelocity,0.0};
        break;
        case GLUT_KEY_UP:
            target.velocity = {0.0,targetVelocity};
        break;
        case GLUT_KEY_DOWN:
            target.velocity = {0.0,-targetVelocity};
        break;
        default: break;
    }
}

void handleSpecialKeyReleased(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            target.velocity = {0.0,0.0};
        break;
        default: break;
    }
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
    glColor3f(0.6,0.6,0.6);
    drawFace(target.position,target.orientation,pointSize);
    glColor3f(0.4,0.2,0.8);
    drawFace(character.position,character.orientation,pointSize);
   
    GLfloat timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    GLfloat deltaTime = (timeSinceStart - oldTimeSinceStart) * 0.001;
    oldTimeSinceStart = timeSinceStart;

    moveListTargets(deltaTime);

    target.updatePosition(deltaTime);
    target.updateOrientation(deltaTime);
    character.updatePosition(deltaTime);
    //SeekMovement(deltaTime);
    //FleeMovement(deltaTime);
    //ArriveMovement(deltaTime);
    //AlignMovement(deltaTime);
    //VelocityMatchMovement(deltaTime);
    //PursueMovement(deltaTime);
    //EvadeMovement(deltaTime);
    //FaceMovement(deltaTime);
    //LookWhereYoureGoing(deltaTime);
    //WanderMovement(deltaTime);
    //SeparationMovement(deltaTime);
    //CollisionAvoidance(deltaTime);

    glFlush();
    glutPostRedisplay();
}

/************************* Viewport **************************/
void reshape(int w, int h) {
    GLfloat aspectratio = (GLfloat) w / (GLfloat) h;
    GLfloat zoom = 35.0;

    glMatrixMode(GL_PROJECTION);   
    glLoadIdentity(); 
    glViewport(0, 0, w, h);
    /*
    gluPerspective( 45 ,        // �ngulo de visi�n
      (float)h/(float)w, // Raz�n entre el largo y el ancho, para calcular la perspectiva
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
    glutCreateWindow("AI VideoGame");

    if(!iniListTargets) initializeListTargets();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutSpecialFunc(handleSpecialKeypress);
    glutSpecialUpFunc(handleSpecialKeyReleased);

    glutKeyboardFunc(controlKey); 
    glutKeyboardUpFunc(controlKeyReleased); 
    
    glutMainLoop();
    return 0;
}