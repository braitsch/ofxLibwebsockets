#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    //bool connected = server.setup( 9092 );
    
    // Uncomment this to set up a server with a protocol
    // Right now, clients created via libwebsockets that are connecting to servers
    // made via libwebsockets seem to want a protocol. Hopefully this gets fixed, 
    // but until now you have to do something like this:
    
    //setup video grabber
    video.listDevices();
    bVideoSetup = video.initGrabber(640,480);
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9093;
    bool connected = server.setup( options );
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    // setup message queue
    font.loadFont("myriad.ttf", 20);
    messages.push_back("WebSocket server setup at "+ofToString( server.getPort() ) + ( server.usingSSL() ? " with SSL" : " without SSL") );
    
    ofBackground(0);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if ( bVideoSetup && video.isFrameNew() ){
        static unsigned long size;
        static ofImage currentImage;
        currentImage.setFromPixels(video.getPixelsRef());
        
        // compress video into image via turbojpg
        // the second param == quality. play with this to get a better framerate
        // you can also try resizing your image!
        //currentImage.resize(160, 120);
        unsigned char * compressed = turbo.compress(currentImage,50,&size);
        server.sendBinary(compressed, size);
        free(compressed);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int x = font.getSize();
    int y = font.getSize()*2;
    ofSetColor(255);
    for (int i = messages.size() -1; i >= 0; i-- ){
        //font.drawString( messages[i], x, y );
        y += font.stringHeight( messages[i] ) + font.getSize();
    }
    if ( bVideoSetup ) video.draw(0,0);
      ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    messages.push_back("New connection from " + args.conn.getClientIP() );
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
//    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    // trace out string messages or JSON messages!
    if ( !args.json.isNull() ){
        messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
    } else {
        messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
    }
    
    //if (messages.size() > NUM_MESSAGES) messages.erase( messages.begin() );
    
    // echo server = send message right back!
    args.conn.send( args.message );
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
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
void ofApp::dragEvent(ofDragInfo dragInfo){}