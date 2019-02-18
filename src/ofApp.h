#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxParagraph.h"
#include "dictionary.h"

#include <fstream>
#include <string>
#include <streambuf>
#include <map>
#include <vector>
#include <math.h>

class ofApp : public ofBaseApp{
	private:
		Dictionary dict;
		std::vector<std::vector<std::string>> translations;
		bool traditional;

		ofTrueTypeFont font;
		ofxButton load_button;
		ofxToggle traditional_toggle;
		ofxToggle simplified_toggle;
		ofxToggle mandarin_toggle;
		ofxToggle cantonese_toggle;
		ofxToggle korean_toggle;
		ofxToggle* curr_variety_toggle;
		ofxToggle* curr_set_toggle;
		ofxLabel variety_label;
		ofxLabel set_label;
		map<ofxToggle*, string> variety_toggles;
		map<ofxToggle*, string> set_toggles;

		ofTrueTypeFontSettings settings{"wqy-microhei.ttc", 20};

		int mouse_x;
		int mouse_y;

	public:
		void setup();
		void update();
		void draw();

		void LoadFile();
		void SetupTranslations(std::string& chinese_text);
		void ToggleRadioStyle(ofxToggle*& curr_toggle, std::map<ofxToggle*, string> toggles);
		void DrawText();
		void DrawTranslations(vector<string> entry);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool bHide;


		ofxPanel gui;


};
