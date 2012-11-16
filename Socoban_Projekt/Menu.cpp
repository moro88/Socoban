#include "Menu.h"
#include <sstream>

Menu::Menu()
{
	engine = Engine::GetInstance();
	bitmap = NULL;
	windowBitmap = NULL;
	actualMap = "";
	freeze = false;
	isInGame = false;
	playerSteps = 0;

	menuBitmap = engine->GetBMP("menu/menubitmap.bmp");
	highscoreBitmap = engine->GetBMP("menu/highscorebitmap.bmp");
	gameBitmap = engine->GetBMP("menu/game1.bmp");

	HighscoreCollection *h = new HighscoreCollection();
	//h->IsQualified(1, 10000, "99:99:98");
	//h->AddHighscore(1, "arek2", "99:99:98", 10000);
	//delete h;

	CreateMainMenu();
}

void Menu::CreateMainMenu()
{
	buttons.clear();

	buttons.push_back(Button("newgame", Point(276, 250), this));
	buttons.push_back(Button("choosenmap", Point(276, 290), this));
	buttons.push_back(Button("highscore", Point(276, 330), this));
	buttons.push_back(Button("editor", Point(276, 370), this));
	buttons.push_back(Button("end", Point(276, 450), this));

	bitmap = menuBitmap;
	windowBitmap = NULL;
	mapToLoad = "menu";

	isInGame = false;
}

void Menu::CreateMapsMenu()
{
	buttons.clear();

	buttons.push_back(Button("map/mapno1", Point(130, 200), this));
	buttons.push_back(Button("map/mapno2", Point(130, 250), this));
	buttons.push_back(Button("map/mapno3", Point(130, 300), this));
	buttons.push_back(Button("map/mapno4", Point(130, 350), this));
	buttons.push_back(Button("map/mapno5", Point(130, 400), this));
	buttons.push_back(Button("map/mapno6", Point(430, 200), this));
	buttons.push_back(Button("map/mapno7", Point(430, 250), this));
	buttons.push_back(Button("map/mapno8", Point(430, 300), this));
	buttons.push_back(Button("map/mapno9", Point(430, 350), this));
	buttons.push_back(Button("map/mapno10", Point(430, 400), this));

	buttons.push_back(Button("back", Point(500, 520), this));
	buttons.push_back(Button("usermap", Point(280, 450), this));

	bitmap = menuBitmap;
	windowBitmap = NULL;
}

void Menu::CreateGameMenu(std::string levelName)
{
	buttons.clear();

	buttons.push_back(Button("restart", Point(engine->GetDisplayWidth() - 120, 30), this));
	buttons.push_back(Button("close", Point(engine->GetDisplayWidth() - 120, 70), this));

	actualMap = levelName;
	mapToLoad = levelName;
	bitmap = gameBitmap;
	windowBitmap = NULL;

	isInGame = true;
	playerSteps = 0;
	startTime = time(NULL);
}

void Menu::CreateGameWindow(std::string windowName, std::string firstBtnName, std::string secondBtnName)
{
	this->windowBitmap = engine->GetBMP("menu/windows/" + windowName + ".bmp");

	Point bitmapLocation((engine->GetDisplayWidth() / 2) - (windowBitmap->GetWidth() / 2),
			(engine->GetDisplayHeight() / 2) - (windowBitmap->GetHeight() / 2));

	if (firstBtnName.length() > 0)
	{
		buttons.push_back(Button("windows/" + firstBtnName, Point(bitmapLocation.GetX() + 20, 
			bitmapLocation.GetY() + this->windowBitmap->GetHeight() - 30), this));
	}
	if (secondBtnName.length() > 0)
	{
		buttons.push_back(Button("windows/" + secondBtnName, Point(bitmapLocation.GetX() + this->windowBitmap->GetWidth() - 80, 
			bitmapLocation.GetY() + this->windowBitmap->GetHeight() - 30), this));
	}
}

void Menu::ButtonClicked(std::string name)
{
	if (name == "choosenmap")
	{
		CreateMapsMenu();
	}
	else if (name == "back")
	{
		CreateMainMenu();
	}
	else if (name.substr(0, 9) == "map/mapno")
	{
		CreateGameMenu("lvl" + name.substr(9, 10));
	}
	else if (name == "newgame")
	{
		CreateGameMenu("lvl1");
	}
	else if (name == "restart")
	{
		CreateGameMenu(actualMap);
	}
	else if (name == "close" || name == "windows/endsmall")
	{
		freeze = false;
		CreateMainMenu();	
	}
	else if (name == "windows/next")
	{
		std::string lvl = "lvl";
		int lvlNo = atoi(actualMap.substr(actualMap.size() - 1, actualMap.size()).c_str());
		std::stringstream out;
		out << lvlNo + 1;
		lvl += out.str();

		freeze = false;
		CreateGameMenu(lvl);	
	}
	else if (name == "end")
	{
		engine->endGameLoop = true;
	}
}

void Menu::NextMap()
{
	this->freeze = true;

	if (actualMap == "lvl10")
	{

	}
	else
	{
		CreateGameWindow("congratulations", "next", "endsmall");
	}
}

std::string Menu::GetMap()
{
	std::string map = mapToLoad;
	mapToLoad = "";
	return map;
}

bool Menu::IsFreezed()
{
	return this->freeze;
}

void Menu::Draw()
{
	if (bitmap != NULL)
	{
		engine->DrawBitmap(bitmap, engine->GetDisplayWidth() - bitmap->GetWidth(), 
			engine->GetDisplayHeight() - bitmap->GetHeight());
	}

	if (windowBitmap != NULL)
	{
		windowBitmap->Draw((engine->GetDisplayWidth() / 2) - (windowBitmap->GetWidth() / 2),
			(engine->GetDisplayHeight() / 2) - (windowBitmap->GetHeight() / 2));
	}
	
	if (isInGame)
	{
		DrawGameText();
	}

	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].Draw();
	}
}

void Menu::DrawGameText()
{
	int r = 255;
	int g = 160;
	int b = 0;

	engine->DrawGameText("Kroki:", engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
			engine->GetDisplayHeight() - 200, r, g, b, true);

	std::ostringstream ss;
	ss << playerSteps;

	engine->DrawGameText(ss.str(), engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 175, r, g, b, true);

	engine->DrawGameText("Czas:", engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 125, r, g, b, true);

	engine->DrawGameText(playingTime, engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 100, r, g, b, true);
}

void Menu::Update(int playerSteps)
{
	if (isInGame)
	{
		/*time_t endTime = time(NULL);
		time_t diff = endTime - startTime;
		
		char buffer[80];
		strftime(buffer, 80,"%X", gmtime(&diff));
		playingTime = buffer;

		this->playerSteps = playerSteps;*/
	}
}

void Menu::MouseMove(Mouse mouse)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].MouseMove(mouse);
	}
}

void Menu::MouseButtonDown(Mouse mouse)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].MouseDown(mouse);
	}
}

void Menu::MouseButtonUp(Mouse mouse)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].MouseUp(mouse);
	}
}

void Menu::CharEntered(char c)
{

}