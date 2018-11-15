#include "Simon.h"
#include "debug.h"
#include "Game.h"



void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy +=	 SIMON_GRAVITY * dt;
	
#pragma region Collision Event
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(colliable_objects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


	

#pragma region Collision logic with Goombas
		// Collision logic with Goombas
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];

		//	if (dynamic_cast<CGoomba *>(e->obj))
		//	{
		//		CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

		//		// jump on top >> kill Goomba and deflect a bit 
		//		if (e->ny < 0)
		//		{
		//			if (goomba->GetState() != GOOMBA_STATE_DIE)
		//			{
		//				goomba->SetState(GOOMBA_STATE_DIE);
		//				vy = -SIMON_JUMP_DEFLECT_SPEED;
		//			}
		//		}
		//		else if (e->nx != 0)
		//		{
		//			/*if (untouchable==0)
		//			{
		//				if (goomba->GetState()!=GOOMBA_STATE_DIE)
		//				{
		//					if (level > SIMON_LEVEL_SMALL)
		//					{
		//						level = SIMON_LEVEL_SMALL;
		//						StartUntouchable();
		//					}
		//					else
		//						SetState(SIMON_STATE_DIE);
		//				}
		//			}*/
		//		}
		//	}
		//}
#pragma endregion

	}
	 //clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion

}

void CSimon::Render()
{
	DebugOut(L"State %d ",vx);
	int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else 
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				if (isSittingDown == true) ani = SIMON_ANI_SIT_DOWN_RIGHT;
				else ani = SIMON_ANI_IDLE_RIGHT;
			}
			else if (nx < 0) 
			{
				if (isSittingDown == true) ani = SIMON_ANI_SIT_DOWN_LEFT;
				else ani = SIMON_ANI_IDLE_LEFT;
			}
		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}
		int alpha = 255;
		if (untouchable) alpha = 128;
		animations[ani]->Render(x, y, alpha);
		RenderBoundingBox();
}


void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		isSittingDown = false;
		if (y > 300)
			vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		isSittingDown = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_DOWN:
		isSittingDown = true;
		vx = 0; 
		vy = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;


	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
	
}





	