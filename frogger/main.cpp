/** main.cpp
 * Main script for Frogger clone
 * Contains all class definitions and implementation
**/
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;

using namespace sf;

int tileSize = 32;

const int tileCountW = 13;

const int tileCountH = 15;

int grid[tileCountW][tileCountH]={0};

int windowWidth = tileCountW * tileSize;
int windowHeight = tileCountH * tileSize;

//bool testMode = false;

struct TestReporter{
    bool reportPosition = true;
    bool reportCollision = true;
    bool reportNonCollision = true;
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
        std::stringstream ss;
        ss << "[" << x << "," << y << "]";
        return ss.str();
    }
    void Set(int intx,int inty){
        x = intx;
        y = inty;
    }
    int x, y;
};

class GameEntity{
    public:
        GameEntity(){};

        GameEntity(string name, int position[2], Texture* tex){
            mName = name;
            mPosition=Position(position[0],position[1]);
            mSprite = Sprite(*tex);
        }

    string getName(){
        return mName;
    }
    void setName(string n){
        mName = n;
    }

    bool checkCollision(GameEntity* target){
        return mSprite.getGlobalBounds().intersects(target->getSprite()->getGlobalBounds());
    }

    Position* getPosition(){
        return &mPosition;
    }

    void setPosition(int arr[2]){
        mPosition.x = arr[0];
        mPosition.y = arr[1];
    }

    void setPosition(Position pos){
        mPosition.Set(pos.x,pos.y);
    }

    Sprite* getSprite(){
        return &mSprite; 
    }

    void setSprite(Sprite s){
        mSprite = s;
    }

    private:
        string mName;
        Position mPosition;
        Sprite mSprite;
};

class Enemy : public GameEntity{
    public:
        
        Enemy():GameEntity(){}
        Enemy(string name, int pos[2], Texture* tex):GameEntity(name,pos,tex){}
        Enemy(string name, int pos[2], Texture* tex, int direction, int spd) : Enemy(name,pos,tex){
            dir = direction;
            speed = spd;
            startPosition.Set(pos[1],pos[2]);
        }
        Position startPosition;
        bool activeThreat = true;
        int dir;
        int speed;
        void boundsCheck(){
            int bounds;
            dir > 0 ? bounds = (tileCountW * tileSize) + tileSize * 2 : bounds = -tileSize * 2;
            if (abs(getPosition()->x)>abs(bounds)){
                setPosition(startPosition);
            }
        }
};

Enemy enemyList[10];

int main(){

    TestReporter testLog;
    int startPosition[2]{6,14};
    Texture pTexture;
    pTexture.loadFromFile("images/frog.png");
    GameEntity Player("Player",startPosition,&pTexture);

    Position* playerPosition = Player.getPosition();
    Sprite* pSprite = Player.getSprite();

    int ePosition[2]{6,8};
    string enemyName = "enemy";
    Enemy enemy(enemyName,ePosition,&pTexture);
    Position* enemyPosition = enemy.getPosition();
    Sprite* eSprite = enemy.getSprite();
    

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
                            if(playerPosition->x >= 1) moveX=-1;
                            break;
                        case Keyboard::Right: 
                            if(playerPosition->x < tileCountW-1) moveX=1;
                            break;
                        case Keyboard::Up: 
                            if(playerPosition->y >= 1) moveY=-1;
                            break;
                        case Keyboard::Down: 
                            if(playerPosition->y < tileCountH-1) moveY=1;
                            break;
                    }
                playerPosition->x += moveX;
                playerPosition->y += moveY;
                if(testLog.reportPosition){
                    std::cout << "Hop! Current Position: (" << playerPosition->x << "," << playerPosition->y << ")" << std::endl;
                }
                bool collision = Player.checkCollision(&enemy);
            }
        }

        if(Player.checkCollision(&enemy)){
            //std::cout << "Collision!" << std::endl;
        }

        //Begin Draw
        window.clear();
        //--
        
        pSprite->setPosition(playerPosition->x*tileSize,playerPosition->y*tileSize);
        eSprite->setPosition(enemy.getPosition()->x*tileSize,enemy.getPosition()->y*tileSize);
        window.draw(*eSprite);
        window.draw(*pSprite);

        //End Draw
        window.display();
        //--
    };
    return 0;
};