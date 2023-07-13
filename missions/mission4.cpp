#include "./missions.h"


void refreshDataBase(System& airports)
{
    airports.regenerate_db();
}