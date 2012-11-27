#include "Menu.h"
#include "Convert.h"

Menu::Menu()
{
	engine = Engine::GetInstance();
	bitmap = NULL;
	windowBitmap = NULL;
	actualMap = "";
	freeze = false;
	isInGame = false;
	playerSteps = 0;
	this->showCaret = false;
	this->IsInEditor = false;

	menuBitmap = engine->GetBMP("menu/menubitmap.bmp");
	highscoreBitmap = engine->GetBMP("menu/highscorebitmap.bmp");
	gameBitmap = engine->GetBMP("menu/game1.bmp");

	highscore = new HighscoreCollection();
	highscoreDrawer = NULL;

	CreateMainMenu();
}

void Menu::CreateMainMenu()
{
	ClearButtons();

	buttons.push_back(new Button("newgame", Point(276, 250), this, true));
	buttons.push_back(new Button("choosenmap", Point(276, 290), this, true));
	buttons.push_back(new Button("highscore", Point(276, 330), this, true));
	buttons.push_back(new Button("editor", Point(276, 370), this, true));
	buttons.push_back(new Button("end", Point(276, 450), this, true));

	bitmap = menuBitmap;
	windowBitmap = NULL;
	mapToLoad = "menu";

	isInGame = false;
}

void Menu::CreateHighscoreMenu()
{
	ClearButtons();

	buttons.push_back(new Button("nexthigh", Point(345, 110), this, true));
	buttons.push_back(new Button("prevhigh", Point(10, 110), this, true));

	buttons.push_back(new Button("back", Point(500, 520), this, true));

	highscoreDrawer = new HighscoreDrawer(Point(10, 150), 255, 255, 255, 2);

	bitmap = highscoreBitmap;
}

void Menu::CreateMapsMenu()
{
	ClearButtons();

	buttons.push_back(new Button("map/mapno1", Point(130, 200), this, true));
	buttons.push_back(new Button("map/mapno2", Point(130, 250), this, true));
	buttons.push_back(new Button("map/mapno3", Point(130, 300), this, true));
	buttons.push_back(new Button("map/mapno4", Point(130, 350), this, true));
	buttons.push_back(new Button("map/mapno5", Point(130, 400), this, true));
	buttons.push_back(new Button("map/mapno6", Point(430, 200), this, true));
	buttons.push_back(new Button("map/mapno7", Point(430, 250), this, true));
	buttons.push_back(new Button("map/mapno8", Point(430, 300), this, true));
	buttons.push_back(new Button("map/mapno9", Point(430, 350), this, true));
	buttons.push_back(new Button("map/mapno10", Point(430, 400), this, true));

	buttons.push_back(new Button("back", Point(500, 520), this, true));
	buttons.push_back(new Button("usermap", Point(280, 450), this, true));

	bitmap = menuBitmap;
	windowBitmap = NULL;
}

void Menu::CreateGameMenu(std::string levelName)
{
	ClearButtons();

	buttons.push_back(new Button("restart", Point(engine->GetDisplayWidth() - 120, 30), this, true));
	buttons.push_back(new Button("close", Point(engine->GetDisplayWidth() - 120, 70), this, true));

	actualMap = levelName;
	mapToLoad = levelName;
	bitmap = gameBitmap;
	windowBitmap = NULL;

	isInGame = true;
	playerSteps = 0;
	startTime = time(NULL);
}

void Menu::ClearButtons()
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		delete buttons[i];
	}

	buttons.clear();
}

void Menu::CreateGameWindow(std::string windowName, std::string firstBtnName, std::string secondBtnName)
{
	this->windowBitmap = engine->GetBMP("menu/windows/" + windowName + ".bmp");

	Point bitmapLocation((engine->GetDisplayWidth() / 2) - (windowBitmap->GetWidth() / 2),
			(engine->GetDisplayHeight() / 2) - (windowBitmap->GetHeight() / 2));

	if (firstBtnName.length() > 0)
	{
		buttons.push_back(new Button("windows/" + firstBtnName, Point(bitmapLocation.GetX() + 20, 
			bitmapLocation.GetY() + this->windowBitmap->GetHeight() - 30), this, true));
	}
	if (secondBtnName.length() > 0)
	{
		buttons.push_back(new Button("windows/" + secondBtnName, Point(bitmapLocation.GetX() + this->windowBitmap->GetWidth() - 80, 
			bitmapLocation.GetY() + this->windowBitmap->GetHeight() - 30), this, true));
	}
}

void Menu::ButtonClicked(std::string name)
{
	if (highscoreDrawer != NULL
		&& name != "nexthigh" && name != "prevhigh")
	{
		delete highscoreDrawer;
		highscoreDrawer = NULL;
	}

	if (name == "choosenmap")
	{
		CreateMapsMenu();
	}
	else if (name == "back")
	{
		CreateMainMenu();
	}
	else if (name == "highscore")
	{
		CreateHighscoreMenu();
	}
	else if (name == "nexthigh")
	{
		if (highscoreDrawer != NULL)
			highscoreDrawer->NextHighscore();
	}
	else if (name == "prevhigh")
	{
		if (highscoreDrawer != NULL)
			highscoreDrawer->PrevHighscore();
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
		int lvlNo = Convert::ToInt(actualMap.substr(actualMap.size() - 1, actualMap.size()).c_str());

		if (showCaret) //je�li zako�czono dodawanie nazwy u�ytkownika
		{
			showCaret = false;
			highscore->AddHighscore(lvlNo, playerName, playingTime, playerSteps);

			playerName = playingTime = "";
			playerSteps = 0;
		}

		freeze = false;
		CreateGameMenu("lvl" + Convert::ToString(lvlNo + 1));	
	}
	else if (name == "end")
	{
		engine->endGameLoop = true;
	}
	else if (name == "editor")
	{
		this->IsInEditor = true;
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
		std::string lvl = "lvl";
		int lvlNo = atoi(actualMap.substr(actualMap.size() - 1, actualMap.size()).c_str());

		if (!showCaret && highscore->IsQualified(lvlNo, playerSteps, playingTime))
		{
			showCaret = true;
			CreateGameWindow("highscore", "next", "");
		}
		else
		{
			CreateGameWindow("congratulations", "next", "endsmall");
		}
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

	if (showCaret)
	{
		engine->DrawGameText(playerName, 320, 288, 50, 50, 50, false, true);
		engine->DrawLine(320 + (playerName.length() * 9), 290, 
			320 + (playerName.length() * 9), 305, 0, 0, 0, 2);
	}

	if (highscoreDrawer != NULL)
	{
		highscoreDrawer->Draw(highscore);
	}
	
	if (isInGame)
	{
		DrawGameText();
	}

	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
	}
}

void Menu::DrawGameText()
{
	int r = 255;
	int g = 160;
	int b = 0;

	engine->DrawGameText("Kroki:", engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
			engine->GetDisplayHeight() - 200, r, g, b, true, false);

	engine->DrawGameText(Convert::ToString(playerSteps), engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 175, r, g, b, true, false);

	engine->DrawGameText("Czas:", engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 125, r, g, b, true, false);

	engine->DrawGameText(playingTime, engine->GetDisplayWidth() - (bitmap->GetWidth() / 2),
		engine->GetDisplayHeight() - 100, r, g, b, true, false);
}

void Menu::Update(int playerSteps)
{
	if (isInGame && !this->showCaret)
	{
		time_t endTime = time(NULL);
		time_t diff = endTime - startTime;
		
		char buffer[80];
		strftime(buffer, 80,"%X", gmtime(&diff));
		playingTime = buffer;

		this->playerSteps = playerSteps;
	}
}

void Menu::CharEntered(char c)
{
	if (showCaret)
	{
		if (playerName.length() < 15)
		{
			if (c >= 32 && c <= 126)
			{
				playerName += c;
			}
		}
		
		if (c == 8 && playerName.length() > 0) //backspace
		{
			playerName = playerName.substr(0, playerName.length() - 1);
		}
	}
}

Menu::~Menu()
{
	if (this->highscoreDrawer != NULL)
	{
		delete this->highscoreDrawer;
	}
}