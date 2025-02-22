/*
 * Networking for Cubior
 * by Brian Handy
 * 5/25/13
 * Networking class for 3d platformer
 * most code copied from enet tutorials
 */

// NetworkTest.cpp : main project file.
// Most code copied from Enet tutorial
// Copying done by Brian Handy, 5/4/13

// had to add directory "include" to VC++ Project Properties > Directories
// had to add enet.lib, winmm.lib, ws2_32.lib to Project Properties > Linker > Input top bar

// This now works with itself, but not with a copy on the same machine. How about another machine?

#include <cstdlib> // atoi
#include <iostream>
#include <enet/enet.h>
#include <string>
#include <cmath>
#include <vector>
#include "networking.h"
#include "gameplay.h" // only for CubiorCount

using namespace std;

// Preset variables
bool reconnectOnDisconnect = true;

// Declare Enet Variables
ENetAddress addressServer;
ENetAddress addressClient;
ENetHost * server;
ENetHost * client;
ENetPeer * peer;
const int messageSize = 1024;
char message[messageSize];

// Declare my code's variables
string role;
bool choseHost;
bool keepLooping;
string oldAddress = "127.0.0.1";
bool connected = false;
int ticks = 0;
bool hostExists = false;
string latestData;
// Recieving
int player = -1;
int posX, posY, posZ;
// Sending
int myPlayer;
int myPosX, myPosY, myPosZ;
bool onlineStatus[cubiorCount];
vector<float> momentum (3, 0);
vector<float> myMomentum (3, 0);
float direction;
float myDirection;

int currentMessageSlot = 0; // for reading in data from a message

void pollFor(ENetHost * host, ENetAddress address) {
  // Host Polling from Enet
    ENetEvent event;
    bool updatePos = false;
    // Wait up to 1000 milliseconds for an event.
    while (enet_host_service (host, & event, 1) > 0) // used to be 1000, not 1 (update 1/sec. Now it's 1000/sec)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            //cout << "A new client connected from " << event.peer -> addressServer.host <<
            //  ":" << event.peer -> addressServer.port << ".\n";
            cout << event.peer -> address.host << " joined the game\n";
            // Store any relevant client information here.
            //event.peer -> data = "Client information";
            connected = true;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            //cout << "A packet of length " << event.packet -> dataLength <<
            //  " containing " << event.packet -> data << " was received from " <<
            //  event.peer -> data << " on channel " << event.channelID << ".\n";

            //cout << event.peer -> data << ": " << event.packet -> data << "\n";
            // Save some data for later
            latestData = ((const char*)event.packet -> data);
            updatePos = true;
            // Clean up the packet now that we're done using it.
            enet_packet_destroy (event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            cout << event.peer -> data << " disconected.\n";
            // Reset the peer's client information.
            event.peer -> data = NULL;
            connected = false;
            break;
        default:
            cout << "No new data" << endl;
        }
    }
    
    if (updatePos) {
      int commaOne, commaTwo, commaThree;
      float momentumX, momentumY, momentumZ;
      string str(latestData);
      const int resultSize = 10;
      string dataArray[resultSize];
      stringToArray(str, dataArray, resultSize);
      
      //for (int i=0; i<resultSize; i++) {
      //  cout << "Line " << i << " is " << dataArray[i] << endl;
      //}
      
        
      for (int v = 0; v<resultSize; v++) {
        cout << endl << "Getting msg[" << v << "] = " << dataArray[v];
      }
      if (resultSize > 0) {
        cout << endl;
      }

      resetSlots();
      player    = getNextSlot(dataArray);
      posX      = getNextSlot(dataArray);
      posY      = getNextSlot(dataArray);
      posZ      = getNextSlot(dataArray);
      momentumX = getNextSlot(dataArray);
      momentumY = getNextSlot(dataArray);
      momentumZ = getNextSlot(dataArray);
      direction = getNextSlot(dataArray);
      
      float momentumArray[] = { momentumX, momentumY, momentumZ };
      std::vector<float> newMomentum (momentumArray, momentumArray + sizeof(momentumArray) / sizeof(float) );
      momentum.swap(newMomentum);

      //cout << " Made the positions " << posX << " / " << posY << " / " << posZ << endl;

    }

    // Get ready for next loop!
    enet_packet_destroy(event.packet);
}

void resetSlots() {
  currentMessageSlot = 0;
}

int getNextSlot(string dataArray[]) {
  int result = atoi(dataArray[currentMessageSlot].c_str());
  currentMessageSlot++;
  return result;
}

string* stringToArray(string str, string* result, int resultSize) {
    int leftComma = 0;
    int rightComma = 0;

    //cout << "String " << str << endl;
    
    for (int i=0; i<resultSize; i++) {
        result[i] = "\0";
    }

    for (int i=0; i<resultSize; i++) {
        string focus = str.substr(leftComma, str.length()-leftComma);
        rightComma = focus.find(',');
        result[i] = str.substr(leftComma, rightComma);
        if (rightComma == string::npos) {
          return result;
        }
        leftComma += rightComma+1;
    }
    return result;
}

int findComma(int commaNum, string text) {
  for (int i=0; i<text.length(); i++) {
    if (text.at(i) == ',') {
      commaNum--;
      if (commaNum == 0) {
        return i;
      }
    }
  }
  return -1;
}

void setOnline(int i) {
    myPlayer = i;
}
void setPosX(int i) {
    myPosX = i;
}
void setPosY(int i) {
    myPosY = i;
}
void setPosZ(int i) {
    myPosZ = i;
}
int getPosX() {
    return posX;
}
int getPosY() {
    return posY;
}
int getPosZ() {
    return posZ;
}
bool getOnline(int i) {
    return player == i;
}

void setMomentum(vector<float> m) {
  myMomentum = m;
}
vector<float> getMomentum() {
  return momentum;
}

void setDirection(float f) {
  myDirection = f;
}
float getDirection() {
  return direction;
}

// The main loop, called repeatedly
void networkTick() {
    
    //cout << "Network is " << (connected ? "" : "not ") << "connected" << endl;
  
    ticks++;

    // Listen for any info for server and client
    //if (choseHost) {
        pollFor(server, addressServer);
    //} else {
        pollFor(client, addressClient);
    //}
    
    // If you need to, feel free to reconnect on a connection loss
    if (!connected && reconnectOnDisconnect) {
        connectTo(oldAddress);
    }
    
    // Then send data if client
    //if (!choseHost) {
      //cout << "Send > ";
      //cin.getline(message, messageSize);
      //message = memcpy(message,newString.c_str(),newString.size());ticks;
      //itoa(ticks, message, 10);
      //string ticksString = to_string(ticks);
      //message = memcpy(message, ticksString.c_str(), ticksString.size());
    
      //int posY = sin(ticks/1000.0*3.14*2)*250+250; // fly up and down in 0 to 500 range
      sprintf(message, "%d,%d,%d,%d,%f,%f,%f,%f", 
        myPlayer,
        myPosX, myPosY, myPosZ,
        myMomentum.at(0), myMomentum.at(1), myMomentum.at(2),
        myDirection);

      ENetPacket* packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);

      //cout << "Sending msg " << message << endl;

      // No packages will be sent until your game is started
      if (strlen(message) > 0 && getStarted() && !getCubiorOnline(myPlayer)) {
          enet_peer_send(peer, 0, packet);
      }
    //}
    
}

int connectTo(string newAddress)
{
    //cout << "Trying to connect to " << newAddress << endl;
    if (!hostExists && !connected) {
        //cout << "No host yet" << endl;
        // Initialize Enety
        if (enet_initialize () != 0)
        {
            fprintf (stderr, "An error occurred while initializing ENet.\n");
            return EXIT_FAILURE;
        }
        atexit (enet_deinitialize);
        //cout << "Setup the exit route" << endl;

    //if (choseHost) {

        // Bind the server to the default localhost.
        // A specific host address can be specified by
        // enet_address_set_host (& addressServer, "x.x.x.x");
        addressServer.host = ENET_HOST_ANY;
        // Bind the server to port 1234.
        addressServer.port = 1234;
        server = enet_host_create (& addressServer, // the address to bind the server host to
                                     32,      // allow up to 32 clients and/or outgoing connections
                                      2,      // allow up to 2 channels to be used, 0 and 1
                                      0,      // assume any amount of incoming bandwidth
                                      0       // assume any amount of outgoing bandwidth
                                  );
        if (server == NULL)
        {
            fprintf (stderr, 
                     "An error occurred while trying to create an ENet server host.\n");
            exit (EXIT_FAILURE);
        }

    //} else {
        client = enet_host_create (NULL, // create a client host
                    1, // only allow 1 outgoing connection
                    2, // allow up 2 channels to be used, 0 and 1
                    57600 / 8, // 56K modem with 56 Kbps downstream bandwidth
                    14400 / 8  // 56K modem with 14 Kbps upstream bandwidth
                    );
        if (client == NULL)
        {
            fprintf (stderr, 
                     "An error occurred while trying to create an ENet client host.\n");
            exit (EXIT_FAILURE);
        }

        // Shortcut for same is an empty string
        if (newAddress.compare("") == 0) {
          newAddress = oldAddress;
        }

        enet_address_set_host (& addressClient, newAddress.c_str());
        addressClient.port = 1234;
        

        peer = enet_host_connect(client, & addressClient, 2, 0);

        if (peer == NULL) {
            fprintf (stderr,
                     "No available peers for initializing an ENet connection");
            exit (EXIT_FAILURE);
        }
        //}

        // Main Loop
        //keepLooping = true;
        //while (keepLooping) {
        //    tick();
        //}
    
        hostExists = true;
        oldAddress = newAddress;
        connected  = true;
    } else if (!connected) {
        peer = enet_host_connect(client, & addressClient, 2, 0);
        
        if (peer == NULL) {
            fprintf (stderr,
                     "No available peers for initializing an ENet connection");
            exit (EXIT_FAILURE);
        }
        connected = true;
    }

    return 0;
}

void disconnectFrom(string newAddress) {
    // Finish using Enet
    enet_host_destroy(client);
    enet_host_destroy(server);

}

bool isConnected() {
    return connected;
}