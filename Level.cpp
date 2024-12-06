#include "Level.h"
#include <string>
#include "Utils.h"
#include "monsters/Monster.h"
#include "ball/Ball.h"
#include "block/block.h"
#include "data/DataCenter.h"
#include <allegro5/allegro_primitives.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "shapes/Circle.h"
#include <array>
#include <iostream>
using namespace std;

// fixed settings
namespace LevelSetting {
	constexpr char level_path_format[] = "./assets/level/LEVEL%d.txt";
	//! @brief Grid size for each level.
	constexpr array<int, 4> grid_size = {
		40, 40, 40, 40
	};
	constexpr int monster_spawn_rate = 90;
	constexpr int ball_spawn_rate = 10;
};

void
Level::init() {
	level = -1;
	grid_w = -1;
	grid_h = -1;
	monster_spawn_counter = -1;
}

/**
 * @brief Loads level data from input file. The input file is required to follow the format.
 * @param lvl level index. The path format is a fixed setting in code.
 * @details The content of the input file should be formatted as follows:
 *          * Total number of monsters.
 *          * Number of each different number of monsters. The order and number follows the definition of MonsterType.
 *          * Indefinite number of Point (x, y), represented in grid format.
 * @see level_path_format
 * @see MonsterType
 */
void
Level::load_level(int lvl) {
	DataCenter *DC = DataCenter::get_instance();

	int map_data[17][20];
	now_level=lvl;
	total_blocks=0;

	char file_path[50];
	sprintf(file_path, "./assets/map/map_%d.txt", now_level%4);
	FILE *f = fopen(file_path, "r");
	
	for(int i=0;i<17;i++)
	{
		for(int j=0;j<20;j++)
		{
			fscanf(f,"%d", &map_data[i][j]);
			if(map_data[i][j]==1)
			{
				DC->blocks.emplace_back(Block::create_block(i,j,lvl));
				total_blocks++;
			}
		}
	}
	//std::cout<<total_blocks<<std::endl;
	// read road path
	debug_log("<Level> load level %d.\n", lvl);
}

/**
 * @brief Updates monster_spawn_counter and create monster if needed.
*/
void
Level::update() 
{
	//cout<<"total_blocks: "<<total_blocks<<endl;
	if (total_blocks == 0) {
		//cout<<"trigger"<<endl;
		now_level++;
		load_level(now_level);
	}
}



Rectangle
Level::grid_to_region(const Point &grid) const {
	int x1 = grid.x * LevelSetting::grid_size[level];
	int y1 = grid.y * LevelSetting::grid_size[level];
	int x2 = x1 + LevelSetting::grid_size[level];
	int y2 = y1 + LevelSetting::grid_size[level];
	return Rectangle{x1, y1, x2, y2};
}

Circle
Level::grid_to_region(const Point &grid, const int &r) const {
	int x = grid.x * LevelSetting::grid_size[level] + LevelSetting::grid_size[level] / 2;
	int y = grid.y * LevelSetting::grid_size[level] + LevelSetting::grid_size[level] / 2;
	return Circle{x, y, r};
}
