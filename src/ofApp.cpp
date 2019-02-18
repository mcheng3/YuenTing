#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	settings = ofTrueTypeFontSettings("wqy-microhei.ttc", 15);
	settings.antialiased = true;
	settings.addRange(ofUnicode::Latin1Supplement);
	settings.addRange(ofUnicode::LatinExtendedAdditional);
	settings.addRange(ofUnicode::GeneralPunctuation);
	settings.addRange(ofUnicode::CJKUnified);
	settings.addRange(ofUnicode::CJKLettersAndMonths);
	settings.addRange(ofUnicode::Uncategorized);

	font.load(settings);

	load_button.addListener(this, &ofApp::LoadFile);


	gui.setup(); // most of the time you don't need a name
	gui.add(variety_label.setup("Variety", ""));
	gui.add(cantonese_toggle.setup("Cantonese", true));
	gui.add(mandarin_toggle.setup("Mandarin", false));
	gui.add(korean_toggle.setup("Korean", false));
	gui.add(set_label.setup("Character set", ""));
	gui.add(traditional_toggle.setup("Traditional", true));
	gui.add(simplified_toggle.setup("Simplified", false));
	gui.add(load_button.setup("Load text file"));

	curr_variety_toggle = &cantonese_toggle;
	curr_set_toggle = &traditional_toggle;
	variety_toggles[&cantonese_toggle] = "cantonese";
	variety_toggles[&mandarin_toggle] = "mandarin";
	variety_toggles[&korean_toggle] = "korean";

	set_toggles[&traditional_toggle] = "traditional";
	set_toggles[&simplified_toggle] = "simplified";
	settings.fontSize = 16;
	font.load(settings);
}

void ofApp::ToggleRadioStyle(ofxToggle*& curr_toggle, map<ofxToggle*, string> toggles) {
	for(auto each : toggles) {
		auto toggle = each.first;
		if(toggle != curr_toggle) {
			if(*toggle == true) {
				*curr_toggle = false;
				curr_toggle = toggle;
			} 
		}
	}
}

void ofApp::LoadFile() {
	ofFileDialogResult result = ofSystemLoadDialog("Select chinese text file");
	if(result.bSuccess) {
		string path = result.getPath();
		ifstream txt_file(path);
		string chinese_text((std::istreambuf_iterator<char>(txt_file)), std::istreambuf_iterator<char>());
		SetupTranslations(chinese_text);
	}
}

void ofApp::SetupTranslations(string& chinese_text) {
	string variety = variety_toggles[curr_variety_toggle];
	dict = Dictionary(variety, traditional_toggle);
	translations = dict.TranslateToEnglish(chinese_text);
}

//--------------------------------------------------------------
void ofApp::update(){
	ToggleRadioStyle(curr_variety_toggle, variety_toggles);
	ToggleRadioStyle(curr_set_toggle, set_toggles);
}

//--------------------------------------------------------------
void ofApp::draw(){
	DrawText();
	gui.draw();
	
}

void ofApp::DrawText() {
	string pos = to_string(mouse_x) + ", " + to_string(mouse_y);
	ofSetColor(0, 0, 0);
	int line_width = 0;
	float line_height = font.getLineHeight();
	int row = 0;

	for(auto entry : translations) {
		string chinese_word = entry[0];
		if(line_width > 600 || chinese_word == "\n"){
			row++;
			line_width = 0;
		}
		chinese_word += "​"; //Add zero-width space to make punctuation render as if there is a character in front of it.
		int string_x = 120 + line_width;
		int string_y =  230 + line_height*row;
		if(!isinf(font.stringWidth(chinese_word))){
			line_width = line_width + font.stringWidth(chinese_word);
		}
		ofRectangle rect = font.getStringBoundingBox(chinese_word, string_x, string_y);
		if((mouseX>rect.x && mouseX<rect.x+rect.width)&&(mouseY>rect.y && mouseY<rect.y+rect.height)){ 
			ofSetColor(255, 0, 0);
			font.drawString(chinese_word, string_x, string_y);
			ofSetColor(0, 0, 0);
			DrawTranslations(entry);
		} else {
			font.drawString(chinese_word, string_x, string_y);
		}		
	}
}

void ofApp::DrawTranslations(vector<string> entry) {
	int entry_y = 170;
	for(int i = 1; i < entry.size(); i++) {
		string trans_entry = entry[i];
		string entry_line;
		int color;
		if(i == 1) {
			ofSetColor(29, 1, 173);
			entry_line = "     \n";
			entry_line += trans_entry;
			font.drawString(entry_line, 860, entry_y);
			ofSetColor(0, 0, 0);
			entry_y += font.getLineHeight() * 2;
		} else {
			color = 0;
			entry_line = to_string(i-1) + ". ";
			entry_line += trans_entry;
			ofxParagraph paragraph = ofxParagraph(entry_line, 800);
			paragraph.setFont("wqy-microhei.ttc", 16);
			paragraph.setColor(color);
			paragraph.draw(810, entry_y);
			entry_y += paragraph.getHeight();
		}

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	mouse_x = x;
	mouse_y = y;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
