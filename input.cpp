
#include "main.h"
#include "input.h"


static Mouse mouse;
static Keyboard kbd;
void CInput::Init()
{

	mouse.SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	mouse.SetWindow(GetWindow());
}

void CInput::Uninit()
{

}

void CInput::Update()
{
	kbd.kbTracker.Update(kbd.GetState());
	mouse.mouseTracker.Update(mouse.GetState());
}

bool CInput::GetKeyPress(BYTE KeyCode)
{
	return kbd.GetState().IsKeyDown((Keyboard::Keys)KeyCode);
}

bool CInput::GetKeyTrigger(BYTE KeyCode)
{
	return kbd.kbTracker.IsKeyPressed((Keyboard::Keys)KeyCode);
}

