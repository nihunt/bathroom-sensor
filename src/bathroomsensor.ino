// This #include statement was automatically added by the Particle IDE.
#include "sevensegment.h"
//Test

// This #include statement was automatically added by the Particle IDE.
#include "application.h"
#include "HttpClient/HttpClient.h"

//something

/**
* Declaring the variables.
*/

int bathroom_id = 1;
int led1 = D2;
int led2 = D7;
int numberOfStalls = 4;

unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

void setup() {
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 for (int i = 1; i < numberOfStalls+1; i++) {
     pintMode(getStallPin(i), INPUT_PULLDOWN);
 }
    Serial.begin(9600);
}

void loop() {
    if (nextTime > millis()) {
        if (isAStallClosed()) {
            setLeds(digitalRead(switchIn));
        }
        return;
    }

    nextTime = millis() + 3000;
}

void callUrlForStall(int stallNumber) {
     request.hostname = "pardot-pingpong.herokuapp.com";
    request.port = 80;
    request.path = "/bathrooms/" + String(bathroom_id) + "/stalls/" + String(stallNumber) +"/" + convertToBool(readStall(stallNumber)) + ".json";

    // Get request
    http.get(request, response, headers);
}

String convertToBool(int digitalRead) {
    if (digitalRead == 1) {
        return "true";
    } else  {
        return "false";
    }
}

void setLeds(int value) {
  digitalWrite(led1, value);
  digitalWrite(led2, value);
}

int getStallPin( int stallNumber) {
    switch (stallNumber) {
        case 1: return D0;
        case 2: return D1;
        case 3: return D2:
        case 4: return D3:
        default : return D0;
    }
}

int readStall( int stallNumber) {
    switch (stallNumber) {
        case 1: return digitalRead(getStallPin(D0));
        case 2: return digitalRead(getStallPin(D1));
        case 3: return digitalRead(getStallPin(D2)):
        case 4: return digitalRead(getStallPin(D3)):
        default : return digitalRead(getStallPin(D0));
    }
}

bool isAStallClosed() {
    for (int i = 1; i< numberOfStalls+1; i++) {
        if (readStall(i) == HIGH) {
            return true;
        }
    }
    return false;
}
