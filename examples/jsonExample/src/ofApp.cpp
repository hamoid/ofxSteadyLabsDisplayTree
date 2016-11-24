#include "ofApp.h"
#include "MouseEventController.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Init the MouseEventController
    MouseEventController::init();

    // Root BaseSprite to contain all assets on the stage
    root = new BaseSprite();
    root->isRoot = true;
    root->name = "root";
    root->visible = true;

    // Loading a background image from the filepath
    stageImg = new BitmapSprite();
    stageImg->loadFile( "stage.png" );
    stageImg->x = 16.5f;
    stageImg->y = 91.0f;
    stageImg->name = "stage";

    // Scraping children assets from JSON
    // This steps through all children elements in the demo json
    // and loads them into sprites for the display tree.
    Json::Value siteJSON = jsonParser.parse("sample_json.json");
    BaseSprite* newSprite = new BaseSprite();
    jsonParser.loadSpritesFromJSON(siteJSON["children"], newSprite);
    newSprite->visible = true;

    // Finally we add everything to the root stage
    root->addChild(stageImg);
    root->addChild(newSprite);
}

//--------------------------------------------------------------
void ofApp::update(){
    // Clear our background
    ofBackground(0);

    // Update mouse events
    MouseEventController::processEvents();

    // Update our display tree
    root->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw the root to the display tree
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
