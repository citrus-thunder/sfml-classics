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

int windowWidth = tileCountW * tileSize;
int windowHeight = tileCountH * tileSize;


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
    int basis=1;
    bool isAbstract;
};

class GameEntity{
    public:
        GameEntity(){};

        GameEntity(string name, int position[2], Texture* tex){  //to be retired
            mName = name;
            mPosition=Position(position[0],position[1]);
            mSprite = Sprite(*tex);
        }
        GameEntity(string name, std::vector<int> position, Texture* tex){ //to be retired
            mName = name;
            mPosition = Position(position.at(0),position.at(1));
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
        getSprite()->setPosition(mPosition.x,mPosition.y);
    }

    void setPosition(std::vector<int> arr){
        mPosition.x = arr[0];
        mPosition.y = arr[1];
        getSprite()->setPosition(mPosition.x,mPosition.y);
    }

    void setPosition(Position pos){
        mPosition.Set(pos.x,pos.y);
        getSprite()->setPosition(mPosition.x,mPosition.y);
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
        Enemy(string name, std::vector<int> pos, Texture* tex, int direction, int spd) : Enemy(name,pos,tex){
            dir = direction;
            speed = spd;
            startPosition.Set(pos[0],pos[1]);
            if(dir<0){
                startPosition.Set(tileCountW * tileSize, getPosition()->y);
            } else {
                startPosition.Set(-tileSize,getPosition()->y);
            }
        }
        Enemy(string name, std::vector<int> pos, Texture* tex):GameEntity(name,pos,tex){}
        Position startPosition;
        bool activeThreat = true;
        int dir;
        int speed = 1;
        void boundsCheck(){
            bool outOfBounds = false;
            if(dir > 0){
                if(getPosition()->x > (tileCountW * tileSize)+tileSize * 2) outOfBounds = true;
            } else {
                if(getPosition()->x < -tileSize * 2) outOfBounds = true;
            }
            if (outOfBounds){
                setPosition(startPosition);
                //std::cout << getName() << " is out of bounds!" << std::endl;
            }
        }
};



int enemySpeed;

int main(){

    TestReporter testLog;
    int startPosition[2]{6,14}; //6,14 for bottom-middle
    Texture pTexture;
    pTexture.loadFromFile("images/frog.png");
    GameEntity Player("Player",startPosition,&pTexture);

    Position* playerPosition = Player.getPosition();
    Sprite* pSprite = Player.getSprite();

    int tickDelay = 5;
    int tick = tickDelay;

    Texture eTexture;
    eTexture.loadFromFile("images/car.png");

    Enemy enemyList[5][3] { //Consider swapping to a vector so a jagged array can be made easily
        {
            {"E5-1",std::vector<int>{(tileCountW-1) * tileSize, 9 * tileSize}, &eTexture, -1,3},
            {"E5-2",std::vector<int>{(tileCountW-4) * tileSize, 9 * tileSize}, &eTexture, -1,3},
            {"E5-3",std::vector<int>{(tileCountW-7) * tileSize, 9 * tileSize}, &eTexture, -1,3}
        },
        {
            {"E4-1",std::vector<int>{1 * tileSize, 10 * tileSize}, &eTexture,1,6},
            {"E4-2",std::vector<int>{4 * tileSize, 10 * tileSize}, &eTexture,1,6},
            {"E4-3",std::vector<int>{7 * tileSize, 10 * tileSize}, &eTexture,1,6}
        },
        {
            {"E3-1",std::vector<int>{(tileCountW-1) * tileSize, 11 * tileSize}, &eTexture, -1,4},
            {"E3-2",std::vector<int>{(tileCountW-4) * tileSize, 11 * tileSize}, &eTexture, -1,4},
            {"E3-3",std::vector<int>{(tileCountW-7) * tileSize, 11 * tileSize}, &eTexture, -1,4}
        },
        { 
            {"E2-1",std::vector<int>{1 * tileSize, 12 * tileSize}, &eTexture,1,5},
            {"E2-2",std::vector<int>{4 * tileSize, 12 * tileSize}, &eTexture,1,5},
            {"E2-3",std::vector<int>{7 * tileSize, 12 * tileSize}, &eTexture,1,5}
        },
        {
            {"E1-1", std::vector<int>{(tileCountW-1) * tileSize, 13 * tileSize}, &eTexture, -1, 2},
            {"E1-2", std::vector<int>{(tileCountW-4) * tileSize, 13 * tileSize}, &eTexture, -1, 2},
            {"E1-3", std::vector<int>{(tileCountW-7) * tileSize, 13 * tileSize}, &eTexture, -1, 2}
        }
    };

    Enemy platformList[5][3] { 
        {
            {"P5-1",std::vector<int>{(tileCountW-1) * tileSize, 3 * tileSize}, &eTexture, -1,3},
            {"P5-2",std::vector<int>{(tileCountW-4) * tileSize, 3 * tileSize}, &eTexture, -1,3},
            {"P5-3",std::vector<int>{(tileCountW-7) * tileSize, 3 * tileSize}, &eTexture, -1,3}
        },
        {
            {"P4-1",std::vector<int>{1 * tileSize, 4 * tileSize}, &eTexture,1,6},
            {"P4-2",std::vector<int>{4 * tileSize, 4 * tileSize}, &eTexture,1,6},
            {"P4-3",std::vector<int>{7 * tileSize, 4 * tileSize}, &eTexture,1,6}
        },
        {
            {"P3-1",std::vector<int>{(tileCountW-1) * tileSize, 5 * tileSize}, &eTexture, -1,4},
            {"P3-2",std::vector<int>{(tileCountW-4) * tileSize, 5 * tileSize}, &eTexture, -1,4},
            {"P3-3",std::vector<int>{(tileCountW-7) * tileSize, 5 * tileSize}, &eTexture, -1,4}
        },
        { 
            {"P2-1",std::vector<int>{1 * tileSize, 6 * tileSize}, &eTexture,1,5},
            {"P2-2",std::vector<int>{4 * tileSize, 6 * tileSize}, &eTexture,1,5},
            {"P2-3",std::vector<int>{7 * tileSize, 6 * tileSize}, &eTexture,1,5}
        },
        {
            {"P1-1", std::vector<int>{(tileCountW-1) * tileSize, 7 * tileSize}, &eTexture, -1, 2},
            {"P1-2", std::vector<int>{(tileCountW-4) * tileSize, 7 * tileSize}, &eTexture, -1, 2},
            {"P1-3", std::vector<int>{(tileCountW-7) * tileSize, 7 * tileSize}, &eTexture, -1, 2}
        }
    };

    for(int i=0;i<5;i++){
        for(int j=0; j<3; j++){
            if(enemyList[i][j].dir>0){
                enemyList[i][j].getSprite()->setScale(-1,1);
            }
            if(platformList[i][j].dir>0){
                platformList[i][j].getSprite()->setScale(-1,1);
            }
        }
    }

    RenderWindow window(VideoMode(windowWidth,windowHeight),"Totally Not That Other Frog Game");
    window.setFramerateLimit(60);
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
            }
        }
        if(tick <= 0){
            for(int i = 0;i < 5; i++){
                for (int j=0; j < 3; j++){
                    Enemy * e = &enemyList[i][j];
                    Position newPos(e->getPosition()->x + (e->speed * e->dir),e->getPosition()->y);
                    e->setPosition(newPos);

                    Enemy * p = &platformList[i][j];
                    Position PnewPos(p->getPosition()->x + (p->speed * p->dir),p->getPosition()->y);
                    p->setPosition(PnewPos);


                    e->boundsCheck();
                    p->boundsCheck();
                    if(e->checkCollision(&Player)) Player.setPosition(startPosition); //expensive collision check. todo-- switch to only check against enemies in current row
                    if(p->checkCollision(&Player)) Player.setPosition(std::vector<int>{(Player.getPosition()->x/tileSize)+(p->dir*p->speed),Player.getPosition()->y});
                    //if(p->checkCollision(&Player)) Player.setPosition(*p->getPosition());
                }
            }
        tick = tickDelay;
      }
      tick -= 1;

        //Begin Draw
        window.clear();
        //--
        
        for(int i = 0; i < 5; i++){
            for (int j=0; j<3; j++){
                Enemy * e = &enemyList[i][j];
                window.draw(*e->getSprite());

                Enemy * p = &platformList[i][j];
                window.draw(*p->getSprite());
            }
        }

        pSprite->setPosition(playerPosition->x*tileSize,playerPosition->y*tileSize); //translate "local" position to pixel position

        window.draw(*pSprite);

        //End Draw
        window.display();
        //--
    };
    return 0;
};