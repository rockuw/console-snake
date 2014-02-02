#include "game.h"
#include <unistd.h>
#include <ncurses.h>

using namespace std;

static void* MoveSnakeThreadFuncWrapper(void* arg)
{
    Game* game = (Game*)arg;
    game->MoveSnakeThreadFunc();

    return NULL;
}

Game::Game(): gameStop_(false)
{
    map_ = new Map(20, 40);
    snake_ = new Snake(map_);

    food_.type = Snake::FOOD;
    food_.x = food_.y = 10;
    
    map_->AddPaintObject(&food_);
}

Game::~Game()
{
    Stop();
    
    delete map_;
    delete snake_;
}

void Game::Start()
{
    map_->StartAutoRepaint();
    pthread_create(&moveSnakeThread_, NULL, MoveSnakeThreadFuncWrapper, this);

    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
          case 'a':
              snake_->ChangeDirection(Snake::LEFT);
              break;
          case 'd':
              snake_->ChangeDirection(Snake::RIGHT);
              break;
          case 'w':
              snake_->ChangeDirection(Snake::UP);
              break;
          case 's':
              snake_->ChangeDirection(Snake::DOWN);
              break;
        }
    }
}

void Game::Stop()
{
    gameStop_ = true;
    pthread_join(moveSnakeThread_, NULL);
}

void Game::RenewFood()
{
    int retry = 3;
    while (retry --) {
        Position newFoodPos = map_->GetRandomPos();
        if (!snake_->OccupyPos(newFoodPos)) {
            food_.x = newFoodPos.x;
            food_.y = newFoodPos.y;
            return;
        }
    }

    // Should rarely reach here !!!
    // Scan the whole map for free pos for the food
    Position newFoodPos;
    for (int i = 1; i < map_->GetRows(); i++) {
        for (int j = 1; i < map_->GetCols(); j++) {
            newFoodPos.x = i;
            newFoodPos.y = j;
            if (!snake_->OccupyPos(newFoodPos)) {
                food_.x = newFoodPos.x;
                food_.y = newFoodPos.y;
                return;
            }
        }
    }

    // Should never reach here !!!
    // The snake occupies the whole map
}

void Game::MoveSnakeThreadFunc()
{
    while (!gameStop_) {
        usleep(200 * 1000);
        Snake::Node newHead = snake_->GetNextHead();
        if ((newHead.x <= 0 || newHead.x >= map_->GetRows()) ||
            (newHead.y <= 0 || newHead.y >= map_->GetCols())) {
            Stop();
        } else if (newHead.x == food_.x && newHead.y == food_.y) {
            snake_->Eat(food_);
            RenewFood();
        } else {
            snake_->Move();
        }
    }
}

