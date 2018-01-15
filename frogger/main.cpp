#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

using namespace sf;

int tileSize = 32;

const int tileCountW = 13;

const int tileCountH = 15;

int grid[tileCountW][tileCountH]={0};

int windowWidth = tileCountW * tileSize;
int windowHeight = tileCountH * tileSize;

bool testMode = false;

struct TestReporter{
    bool reportPosition = true;
};

struct Position{
    Position(){};
    Position(int xpos,int ypos){
        x = xpos;
        y = ypos;
    }
    void Update(int xpos, int ypos){
        if(grid[xpos][ypos]>0){
            grid[xpos][ypos] -= 1;
        } else {

        }
    }
    string Print(){
        return "";
    }
    int x, y;
};

class GameEntity{
    public:
        GameEntity();

        GameEntity(string name, int position[2]){
            mName = name;
            Position mPosition(position[0],position[1]);
        }

        virtual ~GameEntity()=0;

    void GetName(){

    }

    Position getPosition(){
        return mPosition;
    }
    private:
        string mName;
        Position mPosition;
};


/*  At this point in time, this is simply a one-for-one container for Position, and isn't really necessary.
*   Keeping for now.
struct Vehicle{
    Vehicle(Position* pos){
        mPos = pos;
    }
    Position* mPos;
};
*/
int main(){
    Position playerPosition(5,14);               //Set Start Position
    TestReporter testLog;
    Texture fTexture;
    fTexture.loadFromFile("images/frog.png");

    Sprite fSprite(fTexture);
    fSprite.setPosition(playerPosition.x * tileSize,playerPosition.y * tileSize);

    RenderWindow window(VideoMode(windowWidth,windowHeight),"Totally Not That Other Frog Game");
    while(window.isOpen()){
        int moveX, moveY;
        moveX = moveY = 0;

        Event e;
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
                    break;
                case Event::KeyReleased:
                    switch(e.key.code){
                        case Keyboard::Left: 
                            if(playerPosition.x >= 1) moveX=-1;
                            break;
                        case Keyboard::Right: 
                            if(playerPosition.x < tileCountW-1) moveX=1;
                            break;
                        case Keyboard::Up: 
                            if(playerPosition.y >= 1) moveY=-1;
                            break;
                        case Keyboard::Down: 
                            if(playerPosition.y < tileCountH-1) moveY=1;
                            break;
                    }
                playerPosition.x += moveX;
                playerPosition.y += moveY;
                if(testLog.reportPosition){
                    std::cout << "Hop! Current Position: (" << playerPosition.x << "," << playerPosition.y << ")" << std::endl;
                }
            }
        }
        //Begin Draw
        window.clear();
        //--

        fSprite.setPosition(playerPosition.x*tileSize,playerPosition.y*tileSize);
        
        window.draw(fSprite);

        //End Draw
        window.display();
        //--
    };
    return 0;
};