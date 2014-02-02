#ifndef _MAP_H_
#define _MAP_H_

#include <pthread.h>
#include <vector>

class Map;

class PaintObject
{
  public:
    virtual ~PaintObject() {}
    virtual void DrawSelf(Map* map) = 0;
};

class Map;

class Border: public PaintObject
{
    static const char kUpBorderChar = '-';
    static const char kDownBorderChar = '-';
    static const char kLeftBorderChar = '|';
    static const char kRightBorderChar = '|';
    
  public:
    Border(Map* map);

    void DrawSelf(Map* map);

  private:
    int rows_, cols_;
};

struct Position
{
    int x;
    int y;
};

class Map
{
    static const int kMapRepaintInterval = 50 * 1000;
    
  public:
    Map(int rows, int cols);
    virtual ~Map();

    void Clear();
    void Refresh();
    void Draw(int row, int col, char c);
    Position GetRandomPos();
    void StartAutoRepaint();
    void StopAutoRepaint();
    void AddPaintObject(PaintObject* object);
    void AutoRepaintThreadFunc();
    void Paint();
    int GetRows();
    int GetCols();
    
  private:
    int rows_, cols_;
    bool autoRepaint_;
    pthread_t repaintThread_;
    Border* border_;
    std::vector<PaintObject *> objects_;
};

#endif /* _MAP_H_ */
