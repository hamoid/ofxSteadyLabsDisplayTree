#include "ofApp.h"
#include "MouseEventController.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //objects with mouse events require the mousecontroller to be initialized before instantiating the objects
    MouseEventController::init();

    root = new DisplayObject();
    root->isRoot = true;
    string buttonDir = "testButton";

    //instantiate the buttons
    button1 = new ButtonSprite( buttonDir );
    button2 = new ButtonSprite( buttonDir );

    //set the registration point to the center, so it draws and scales from the center
    button1->registrationX = button1->width/2;
    button1->registrationY = button1->height/2;

    //set the registration point to the center, so it draws and scales from the center
    button2->registrationX = button2->width/2;
    button2->registrationY = button2->height/2;

    button1->x = 100;
    button2->x = 300;

    //give them a 100 pixel margin from the top
    button1->y = button2->y = 150;

    //add it to root
    root->addChild( button1 );
    root->addChild( button2 );

    //button 1 gets bigger every time you roll over it, but when you click it, it returns to normal size
    button1->addListener(ButtonSprite::ON_PRESS, this, &ofApp::onButton1Pressed);
    button1->addListener(ButtonSprite::ON_ROLLOVER, this, &ofApp::onButton1RolledOver);


    //button 2 gets smaller every time you click it, but when you roll out, it returns to normal size
    button2->addListener(ButtonSprite::ON_PRESS, this, &ofApp::onButton2Pressed);
    button2->addListener(ButtonSprite::ON_ROLLOUT, this, &ofApp::onButton2RolledOut);
}

//--------------------------------------------------------------
void ofApp::onButton1Pressed( ofMessage &e ){
    button1->scaleX = button1->scaleY = 1;
}

void ofApp::onButton1RolledOver( ofMessage &e ){
    button1->scaleX *=1.1;
    button1->scaleY = button1->scaleX;
}


//--------------------------------------------------------------
void ofApp::onButton2Pressed( ofMessage &e ){
    button2->scaleX *=.95;
    button2->scaleY = button2->scaleX;

}

void ofApp::onButton2RolledOut( ofMessage &e ){
    button2->scaleX = button2->scaleY = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
    root->update();
    MouseEventController::processEvents();
}

//--------------------------------------------------------------
void ofApp::draw(){
    root->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
