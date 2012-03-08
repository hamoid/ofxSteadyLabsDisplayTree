/***********************************************************************
 
 Copyright (c) 2011,2012, Mike Manh
 ***STEADY LTD http://steadyltd.com ***
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

#ifndef IOSTOUCHEVENTCONTROLLER_H
#define IOSTOUCHEVENTCONTROLLER_H

#include "ofMain.h"

class IOSTouchEnabler;

class IOSTouchEventController{
    
public:
    IOSTouchEventController();
    ~IOSTouchEventController();
    static void addEnabler( IOSTouchEnabler* inEnabler);
    static void removeEnabler( IOSTouchEnabler* inEnabler);
    static void processEvents();
    static void init();
    
    enum TouchEventType
    {
        STILL,
        TOUCH_DOWN,
        TOUCH_UP,
        TOUCH_MOVE
    };
    
    struct TouchEvent{
        TouchEventType type;
        ofTouchEventArgs args;
    };
        
    void touchDown( ofTouchEventArgs &touch );
	void touchMoved( ofTouchEventArgs &touch );
	void touchUp( ofTouchEventArgs &touch );
	void touchDoubleTap( ofTouchEventArgs &touch );
	void touchCancelled( ofTouchEventArgs &touch );

private:
    
protected:
    
    std::vector<IOSTouchEnabler*> _touchEnablers;
    std::map< IOSTouchEnabler*, int> _touchEnablerToIndex;
    
    static IOSTouchEventController* instance;
    
    void _addEnabler( IOSTouchEnabler* inEnabler);
    void _removeEnabler( IOSTouchEnabler* inEnabler);
    
    //a queue of touch events that have been adding up since before the event phase
    std::queue< TouchEvent* > _eventQueue;
        
    void _handleEvent( TouchEvent* inEvent );
    
    void _init();
    
    void _sort();
    void _processEvents();
    
    
    bool _touchMovedThisFrame;
    
};


#endif