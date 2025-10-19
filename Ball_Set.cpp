#include "Ball_Set.h"

// AsBall_Set
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Act()
{
	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::On_Platform)
			if (curr_ball->Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= curr_ball->Release_Timer_Tick)
				curr_ball->Release();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Release_From_Platform(double platform_x_pos)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		if (Balls[i].Get_State() == EBall_State::On_Platform)
			Balls[i].Set_State(EBall_State::Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Release_Next_Ball()
{
	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::On_Platform)
		{
			curr_ball->Release();
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_On_Platform(double platform_x_pos)
{
	int i;

	for (i = 0; i < 1; i++)
	{
		Balls[i].Set_State(EBall_State::Normal);
		Balls[i].Set_State(EBall_State::On_Platform, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
		Balls[i].Release_Timer_Tick = 0;
	}

	for (; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBall_State::Disabled);
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::All_Balls_Are_Lost()
{
	int i;
	int active_balls_count = 0;
	int lost_balls_count = 0;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		if (Balls[i].Get_State() == EBall_State::Disabled)
			continue;

		++active_balls_count;

		if (Balls[i].Get_State() == EBall_State::Lost)
		{
			++lost_balls_count;
			continue;
		}
	}

	if (active_balls_count == lost_balls_count)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_For_Test()
{
	Balls[0].Set_For_Test();  // ¬ повтор€ющихс€ тестах участвует только 0-й м€чик
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Is_Test_Finished()
{
	return Balls[0].Is_Test_Finished();  // ¬ повтор€ющихс€ тестах участвует только 0-й м€чик
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Triple_Balls()
{// "–астроить" самый дальний лет€щий от платформы м€чик

	int i;
	ABall *curr_ball;
	ABall *further_ball = 0;
	ABall *left_candidate = 0, *right_candidate = 0;
	double curr_ball_x, curr_ball_y;
	double further_ball_x, further_ball_y;

	// 1. ¬ыбираем самый дальний по Y м€чик
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() != EBall_State::Normal)
			continue;

		if (further_ball == 0)
			further_ball = curr_ball;
		else
		{
			curr_ball->Get_Center(curr_ball_x, curr_ball_y);
			further_ball->Get_Center(further_ball_x, further_ball_y);

			if (curr_ball_y < further_ball_y)
				further_ball = curr_ball;
		}
	}

	// 2. ≈сли есть "нормальный" м€чик, то размножаем его
	if (further_ball == 0)
		return;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		switch (curr_ball->Get_State() )
		{
		case EBall_State::Disabled:
		case EBall_State::Lost:
			if (left_candidate == 0)
				left_candidate = curr_ball;
			else
				if (right_candidate == 0)
				{
					right_candidate = curr_ball;
					break;  // ќба кандидата найдены
				}
		}
	}

	// 3. –азводим боковые м€чики в стороны
	if (left_candidate != 0)
	{
		*left_candidate = *further_ball;
		left_candidate->Set_Direction(left_candidate->Get_Direction() + AsConfig::Min_Ball_Angle);
	}

	if (right_candidate != 0)
	{
		*right_candidate = *further_ball;
		right_candidate->Set_Direction(right_candidate->Get_Direction() - AsConfig::Min_Ball_Angle);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Inverse_Balls()
{// ћен€ем направление на обратное у всех м€чиков

	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::Normal)
			curr_ball->Set_Direction(curr_ball->Get_Direction() + M_PI);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Accelerate()
{
	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::Normal)
			curr_ball->Set_Speed(curr_ball->Get_Speed() * AsConfig::Ball_Acceleration);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Reset_Speed()
{
	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::Normal)
			curr_ball->Set_Speed(AsConfig::Normal_Ball_Speed);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::On_Platform_Advance(double direction, double platform_speed, double max_speed)
{
	int i;
	ABall *curr_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		curr_ball = &Balls[i];

		if (curr_ball->Get_State() == EBall_State::On_Platform)
			curr_ball->Forced_Advance(direction, platform_speed, max_speed);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Get_Next_Game_Object(int &index, AGame_Object **game_obj)
{
	if (index < 0 || index >= AsConfig::Max_Balls_Count)
		return false;

	*game_obj = &Balls[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------
