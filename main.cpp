#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = {173,204,96,255};
Color darkGreen = {43,51,24,255};

int cellsize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool eventTrigger(double interval){
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
};

class Food{
    public:
    Vector2 position;

    Food(deque<Vector2> snakeBody){
    position =  generateRandomPos(snakeBody);
    }

    ~Food(){

    }

    void draw(){
        DrawRectangle(offset+position.x*cellsize, offset+position.y*cellsize,cellsize,cellsize,darkGreen);
    }

    Vector2 generateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        Vector2 position = Vector2{x,y};
        return position;
    }

    Vector2 generateRandomPos(deque<Vector2> snakebody){

        Vector2 position = generateRandomCell();

        while(ElementInDeque(position, snakebody)){
            position = generateRandomCell();
        }
        return position;
    }
};

class Snake{
    public:
    deque<Vector2> body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSegment = false;

    void draw(){
        for(unsigned int i = 0; i < body.size(); i++)
        {
            int x = body[i].x;
            int y = body[i].y;
            DrawRectangle(offset+x*cellsize,offset+y*cellsize, cellsize,cellsize, darkGreen);
        }
    }
    void update(){
    
        if(addSegment == true){
            body.push_front(Vector2Add(body[0],direction));
            addSegment = false;
        }
        else{
            body.pop_back();
            body.push_front(Vector2Add(body[0],direction));
        }
        
    }
        void reset(){
        body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        direction = {1,0};
    }
};

class Game{
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;

    
    void draw(){
        food.draw();
        snake.draw();
    }

    void update(){
        if(running == true){
            snake.update();
            checkCollisionWithFood();
            checkCollisionWithEdge();
            checkCollisionWithBody();
        }
    }

    void checkCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.generateRandomPos(snake.body);
            snake.addSegment = true;
            score ++;
        }
    }

    void checkCollisionWithEdge(){
        if(snake.body[0].x == cellCount || snake.body[0].y == -1){
            gameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            gameOver();
        }
    }

    void checkCollisionWithBody(){
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if( ElementInDeque(snake.body[0], headlessbody)){
            gameOver();
        }
    }

    void gameOver(){
        cout<<"Game Over"<<endl;
        snake.reset();
        running = false;
        score = 0;
    }

};

int main () {
    cout << "Starting game...";
    InitWindow(2*offset + cellsize*cellCount,2*offset + cellsize*cellCount, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        BeginDrawing();
        if(eventTrigger(0.2)){
            game.update();
        }

        // Controls
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
            game.running = true;
        }
        else if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
            game.running = true;
        }
        else if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
            game.running = true;
        }
        else if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
            game.running = true;
        }


        ClearBackground(green);
        
        game.draw();
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5,(float)cellsize*cellCount,(float)cellsize*cellCount}, 5, darkGreen);
        DrawText(TextFormat("%i", game.score), offset-5, 20, 40, darkGreen);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}