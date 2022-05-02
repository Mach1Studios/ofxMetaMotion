//
//  ThreadedMMC.h
//  threadedMetaMotionExample
//
//  Created by Dylan Marcus on 1/5/21.
//

#ifndef ThreadedMMC_h
#define ThreadedMMC_h

#include "ofMain.h"
#include <atomic>

class ThreadedMMC: public ofThread
{
public:
    metamotionController mmc;
    
    ~ThreadedMMC(){
        stop();
        waitForThread(false);
    }

    void setup(){
        mmc.setup();
        startThread();
    }

    void stop(){
        if (mmc.isConnected){
            mmc.disconnectDevice(mmc.board);
        }
        stopThread();
    }

    void threadedFunction(){
        while(isThreadRunning()){
            if (!mmc.isConnected && !mmc.isSearching &&
                !mmc.currentlyInitializing){
                mmc.currentlyInitializing  = mmc.search();
            }
            mmc.update();
            threadFrameNum++;
        }
    }
    
    int getThreadFrameNum(){
        return threadFrameNum;
    }
    
protected:
    int threadFrameNum = 0;
};


#endif /* ThreadedMMC_h */
