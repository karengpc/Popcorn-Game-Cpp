#pragma once

#include "Explosive_Ball.h"
#include "Gate.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed,
	Opening,
	Staring,
	Closing
};
//------------------------------------------------------------------------------------------------------------
enum class EMonster_State: unsigned char
{
	Missing,
	Emitting,
	Alive,
	Destroing
};
//------------------------------------------------------------------------------------------------------------
class AMonster: public AHit_Checker, public AGame_Object
{
public:
	AMonster();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);
	virtual bool Check_Hit(RECT& rect);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Activate(int x_pos, int y_pos, bool moving_right);
	bool Is_Active();
	void Destroy();

	static const int Width = 16;
	static const int Height = 16;

private:
	void Draw_Alive(HDC hdc);
	void Draw_Destroing(HDC hdc, RECT &paint_area);
	void Act_Alive();
	void Act_Destroing();
	void Get_Monster_Rect(double x_pos, double y_pos, RECT &rect);
	void Redraw_Monster();

	EEye_State Eye_State;
	EMonster_State Monster_State;
	double X_Pos, Y_Pos;
	double Speed, Direction;
	double Cornea_Height;
	int Start_Blink_Timeout, Total_Animation_Timeout;
	int Next_Direction_Switch_Tick, Alive_Timer_Tick;
	RECT Monster_Rect, Prev_Monster_Rect;

	static const int Blink_Stages_Count = 7;
	static const int Explosive_Balls_Count = 20;

	int Blink_Ticks[Blink_Stages_Count];
	AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];

	static const double Max_Cornea_Height;
	static const double Blink_Timeouts[Blink_Stages_Count];
	static const EEye_State Blink_States[Blink_Stages_Count];
};
//------------------------------------------------------------------------------------------------------------
