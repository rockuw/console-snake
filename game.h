#ifndef _GAME_H_
#define _GAME_H_

#include <pthread.h>
#include "snake.h"
#include "map.h"

class Game
{
  public:
    Game();
    virtual ~Game();

    void MoveSnakeThreadFunc();
    void Start();
    void Stop();

  private:
    void RenewFood();
    
  private:
    Map* map_;
    Snake* snake_;
    Snake::Node food_;
    bool gameStop_;
    pthread_t moveSnakeThread_;
};


#endif /* _GAME_H_ */
