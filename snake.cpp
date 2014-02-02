#include "snake.h"
#include "map.h"

using namespace std;

Snake::Snake(Map* map): map_(map)
{
    head_.x = 1;
    head_.y = 4;
    head_.type = HEAD;

    Node b;
    b.type = BODY;
    b.x = head_.x;
    b.y = head_.y - 1;
    body_.push_back(b);

    --b.y;
    body_.push_back(b);

    dir_ = RIGHT;

    map_->AddPaintObject(this);
}

Snake::~Snake()
{
}

void Snake::ChangeDirection(Direction dir)
{
    if ((dir_ == LEFT && dir == RIGHT) ||
        (dir_ == RIGHT && dir == LEFT) ||
        (dir_ == UP && dir == DOWN) ||
        (dir_ == DOWN && dir == UP)) {
        return;
    }

    dir_ = dir;
}

void Snake::Move()
{
    Node newHead = GetNextHead();
    Node oldHead = head_;
    oldHead.type = BODY;

    ScopedLock lock(&lock_);
    
    body_.push_front(oldHead);
    body_.pop_back();
    head_ = newHead;
}

void Snake::Eat(const Node& food)
{
    Node newHead;
    newHead.type = HEAD;
    newHead.x = food.x;
    newHead.y = food.y;

    Node oldHead = head_;
    oldHead.type = BODY;

    ScopedLock lock(&lock_);
    
    head_ = newHead;
    body_.push_front(oldHead);
}

bool Snake::OccupyPos(const Position& pos)
{
    if (head_.x == pos.x && head_.y == pos.y) {
        return true;
    }
    
    for (typeof(body_.begin()) it = body_.begin();
         it != body_.end();
         ++it) {
        if (it->x == pos.x && it->y == pos.y) {
            return true;
        }
    }

    return false;
}

Snake::Node Snake::GetNextHead()
{
    Node newHead;
    newHead.type = HEAD;
    
    switch (dir_) {
      case LEFT:
          newHead.x = head_.x;
          newHead.y = head_.y - 1;
          break;
      case RIGHT:
          newHead.x = head_.x;
          newHead.y = head_.y + 1;
          break;
      case UP:
          newHead.x = head_.x - 1;
          newHead.y = head_.y;;
          break;
      case DOWN:
          newHead.x = head_.x + 1;
          newHead.y = head_.y;
          break;
    }

    return newHead;
}

void Snake::DrawSelf(Map* map)
{
    map->Draw(head_.x, head_.y, kHeadChar);

    for (typeof(body_.begin()) it = body_.begin();
         it != body_.end();
         ++it) {
        map->Draw(it->x, it->y, kBodyChar);
    }
}
