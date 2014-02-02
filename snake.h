#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <deque>
#include "map.h"
#include "lock.h"

class Snake: public PaintObject
{
    static const char kHeadChar = '*';
    static const char kBodyChar = 'O';
    static const char kFoodChar = 'x';
    
  public:
    enum NodeType
    {
        HEAD = 0,
        BODY,
        FOOD
    };

    struct Node: public PaintObject
    {
        NodeType type;
        int x, y;

        void DrawSelf(Map* map)
        {
            char ch;
            switch (type) {
              case HEAD:
                  ch = kHeadChar;
                  break;
              case BODY:
                  ch = kBodyChar;
                  break;
              case FOOD:
                  ch = kFoodChar;
                  break;
            }
            map->Draw(x, y, ch);
        }
    };

    enum Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

  public:
    Snake(Map* map);
    virtual ~Snake();
    
    void Move();
    void ChangeDirection(Direction dir);
    void DrawSelf(Map* map);
    Node GetNextHead();
    void Eat(const Node& food);
    bool OccupyPos(const Position& pos);

  private:
    Map* map_;
    Node head_;
    std::deque<Node> body_;
    Direction dir_;
    Mutex lock_;
};


#endif /* _SNAKE_H_ */
