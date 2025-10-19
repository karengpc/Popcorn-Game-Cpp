#include "Border.h"

// AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::~AsBorder()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		delete Gates[i];
}
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
: Floor_Rect{}, Gates{}
{
	Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.top = AsConfig::Floor_Y_Pos * AsConfig::Global_Scale;
	Floor_Rect.right = (AsConfig::Max_X_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;

	// Гейты
	Gates[0] = new AGate(1, 29, 0, 3);
	Gates[1] = new AGate(AsConfig::Max_X_Pos, 29, AsConfig::Level_Width - 1, 3);

	Gates[2] = new AGate(1, 77, 0, 9);
	Gates[3] = new AGate(AsConfig::Max_X_Pos, 77, AsConfig::Level_Width - 1, 9);

	Gates[4] = new AGate(1, 129);
	Gates[5] = new AGate(AsConfig::Max_X_Pos, 129);

	Gates[6] = new AGate(1, 178);
	Gates[7] = new AGate(AsConfig::Max_X_Pos, 178);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsTools::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Open_Gate(int gate_index, bool short_open)
{
	if (gate_index != AsConfig::Gates_Count - 1 && short_open)
		AsConfig::Throw();

	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		Gates[gate_index]->Open_Gate(short_open);
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
int AsBorder::Long_Open_Gate()
{
	int i;
	int gate_index;
	bool got_gate = false;
	AGate *gate;

	gate_index = AsTools::Rand(AsConfig::Gates_Count);

	
	for (i = 0; i < AsConfig::Gates_Count; i++)
	{
		gate = Gates[gate_index];

		if (gate_index != AsConfig::Gates_Count - 1)
			if (gate->Is_Closed() )
			{
				if (gate->Level_X_Pos == -1)
				{
					got_gate = true;
					break;
				}

				if (! AsLevel::Has_Brick_At(gate->Level_X_Pos, gate->Level_Y_Pos)
					&& ! AsLevel::Has_Brick_At(gate->Level_X_Pos, gate->Level_Y_Pos + 1) )
				{
					got_gate = true;
					break;
				}
			}

		++gate_index;

		if (gate_index >= AsConfig::Gates_Count)
			gate_index = 0;
	}

	if (! got_gate)
		AsConfig::Throw();

	Open_Gate(gate_index, false);

	return gate_index;
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Opened(int gate_index)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		return Gates[gate_index]->Is_Opened();
	else
	{
		AsConfig::Throw();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Closed(int gate_index)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		return Gates[gate_index]->Is_Closed();
	else
	{
		AsConfig::Throw();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Get_Gate_Pos(int gate_index, int &gate_x_pos, int &gate_y_pos)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		Gates[gate_index]->Get_Pos(gate_x_pos, gate_y_pos);
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball)
{// Корректируем позицию при отражении от рамки

	bool got_hit = false;
	
	// 1. Левый край
	if (next_x_pos - AsConfig::Ball_Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	// 2. Верхний край
	if (next_y_pos - AsConfig::Ball_Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect(true);
	}

	// 3. Правый край
	if (next_x_pos + AsConfig::Ball_Radius > AsConfig::Max_X_Pos + 1)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	// 4. Нижний край
	if (AsConfig::Level_Has_Floor && next_y_pos + AsConfig::Ball_Radius > AsConfig::Floor_Y_Pos)
	{
		got_hit = true;
		ball->Reflect(true);
	}

	// Чтобы шарик смог улететь ниже пола, проверяем его max_y_pos ниже видимой границы
	if (next_y_pos + AsConfig::Ball_Radius > (double)AsConfig::Max_Y_Pos + AsConfig::Ball_Radius * 4.0)  
		ball->Set_State(EBall_State::Lost);

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Begin_Movement()
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Finish_Movement()
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Advance(double max_speed)
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
double AsBorder::Get_Speed()
{
	return 0.0;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear(HDC hdc, RECT &paint_area)
{
	int i;
	RECT intersection_rect;

	// 1. Стираем гейты
	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Clear(hdc, paint_area);

	// 2. Стираем пол (если надо)
	if (! AsConfig::Level_Has_Floor)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	AsTools::Rect(hdc, Floor_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{// Рисует рамку уровня

	int i;

	// 1. Линия слева
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 2, 1 + i * 4, false);

	// 2. Линия справа
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, AsConfig::Max_X_Pos + 1, 1 + i * 4, false);

	// 3. Линия сверху
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);

	// 4. Пол (если есть)
	if (AsConfig::Level_Has_Floor)
		Draw_Floor(hdc, paint_area);

	// 5. Гейты
	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false;  // Заглушка, т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool top_border)
{// Рисует элемент рамки уровня

	int i;
	int gate_top_y, gate_low_y;
	RECT intersection_rect, rect;

	rect.left = x * AsConfig::Global_Scale;
	rect.top = y * AsConfig::Global_Scale;
	rect.right = (x + 4) * AsConfig::Global_Scale;
	rect.bottom = (y + 4) * AsConfig::Global_Scale;

	if (! top_border)
	{
		for (i = 0; i < AsConfig::Gates_Count; i++)
		{
			Gates[i]->Get_Y_Size(gate_top_y, gate_low_y);

			if (rect.top >= gate_top_y && rect.bottom <= gate_low_y)
				return;  // Гейт целиком перекроет элемент рамки
		}
	}

	if (! IntersectRect(&intersection_rect, &paint_area, &rect) )
		return;

	// Основная линия
	if (top_border)
		AsTools::Rect(hdc, x, y + 1, 4, 3, AsConfig::Blue_Color);
	else
		AsTools::Rect(hdc, x + 1, y, 3, 4, AsConfig::Blue_Color);

	// Белая кайма
	if (top_border)
		AsTools::Rect(hdc, x, y, 4, 1, AsConfig::White_Color);
	else
		AsTools::Rect(hdc, x, y, 1, 4, AsConfig::White_Color);

	// Перфорация
	if (top_border)
		AsTools::Rect(hdc, x + 2, y + 2, 1, 1, AsConfig::BG_Color);
	else
		AsTools::Rect(hdc, x + 2, y + 1, 1, 1, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area)
{
	int i, strokes_count;
	int x_pos, y_pos;
	int line_len = 4 * AsConfig::Global_Scale;
	int gap_len = 2 * AsConfig::Global_Scale;
	int stroke_len = line_len + gap_len;
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	strokes_count = (Floor_Rect.right - Floor_Rect.left + AsConfig::Global_Scale) / stroke_len;
	x_pos = Floor_Rect.left + 1;
	y_pos = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;

	AsConfig::Letter_Color.Select(hdc);

	for (i = 0; i < strokes_count; i++)
	{
		MoveToEx(hdc, x_pos, y_pos, 0);
		LineTo(hdc, x_pos + line_len, y_pos);

		x_pos += stroke_len;
	}
}
//------------------------------------------------------------------------------------------------------------
