#include "Engine.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_image.h"
#include "Mouse.h"

Engine *Engine::eng = NULL;


Engine::Engine()
{
	endGameLoop = false;
	game = NULL;
	allegroInitialized = false;
	display = NULL;
	eventQueue = NULL;
	timer = NULL;
	font = NULL;
}

void Engine::Initialize(std::string gameName)
{
	if (!allegroInitialized)
	{
		if (!al_init())
		{
			ShowError("Blad podczas inicjalizacji allegro.");
			return;
		}

		if (!al_install_keyboard())
		{
			ShowError("Blad podczas inicjalizacji klawiatury.");
			return;
		}

		if(!al_install_mouse()) 
		{
			ShowError("Blad podczas inicjalizacji myszy.");
			return;
		}
		
		display = al_create_display(800, 600);
		if (!display) 
		{
			ShowError("Blad podczas inicjalizacji.");
			return;
		}

		al_init_font_addon();
		al_init_ttf_addon();

		font = al_load_ttf_font("tahoma.ttf", 20, 0);
		if (!font) 
		{
			ShowError("Blad podczas inicjalizacji czcionki.");
			return;
		}

		al_set_window_title(display, gameName.c_str());

		timer = al_create_timer(1.0 / FPS);
		if (!timer) 
		{
			ShowError("Blad podczas tworzenia timera.");
			return;
		}

		eventQueue = al_create_event_queue();
		if (!eventQueue) 
		{
			ShowError("Blad podczas inicjalizacji kolejki.");
			return;
		}
 
		al_register_event_source(eventQueue, al_get_display_event_source(display));
		al_register_event_source(eventQueue, al_get_timer_event_source(timer));
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
		al_register_event_source(eventQueue, al_get_mouse_event_source());

		al_init_image_addon();

		allegroInitialized = true;
	}
}

void Engine::AddEvents(IGame *game, IMouseEvents *mouseEvents)
{
	this->game = game;
	this->mouseEvents = mouseEvents;
}

void Engine::ShowError(std::string message)
{
	al_show_native_message_box(al_get_current_display(), 
									 "Socoban", 
									 "Error", 
									 message.c_str(),
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void Engine::DrawBitmap(Bitmap *bitmap, int x, int y)
{
	bitmap->Draw(x, y);
}

void Engine::DrawGameText(std::string text, int x, int y, int r, int g, int b, bool center)
{
	int flag = center ? ALLEGRO_ALIGN_CENTRE : ALLEGRO_ALIGN_LEFT;

	al_draw_text(this->font, al_map_rgb(r, g, b), x, y, flag, text.c_str());
}

int Engine::GetDisplayWidth()
{
	return al_get_display_width(display);
}

int Engine::GetDisplayHeight()
{
	return al_get_display_height(display);
}

Bitmap *Engine::GetBMP(std::string filePath)
{
	if (bitmapsPointiers.count(filePath))
	{
		return bitmapsPointiers[filePath];
	}
	else
	{
		Bitmap *bmp = new Bitmap("graphics/" + filePath);

		bitmapsPointiers[filePath] = bmp;

		return bmp;
	}
}

void Engine::StartGameLoop()
{
	Mouse mouse;

	bool redraw = false;
	al_start_timer(timer);
 
	while(!endGameLoop)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventQueue, &ev);
 
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			game->Update();

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) 
		{
			Key key = (Key)ev.keyboard.keycode;
			game->KeyDownEvent(key);
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) 
		{
			Key key = (Key)ev.keyboard.keycode;
			game->KeyUpEvent(key);
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
			  ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			mouse.x = ev.mouse.x;
			mouse.y = ev.mouse.y;
			mouse.button = ev.mouse.button;

			mouseEvents->MouseMove(mouse);
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			mouse.x = ev.mouse.x;
			mouse.y = ev.mouse.y;
			mouse.button = ev.mouse.button;

			mouseEvents->MouseButtonUp(mouse);
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			mouse.x = ev.mouse.x;
			mouse.y = ev.mouse.y;
			mouse.button = ev.mouse.button;

			mouseEvents->MouseButtonDown(mouse);
		}
 
		if(redraw && al_is_event_queue_empty(eventQueue))
		{
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));

			game->Draw();

			al_flip_display();
		}
	}
}

Engine *Engine::GetInstance()
{
	if (eng == NULL)
	{
		eng = new Engine();
	}

	return eng;
}

void Engine::DisposeBitmaps()
{
	for (std::map<std::string, Bitmap*>::const_iterator iterator = bitmapsPointiers.begin(); iterator != bitmapsPointiers.end(); ++iterator)
	{
		delete iterator->second;
	}

	bitmapsPointiers.clear();
}

void Engine::Exit()
{
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(eventQueue);
	al_destroy_font(font);

	DisposeBitmaps();

	delete eng;
}