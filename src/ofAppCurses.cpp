//     _____    ___
//    /    /   /  /     ofxNcurses
//   /  __/ * /  /__    (c) ponies & light, 2014.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxNcurses
//  Created by Tim Gfrerer on 09/10/2014.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "ofAppCurses.h"
#include "ofAppNoWindow.h"
#include "ofMain.h"

#if defined TARGET_OSX || defined TARGET_LINUX
//#include <stdlib.h>
//#include <string.h>
//#include <sys/select.h>
//#include <termios.h>
//#include <signal.h>
#include <curses.h>
#endif


#define inWin ((WINDOW*)inputWin)

//----------------------------------------------------------
ofAppCurses::ofAppCurses(){
	ofAppPtr = NULL;
	width = 0;
	height = 0;
	inputWin = NULL;
}

//----------------------------------------------------------

ofAppCurses::~ofAppCurses(){
	// clean up the screen.
	if (inputWin != NULL) delwin(inWin);
	exit();
}

//------------------------------------------------------------
void ofAppCurses::runAppViaInfiniteLoop(ofBaseApp * appPtr){

	ofSetCurrentRenderer(shared_ptr<ofBaseRenderer>(new ofNoopRenderer));

	ofAppPtr = appPtr;

	// ncurses manages our window...
	initscr();					/* Start curses mode 		  */
	
	noecho();					/* Do not echo inputs to the main screen. */
	
	inputWin = newpad(2, 2); // creating an invisible window, so that key entry does not auto-refresh screen.

	nodelay(inWin, TRUE);		/* Set asynchronous keyboard input for virtual window */
	keypad(inWin, TRUE);

	ofNotifySetup();

	bool isRunning = true;
	while (isRunning)
	{
		int  key = wgetch(inWin); // note that this will implicitly refresh your screen.
		bool isKeyWaiting = key != ERR ? true : false;
		
		while (isKeyWaiting)
		{
			switch (key) {
				case ERR: /* no input waiting */
					break;
				default:
					ofAppPtr->keyPressed( key );
					break;
			}
			// try to get the next key.
			key = wgetch(inWin);
			isKeyWaiting = key != ERR ? true : false;
		}
		ofNotifyUpdate();
		ofNotifyDraw();
	}
}

//------------------------------------------------------------
void ofAppCurses::exit(){
	erase();
	curs_set(1);		/* make cursor visible  */
	endwin();			/* End curses mode		  */
	reset_shell_mode();
}

ofPoint	ofAppCurses::getWindowPosition(){
	return ofPoint(0,0);
}

ofPoint	ofAppCurses::getWindowSize(){
	return ofPoint(width,height);
}

ofPoint	ofAppCurses::getScreenSize(){
	return ofPoint(width,height);
}


int	ofAppCurses::getWidth(){
	return width;
}

int	ofAppCurses::getHeight(){
	return height;
}

