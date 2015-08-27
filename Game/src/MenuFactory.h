#ifndef MENUFACTORY_H
#define MENUFACTORY_H

#include "Frame.h"

class MenuFactory
{
public:
	static Frame * MainMenu(){
		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("menu_background");

		Hud * logo = new Hud(Vector2(1, 6), Vector2(28, 8), 100);
		logo->setTexture("game_logo");

		Hud * menu_bar = new Hud(Vector2(0, 0), Vector2(30, 100), 1);
		menu_bar->setTexture("menu_collect");

		Button * play = new Button(Vector2(5, 20), Vector2(23, 6), "PLAY");
		Button * options = new Button(Vector2(5, 28), Vector2(23, 6), "OPTIONS");
		Button * help = new Button(Vector2(5, 36), Vector2(23, 6), "HELP");
		Button * exit = new Button(Vector2(5, 44), Vector2(23, 6), "EXIT");

		Frame * frame = new Frame("main_menu");
		frame->addBackground(background);
		frame->addHud(menu_bar);
		frame->addHud(logo);
		frame->addContainer(play);
		frame->addContainer(options);
		frame->addContainer(help);
		frame->addContainer(exit);

		return frame;
	}

	static Frame * OptionsMenu(){
		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("menu_background");

		Hud * logo = new Hud(Vector2(1, 6), Vector2(28, 8), 100);
		logo->setTexture("game_logo");

		Hud * menu_bar = new Hud(Vector2(0, 0), Vector2(30, 100), 1);
		menu_bar->setTexture("menu_collect");

		Button * play = new Button(Vector2(5, 20), Vector2(23, 6), "PLAY");
		Button * options = new Button(Vector2(20, 28), Vector2(23, 6), "OPTIONS");
		Button * help = new Button(Vector2(5, 36), Vector2(23, 6), "HELP");
		Button * exit = new Button(Vector2(5, 44), Vector2(23, 6), "EXIT");

		Frame * frame = new Frame("main_menu");
		frame->addBackground(background);
		frame->addHud(menu_bar);
		frame->addHud(logo);
		frame->addContainer(play);
		frame->addContainer(options);
		frame->addContainer(help);
		frame->addContainer(exit);

		return frame;
	}

	static Frame * LoadingMenu(){
		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("menu_load");

		Label * label = new Label(Vector2(10, 90), Vector3(109, 93, 60), 15);
		label->setText("Tip: be careful with cooldowns.");

		Label * loader = new Label(Vector2(40, 4), Vector3(109, 93, 60), 20);
		loader->setText("Loading ...");

		ProgressBar * loading = new ProgressBar(Vector2(40, 7), Vector2(20, 4), Vector3(255,255,255));

		Frame * frame = new Frame("loading_menu");
		frame->addBackground(background);
		frame->addHud(loader);
		frame->addHud(label);
		frame->addContainer(loading);

		return frame;
	}

	static Frame * IntermissionMenu(){
		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("menu_load");
		background->color = Vector3(0.0, 0.0, 0.0);

		Label * label = new Label(Vector2(10, 75), Vector3(85, 134, 93), 17);
		label->addGlow(Vector3(0, 84, 15));
		label->setText("¿Down with uncle Sam¿");
		label->readyFade();

		Label * label2 = new Label(Vector2(10, 78), Vector3(85, 134, 93), 17);
		label2->addGlow(Vector3(0, 84, 15));
		label2->setText("Day 3: 26-12-1989");
		label2->readyFade();

		Label * label3= new Label(Vector2(10, 81), Vector3(85, 134, 93), 17);
		label3->addGlow(Vector3(0, 84, 15));
		label3->setText("Lieutant Ludovich Vloy a.k.a Mr.Piolet");
		label3->readyFade();

		Label * label4 = new Label(Vector2(10, 84), Vector3(85, 134, 93), 17);
		label4->addGlow(Vector3(0, 84, 15));
		label4->setText("031 Zone, Russia");
		label4->readyFade();
		//label->insertOverTime(100);

		Frame * frame = new Frame("intermission_menu");
		frame->addBackground(background);
		frame->addHud(label);
		frame->addHud(label2);
		frame->addHud(label3);
		frame->addHud(label4);

		return frame;
	}

	static Frame * PlayingMenu(){

		Frame * frame = new Frame("loading_menu");

		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("menu_load");
		frame->addBackground(background);

		Hud * minimap = new Hud(Vector2(1, 72), Vector2(20, 25), 0);
		minimap->setTexture("hud_bomb");
		frame->addHud(minimap);

		Hud * minimapLayer = new Hud(Vector2(1, 72), Vector2(20, 25), 0);
		minimapLayer->setTexture("hud_compass");
		frame->addHud(minimapLayer);


		Hud * pilot1 = new Hud(Vector2(75, 2), Vector2(7, 7), 0);
		pilot1->setTexture("hud_allies_first");
		frame->addHud(pilot1);

		Hud * pilot2 = new Hud(Vector2(81, 2), Vector2(7, 7), 0);
		pilot2->setTexture("hud_allies_second");
		frame->addHud(pilot2);

		Hud * pilot3 = new Hud(Vector2(87, 2), Vector2(7, 7), 0);
		pilot3->setTexture("hud_allies_third");
		frame->addHud(pilot3);

		Hud * pilot4 = new Hud(Vector2(93, 2), Vector2(7, 7), 0);
		pilot4->setTexture("hud_allies_fourth");
		frame->addHud(pilot4);

		Hud * bombs = new Hud(Vector2(90, 82), Vector2(4, 4), 0);
		bombs->setTexture("hud_bomb");
		frame->addHud(bombs);

		Hud * rockets = new Hud(Vector2(82, 82), Vector2(4, 4), 0);
		rockets->setTexture("hud_rocket");
		frame->addHud(rockets);

		/*
		Hud * ammoheat = new Hud(Vector2(82, 85), Vector2(16, 6), 0);
		ammoheat->setTexture("hud_ammobar");
		frame->addHud(ammoheat);
		*/
		Hud * ammoico = new Hud(Vector2(79, 87), Vector2(2.5, 2.5), 0);
		ammoico->setTexture("hud_ammoicon");
		frame->addHud(ammoico);

		Hud * healthback = new Hud(Vector2(82, 92), Vector2(16, 3), 0);
		healthback->setTexture("hud_healthback");
		frame->addHud(healthback);

		Hud * alliedScore = new Hud(Vector2(42, 2), Vector2(16, 3), 0);
		alliedScore->setTexture("hud_healthback");
		frame->addHud(alliedScore);

		Hud * enemyscore = new Hud(Vector2(42, 6), Vector2(16, 3), 0);
		enemyscore->setTexture("hud_healthback");
		frame->addHud(enemyscore);

		Hud * alliedScoreIcon = new Hud(Vector2(38.5, 2), Vector2(3, 3), 0);
		alliedScoreIcon->setTexture("hud_alliedscoreflag");
		frame->addHud(alliedScoreIcon);

		Hud * enemyScoreIcon = new Hud(Vector2(38.5, 6), Vector2(3, 3), 0);
		enemyScoreIcon->setTexture("hud_enemyscoreflag");
		frame->addHud(enemyScoreIcon);

		Hud * healthcross = new Hud(Vector2(79, 92.5), Vector2(2, 2), 0);
		healthcross->setTexture("hud_healthcross");
		frame->addHud(healthcross);

		return frame;
	}


	static Frame * GameOverMenu(){

		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("background_lose");

		Label * label = new Label(Vector2(20, 40), Vector3(85, 134, 93), 30);
		label->addGlow(Vector3(0, 84, 15));
		label->setText("Press [START] to return to the main menu.");
		label->setFade(1000);

		Frame * frame = new Frame("loading_menu");
		frame->addBackground(background);
		frame->addHud(label);

		return frame;
	}

	static Frame * GameWinMenu(){

		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("background_win");

		Label * label = new Label(Vector2(27, 90), Vector3(85, 134, 93), 30);
		label->addGlow(Vector3(0, 84, 15));
		label->setText("Press [START] to return to the main menu.");
		label->setFade(1000);

		Frame * frame = new Frame("loading_menu");
		frame->addBackground(background);
		frame->addHud(label);

		return frame;
	}

	static Frame * ControlsMenu()
	{
		Hud * background = new Hud(Vector2(0, 0), Vector2(100, 100), 0);
		background->setTexture("background_controls");

		Label * label = new Label(Vector2(27, 90), Vector3(85, 134, 93), 30);
		label->addGlow(Vector3(0, 84, 15));
		label->setText("Press [START] to return to the main menu.");
		label->setFade(1000);

		Frame * frame = new Frame("controls_menu");
		frame->addBackground(background);
		frame->addHud(label);

		return frame;
	}

};

#endif