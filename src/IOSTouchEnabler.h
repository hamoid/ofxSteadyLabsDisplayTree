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

#ifndef IOSTOUCHENABLER_H
#define IOSTOUCHENABLER_H

#include "InteractionEnabler.h"
#include "ofMain.h"

class DisplayObject;
//class ofTouchEventArgs;

class IOSTouchEnabler: public InteractionEnabler{
public:
    IOSTouchEnabler( DisplayObject* inTarget = NULL ): InteractionEnabler( inTarget ){ 
        _touchOver	= false;
        _touchDown	= false;
        blocking = true;
    };
    
    ~IOSTouchEnabler();
    
    void enableTouchEvents();
    void disableTouchEvents();
    
    
    bool _touchMoved(ofTouchEventArgs &e);
    void _touchMovedBlocked(ofTouchEventArgs &e);
	void _touchPress(ofTouchEventArgs &e, bool overRideHitTest = true);
	// void _touchDragged(ofTouchEventArgs &e, bool overRideHitTest = false);
	void _touchUp(ofTouchEventArgs &e, bool overRideHitTest = true);
    /*
    void touchDown( ofTouchEventArgs &touch );
	void touchMoved( ofTouchEventArgs &touch );
	void touchUp( ofTouchEventArgs &touch );
	void touchDoubleTap( ofTouchEventArgs &touch );
	void touchCancelled( ofTouchEventArgs &touch );
    */
    bool blocking;
    
    
protected:
    
    int _touchX, _touchY, _touchButton;
    bool		_touchOver;
	bool		_touchDown;
    
    
    
    static void sortEnablers();
    
    friend class IOSTouchEventController;
};


#endif