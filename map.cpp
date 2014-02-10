#include <ncurses.h>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include "map.h"

using namespace std;

Border::Border(Map* map)
{
    rows_ = map->GetRows();
    cols_ = map->GetCols();
    map->AddPaintObject(this);
}

void Border::DrawSelf(Map* map)
{
    for (int i = 0; i <= cols_; i++) {
        map->Draw(0, i, kUpBorderChar);
    }

    for (int i = 0; i <= cols_; i++) {
        map->Draw(rows_, i, kDownBorderChar);
    }

    for (int i = 0; i <= rows_; i++) {
        map->Draw(i, 0, kLeftBorderChar);
    }

    for (int i = 0; i <= rows_; i++) {
        map->Draw(i, cols_, kRightBorderChar);
    }
}

Map::Map(int rows, int cols)
    : rows_(rows), cols_(cols)
    , autoRepaint_(false)
{
    initscr();
    curs_set(0);
    noecho();
    border_ = new Border(this);
    srand(time(NULL));
}

Map::~Map()
{
    if (autoRepaint_) {
        StopAutoRepaint();
    }
    delete border_;
    endwin();
}

void Map::Draw(int row, int col, char c)
{
    mvaddch(row, col, c);
}

void Map::Clear()
{
    clear();
}

void Map::Refresh()
{
    refresh();
}

void Map::Paint()
{
    for (typeof(objects_.begin()) it = objects_.begin();
         it != objects_.end();
         ++it) {
        (*it)->DrawSelf(this);
    }
}

void Map::AutoRepaintThreadFunc()
{
    while (autoRepaint_) {
        usleep(kMapRepaintInterval);
        Clear();
        Paint();
        Refresh();
    }
}

static void* AutoRepaintThreadFuncWrapper(void* arg)
{
    Map* map = (Map*)arg;
    map->AutoRepaintThreadFunc();

    return NULL;
}

void Map::StartAutoRepaint()
{
    autoRepaint_ = true;
    pthread_create(&repaintThread_, NULL, AutoRepaintThreadFuncWrapper, this);
}

void Map::StopAutoRepaint()
{
    autoRepaint_ = false;
    pthread_join(repaintThread_, NULL);
}

void Map::AddPaintObject(PaintObject* object)
{
    objects_.push_back(object);
}

int Map::GetRows()
{
    return rows_;
}

int Map::GetCols()
{
    return cols_;
}

Position Map::GetRandomPos()
{
    Position pos;
    pos.x = rand() % (rows_ - 1) + 1;
    pos.y = rand() % (cols_ - 1) + 1;

    return pos;
}
