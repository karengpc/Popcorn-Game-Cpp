#pragma once

#include "Laser_Beam.h"

//------------------------------------------------------------------------------------------------------------
class AsLaser_Beam_Set: public AGame_Objects_Set
{
public:
	void Fire(double left_gun_x_pos, double right_gun_x_pos);
	void Disable_All();

private:
	bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);

	static const int Max_Laser_Beam_Count = 10;

	ALaser_Beam Laser_Beams[Max_Laser_Beam_Count];
};
//------------------------------------------------------------------------------------------------------------
