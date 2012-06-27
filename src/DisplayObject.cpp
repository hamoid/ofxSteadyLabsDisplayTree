/***********************************************************************
 
 Copyright (c) 2011,2012, Mike Manh
 ***SteadyLabs, STEADY LTD http://steadyltd.com ***
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are met:
 
 *Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 *Redistributions in binary form must reproduce the above copyright notice, 
 this list of conditions and the following disclaimer in the 
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/ 

#include <iostream>
#include "DisplayObject.h"
#include "MouseEnabler.h"

string DisplayObject::ON_PRESS = "onPress";
string DisplayObject::ON_RELEASE = "onRelease";
string DisplayObject::ON_RELEASE_OUTSIDE = "onReleaseOutside";
string DisplayObject::ON_ROLLOVER = "onRollover";
string DisplayObject::ON_ROLLOUT = "onRollout";


ofMatrix4x4 DisplayObject::baseMatrix;

DisplayObject::DisplayObject(){
    init();
}

void DisplayObject::init(){
    animator=new DisplayObjectAnimator(this);
    mouseEnabler = new MouseEnabler( this );
    parentSprite = NULL;
    name = ""; //default to empty string
    visible = true;
    worldMouseEnabled = mouseEnabled = true;
    alpha = 1.0f;
    rotation = 0.f;
    renderOrder = -1;
    isRoot = false;
    scaleX = scaleY = 1.f;
    registrationX = registrationY = 0.f;
    blendMode = OF_BLENDMODE_ALPHA;
    clipToBounds=false;
    opaque=false;
    backgroundColor=ofColor(255,255,255,255);
    clipMargins.x=0;
    clipMargins.y=0;
    clipMargins.z=0;
    clipMargins.w=0;
    
    initialX=0;
    initialY=0;
}

DisplayObject::~DisplayObject(){
    //TODO: destroy
    disableMouseEvents();
    
    //rdelete animator;
    
    int numChildren = children.size();
    for (int i = 0; i < numChildren; i++)
        delete children[i];
    
    delete mouseEnabler;
}

void DisplayObject::update(){
    animator->update();
    
    if ( !visible ) return;
    int numChildren = children.size();
    for ( int i = 0; i < numChildren; i++ ){
        //cout << children[i]->name << " update" << endl;
        children[ i ]->update();
    }
}

void DisplayObject::render(){

}

void copyCurMatrix( ofMatrix4x4 &outMat){
    GLfloat m[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, m);   
    outMat.set(m);
}

int DisplayObject::draw( int inRenderOrder){
    if ( isRoot ){ //ok we will need people to specify if something is root... gone are the innocent days when you could just draw whenever
        
        copyCurMatrix(DisplayObject::baseMatrix);
        inRenderOrder = 0;
        renderOrder = 0;
        
    }
    //cout << "DisplayObject::draw::renderOrder:" <<inRenderOrder<<"\n";
    if (!visible ) {
        renderOrder = -1;
        //todo maybe reset the render orders of all children
        
        return inRenderOrder;
        
    }
    
    
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode( blendMode );
    calcTransform(); 
    //calc transform only calculates alpha transforms, since the matrices are taken care of
  
    ofSetColor(255, 255, 255, worldAlpha * 255);
    ofTranslate( x, y );
    
    
    ofScale( scaleX, scaleY);
    ofRotate( rotation, 0, 0, 1 );
    
    //HAZARD: i have no idea why this works... Mike Manh
    ofTranslate( -registrationX, -registrationY );
    
    copyCurMatrix(transformedMatrix);
    
    if ((clipToBounds) && (width>0) && (height>0))
    {
        float psX=scaleX;
        float psY=scaleY;
        
        float dx=x;
        float dy=y;
        
        float ph=0;
        float pw=0;
        
        DisplayObject *p=parentSprite;
        while(p)
        {
            ph=(p->height>ph) ? p->height : ph;
            pw=(p->width>pw) ? p->width : pw;
            
            psX*=p->scaleX;
            psY*=p->scaleY;
            dx+=p->x;
            dy+=p->y;
            p=p->parentSprite;
        }
        
        glEnable(GL_SCISSOR_TEST);
        glScissor((dx-clipMargins.x)*psX, (ph-((dy-clipMargins.y)+height))*psY, (width+(clipMargins.x+clipMargins.z))*psX, (height+(clipMargins.y+clipMargins.w)));
    }

    render();

    renderOrder = inRenderOrder;
    
    /*
    GLint curStackDepth;
    GLint maxStackDepth;
    glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &curStackDepth);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &maxStackDepth);
    
    cout << "DisplayObject::draw::renderOrder:" << renderOrder << ", name:" <<name << ", maxStackDepth:" << maxStackDepth << ", curStackDepth:" << curStackDepth << endl;
     */
    
    if (opaque)
    {
        ofSetColor(ofColor(backgroundColor.r,backgroundColor.g,backgroundColor.b,floor((float)backgroundColor.a*alpha)));
        glRectd(0, 0, width, height);
    }
     
    inRenderOrder++;
    
    inRenderOrder = drawChildren( inRenderOrder );
    drawComplete();
    
    if ((clipToBounds) && (width>0) && (height>0))
    {
        glDisable(GL_SCISSOR_TEST);
    }

    
    ofPopStyle();
    ofPopMatrix();
    
//    if (clipToBounds)
//        endClip();
//    
    return inRenderOrder;
}

void DisplayObject::depthFirstTraversalWithVoid( void (DisplayObject::*pt2Func)(void) ){
    int numChildren = children.size();
    for( int i = 0; i < numChildren; i++ ){
       // children[i]->*pt2Func();
       // *pt2Func();

    }
}

void DisplayObject::depthFirstTraversalWithInt( int (DisplayObject::*pt2Func)( int ) ){
    int numChildren = children.size();
    for( int i = 0; i < numChildren; i++ ){
       // children[i]->*pt2Func(3);
        
    }
}

void DisplayObject::drawComplete(){ //don't need this anymore, i don't think
    if ( parentSprite == NULL ){ //ie it is root
        ofDisableAlphaBlending();
    }
}

int DisplayObject::drawChildren( int inRenderOrder ){
    int numChildren = children.size();
    for ( int i = 0; i < numChildren; i++ ){
        inRenderOrder = children[ i ]->draw( inRenderOrder );
    }
    return inRenderOrder;
}

/*bool DisplayObject::hitTest(int tx, int ty) {
	return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}*/

ofPoint DisplayObject::unprojectPoint(int x, int y ){

    ofVec3f outVec(x,y,0);
    //cout << "outVec:"<< outVec << endl;
    //cout << testMat.getInverse().preMult(outVec) << endl;
    
    
    //todo maybe break this into a function, unprojectPoint
    ofMatrix4x4 unWindowedMatrix = transformedMatrix;
    unWindowedMatrix.postMult( baseMatrix.getInverse() ); //undo the window's transforms
    ofVec3f thisPoint = unWindowedMatrix.getInverse().preMult(outVec);    
    return thisPoint;
}

bool DisplayObject::hitTest(int tx, int ty) {
    ofVec3f thisPoint = unprojectPoint( tx, ty);
    //cout << tx << " " << ty << " " << thisPoint.x << " " << thisPoint.y << " " <<  endl;
    
    return ( thisPoint.x >=0 && thisPoint.x <=width && thisPoint.y >=0 &&thisPoint.y <= height);
    return ( thisPoint.x >=0 && thisPoint.x <=width && thisPoint.y >=0 &&thisPoint.y <= height);
    
    // cout << "Sprite::hitTest::0:tx:" << tx << "\n";
    /*bool hit = ((tx > worldX) && (tx < worldX + width) && (ty > worldY) && (ty < worldY + height));
    if (hit)
    {
        // cout << "Sprite::hitTest::1:tx:" << tx << "\n";
    }
    
	return ((tx > worldX) && (tx < worldX + width) && (ty > worldY) && (ty < worldY + height));
     */
}

void DisplayObject::addChild( DisplayObject* inSprite ){
    int extantIndex = getIndexOfChild(inSprite);
    
    if ( extantIndex == -1 ){
        children.push_back(inSprite);
        inSprite->parentSprite = this;
        inSprite->recalcMouseState();
        // cout << "PARENT:: " << this->name << " CHILD:: " << inSprite->name << endl;
    }
    
    // cout << "BaseSprite::addChild::parent:"<< name << ", child:" << inSprite->name << "\n";
}

void DisplayObject::calcTransform(){
    if ( parentSprite ==NULL ){ //root or not on stage
        //worldX = x;
        //worldY = y;
        worldAlpha = alpha;

		// cout<< "root sprite's alpha is:"<<worldAlpha<< "\n";
    }
    else{
        
        //worldX = parentSprite-> worldX + x;
        //worldY = parentSprite-> worldY + y;
        
        worldVisible = visible && parentSprite->worldVisible;
        worldAlpha = parentSprite-> worldAlpha * alpha; //alpha is expected to be between 0 and 1
        // cout<< "parent's alpha is:"<< parentSprite->worldAlpha << ", alpha is:"<< worldAlpha<< "\n";
    }
    
    int alphaConverted = worldAlpha * 255;
    // std::cout<< "alphaConverted:" << alphaConverted << "\n";
    //ofSetColor(255, 255, 255, alphaConverted);
    
}

void DisplayObject::traceTransformChain(){
    DisplayObject* curObj = this;
    cout <<"traceTransformChain:\n";
    while ( curObj != NULL ){
        curObj->traceTransform();
        curObj = curObj->parentSprite;
    }
}

void DisplayObject::traceTransform(){
    cout<< "name:" << name <<" x:" << x << ", y:"<< y << ", scaleX:" << scaleX <<", scaleY:" << scaleY <<", rotation:" <<rotation << endl;
}

int DisplayObject::getIndexOfChild( DisplayObject* inSprite ){
    vector< DisplayObject* >::iterator result;
    
    result = std::find( children.begin(), children.end(), inSprite);
    
    if ( result == children.end()){
        return -1; //sprite is not in the children list yet
    }
    
    else return (result - children.begin() ) ; //man this seems hackish, but apparently vectors are guaranteed to be contiguous blocks of memory
}

void DisplayObject::removeChild( DisplayObject* inSprite ){
    vector< DisplayObject* >::iterator result;
    
    result = std::find( children.begin(), children.end(), inSprite);
    
    if ( result != children.end() ){
        children.erase( result );
        inSprite->parentSprite = NULL;
        inSprite->recalcMouseState();//might not be necessary
    }
}

void DisplayObject::mouseEnable(){
    mouseEnabled = true;
    recalcMouseState();
}

struct FindByName {
    string name;
    FindByName(const std::string& name) : name(name) {}
    bool operator()(const DisplayObject* j) { return j->name == name; }
};

DisplayObject* DisplayObject::getChildByName( string inName ){
    vector< DisplayObject* >::iterator result;
    
    result = std::find_if( children.begin(), children.end(), FindByName( inName ) );
    
    if ( result == children.end()){
        return NULL; //sprite is not in the children list yet
    }
    else{
        return( children[ result - children.begin()] );
    }
    
}

DisplayObject *DisplayObject::rootSprite()
{
    if (parentSprite==NULL)
        return NULL;
    
    DisplayObject *p=parentSprite;
    while(true)
    {
        if (p->parentSprite==NULL)
            return p;
        
        p=p->parentSprite;
    }
}

DisplayObject *DisplayObject::spriteNamed(std::string spriteName, bool descend)
{
    cout << "search for " << spriteName << endl;
    DisplayObject *result=getChildByName(spriteName);
    
    if (result)
        return result;
    
    if (descend)
    {
        int count=children.size();
        for(int i=0; i<count; i++)
        {
            result=children[i]->spriteNamed(spriteName,true);
            if (result)
                return result;
        }
    }
    
    return NULL;
}

void DisplayObject::mouseDisable(){
    mouseEnabled = false;
    recalcMouseState();
}

void DisplayObject::recalcMouseState(){
    if ( parentSprite == NULL ){
        worldMouseEnabled = mouseEnabled; 
    }
    else{
        worldMouseEnabled = parentSprite->worldMouseEnabled && mouseEnabled;
    }
    int numChildren = children.size();
    for ( int i = 0; i < numChildren; i++ ){
        children[ i ]->recalcMouseState();
    }
}

//====================end basesprite stuff====================

void DisplayObject::enableMouseEvents() {
    mouseEnabled=true;
    // cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< DisplayObject::enableMouseEvents\n";
    mouseEnabler->enableMouseEvents();

}

void DisplayObject::disableMouseEvents() {
    // cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< DisplayObject::disableMouseEvents\n";
    mouseEnabled=false;
    mouseEnabler->disableMouseEvents();
}

void DisplayObject::setMouseBlocking(bool inBlocking){
    mouseEnabler->blocking = inBlocking;
}


void DisplayObject::onPress(int x, int y, int button)
{
    this->dispatchEvent(ON_PRESS, name);
}


void DisplayObject::onRelease(int x, int y, int button)
{
    this->dispatchEvent(ON_RELEASE, name);
}

void DisplayObject::onReleaseOutside(int x, int y, int button)
{
    this->dispatchEvent(ON_RELEASE_OUTSIDE, name);
}

//TODO: CK, implement rollover and rollout states
void DisplayObject::onRollOut()
{
    this->dispatchEvent(ON_ROLLOUT, name);
}

void DisplayObject::onRollOver(int x, int y)
{
    this->dispatchEvent(ON_ROLLOVER, name);
}

