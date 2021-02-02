//
//  ThreadedMMC.h
//  threadedMetaMotionExample
//
//  Created by Dylan Marcus on 1/5/21.
//

#ifndef ThreadedMMC_h
#define ThreadedMMC_h

#include "ofMain.h"

class ThreadedMMC: public ofThread
{
public:

    ~ThreadedMMC(){
        stop();
        waitForThread(false);
    }

    void setup(){
        start();
    }

    /// Start the thread.
    void start(){
        startThread();
    }

    void stop(){
        stopThread();
    }

    void threadedFunction(){
        while(isThreadRunning()){
        }
    }

    int getThreadFrameNum(){
        return threadFrameNum;
    }
    
protected:
    int threadFrameNum = 0;
};


#endif /* ThreadedMMC_h */
