#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include <optional>

bool DEBUGGING_LEVELS_AND_BUILDING_THEM=false;
bool CHEAT_MODE=false;

struct Input{
	bool M;
	bool Escape;
	bool Mouse1;
	bool Mouse2;
	bool F9;
	bool F1;
	bool F2;
	bool R;
	bool LShift;
	bool left;
	bool right;
	bool down;
	bool up;
	bool PageUp;
	bool F;

	void read(const std::optional<sf::Event>& event){
		if (const auto* key=event->getIf<sf::Event::KeyPressed>()){
			if (key->code == sf::Keyboard::Key::M){M=true;}
			if (key->code == sf::Keyboard::Key::Escape){Escape=true;}
			if (key->code == sf::Keyboard::Key::F9){F9=true;}
			if (key->code == sf::Keyboard::Key::F1){F1=true;}
			if (key->code == sf::Keyboard::Key::F2){F2=true;}
			if (key->code == sf::Keyboard::Key::F){F=true;}
			if (key->code == sf::Keyboard::Key::R){R=true;}
			if (key->code == sf::Keyboard::Key::PageUp){PageUp=true;}
			if (key->code == sf::Keyboard::Key::LShift){LShift=true;}
			if (key->code == sf::Keyboard::Key::Left){left=true;}
			if (key->code == sf::Keyboard::Key::Right){right=true;}
			if (key->code == sf::Keyboard::Key::Down){down=true;}
			if (key->code == sf::Keyboard::Key::Up){up=true;}
		}
		if (const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
			if (mouse->button == sf::Mouse::Button::Left){Mouse1=true;}
			if (mouse->button == sf::Mouse::Button::Right){Mouse2=true;}
		}
		
	}

	void reset(){
		M=false;
		Escape=false;
		LShift=false;
		F9=false;
		Mouse1=false;
		Mouse2=false;
		left=false;
	 	right=false;
		down=false;
		up=false;
	}

};

struct Assets{
	sf::Font font;
	sf::Texture player_texture;
	sf::Texture wall_texture;
	sf::Texture finish_texture;
	sf::Texture checkpoint_texture1;
	sf::Texture Win0_texture;
	sf::Texture Win1_texture;
	sf::Texture Win2_texture;
	sf::Texture Win3_texture;
	sf::Texture sky_texture;
	sf::Texture Menu0_texture;
	sf::Texture Menu1_texture;
	sf::Texture Menu2_texture;
	sf::Texture Menu3_texture;
	sf::Texture Menu4_texture;
	void LoadAllTextures(){
		//if (!font.openFromFile("../../assets/fonts/arial.ttf")){}
		if (!player_texture.loadFromFile("../../assets/textures/PlayerTexture.png")){}
		if (!wall_texture.loadFromFile("../../assets/textures/WallTexture.png")){}
		if (!finish_texture.loadFromFile("../../assets/textures/FinishTexture.png")){}
		if (!checkpoint_texture1.loadFromFile("../../assets/textures/CheckpointTexture.png")){}
		if (!Win0_texture.loadFromFile("../../assets/textures/Win0.png")){}
		if (!Win1_texture.loadFromFile("../../assets/textures/Win1.png")){}
		if (!Win2_texture.loadFromFile("../../assets/textures/Win2.png")){}
		if (!Win3_texture.loadFromFile("../../assets/textures/Win3.png")){}
		if (!sky_texture.loadFromFile("../../assets/textures/SkyTexture.png")){}
		if (!Menu0_texture.loadFromFile("../../assets/textures/Menu0.png")){}
		if (!Menu1_texture.loadFromFile("../../assets/textures/Menu1.png")){}
		if (!Menu2_texture.loadFromFile("../../assets/textures/Menu2.png")){}
		if (!Menu3_texture.loadFromFile("../../assets/textures/Menu3.png")){}
		if (!Menu4_texture.loadFromFile("../../assets/textures/Menu4.png")){}
	}
};
Assets global_assets;

struct Finish;
struct Wall;
struct Checkpoint;
struct Player;
struct Camera;
struct Sky;
struct WinScreen;
struct TheWholeLevel;
struct Menu;

struct Menu{
	sf::Sprite sprite{global_assets.Menu0_texture};
	int state=0;
	float scale=6;

	void checkmouse(TheWholeLevel& the_whole_level);
	void draw(TheWholeLevel& the_whole_level);
};

struct Finish{
	sf::Vector2f size=sf::Vector2f(global_assets.finish_texture.getSize());
	sf::Sprite sprite{global_assets.finish_texture};
	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
	}
};

struct Wall{
	sf::Vector2f size=sf::Vector2f(global_assets.wall_texture.getSize());
	sf::Sprite sprite{global_assets.wall_texture};
	std::string type="wall"; //wall    bouncy    spike    glue    redwall   bluewall

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
	}
};

struct Checkpoint{
	sf::Vector2f size=sf::Vector2f(global_assets.checkpoint_texture1.getSize());	
	sf::Sprite sprite{global_assets.checkpoint_texture1};
	bool activated=false;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
	}

	void activate(){
		sf::Angle angle=sf::degrees(-90);
		sprite.setRotation(angle);
		activated=true;
	}

};

struct Player{
	sf::Vector2f size=sf::Vector2f(global_assets.player_texture.getSize());	
	sf::Vector2f velocity={0.f,0.f};
	sf::Vector2f current_checkpoint={0.f,0.f};
	sf::Sprite sprite{global_assets.player_texture};
	bool is_touching_down=false;
	bool is_touching_up=false;
	bool is_touching_left=false;
	bool is_touching_right=false;
	float checkpoint_countdown=50;
	int current_level=1;
	int checkpoint_number=0;
	std::string gamestate="menu"; //playing   escape  win   menu   level_selector     settings
	Finish finish;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		current_checkpoint=setup_coords;
		velocity={0.f,0.f};
		current_checkpoint=setup_coords;
		checkpoint_number=0;
	}
	
	void wall_collision_x(TheWholeLevel& the_whole_level);

	void wall_collision_y(TheWholeLevel& the_whole_level);

	void gravity(TheWholeLevel& the_whole_level);
	
	void jumpIfPossible(TheWholeLevel& the_whole_level){
		if (is_touching_down){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
				if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){velocity.y-=15;}
			}
		}
	}
	void sideways_movement(TheWholeLevel& the_whole_level);
	void debug_movement(TheWholeLevel& the_whole_level);

	void checkpoint_colliion(TheWholeLevel& the_whole_level);

	void respawn_player(TheWholeLevel& the_whole_level);

	void finish_collision(){
		if (sprite.getGlobalBounds().findIntersection(finish.sprite.getGlobalBounds())){
			gamestate="escape";
		}
	}

};

struct Camera{
	sf::View view;

	void setup(){
		sf::FloatRect rect({0.f,0.f}, {1920.f,1080.f});
		view=sf::View(rect);
		//if (DEBUGGING_LEVELS_AND_BUILDING_THEM){view.zoom(2.f);} 
	}

	void follow_player(TheWholeLevel& the_whole_level);
};

struct Sky{
	sf::Vector2f size=sf::Vector2f(global_assets.sky_texture.getSize());	
	sf::Sprite sprite{global_assets.sky_texture};
	float scalex=6;
	float scaley=6;
	void draw(TheWholeLevel& the_whole_level);
};

struct WinScreen{
	sf::Vector2f size=sf::Vector2f(global_assets.Win0_texture.getSize());
	sf::Sprite sprite{global_assets.Win0_texture};
	int state=0;
	sf::Vector2f coords=sf::Vector2f({600,200});
	float scale=5;

	void checkmouse(TheWholeLevel& the_whole_level);
	void draw(TheWholeLevel& the_whole_level);
};

struct TheWholeLevel{
	sf::RenderWindow window;
	Input input;
	Player player;
	std::vector<Wall> walls;
	std::vector<Checkpoint> checkpoints;
	Camera camera;
	WinScreen winscreen;
	std::vector<std::string> new_walls;
	Sky sky;
	Menu menu;
	float dt=0;

	void setup(){
		window.create(sf::VideoMode({1920, 1080}), "SFML works!", sf::State::Fullscreen);
		window.setVerticalSyncEnabled(true);
		//window.setFramerateLimit(60);
		camera.setup();
		player.current_level=1;
	}
};

void LoadLevel(TheWholeLevel& the_whole_level){
	std::string level_load_string="../../assets/levels/";
	level_load_string+=(std::to_string(the_whole_level.player.current_level));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	the_whole_level.walls.clear();
	the_whole_level.checkpoints.clear();
	float x,y,l;
	static Wall wall;

	while (file>>type){
		if (type=="player"){
			file>>x>>y;
			the_whole_level.player.setup({x*64,y*64});
		}
		if (type=="wall"){
			file>>x>>y;
			wall.setup({x*64,y*64});
			the_whole_level.walls.push_back(wall);
		}
		if (type=="wallline"){
			int plus=1;
			file>>x>>y>>l>>type;
			if (l<0){l=-l;plus=-1;}
			if (type=="hori"){
				for (int i=0;i<l;i++){
				wall.setup({(x+i*plus)*64,y*64});
				the_whole_level.walls.push_back(wall);
				}
			} else {
				for (int i=0;i<l;i++){
				wall.setup({x*64,(y+i*plus)*64});
				the_whole_level.walls.push_back(wall);
				}
			}
		}
		if (type=="checkpoint"){
			file>>x>>y;
			Checkpoint checkpoint;
			checkpoint.setup({x*64,y*64});
			the_whole_level.checkpoints.push_back(checkpoint);
		}
		if (type=="finish"){
			file>>x>>y;
			the_whole_level.player.finish.setup({x*64,y*64});
		}
	}
}

void mouse_block_placing(TheWholeLevel& the_whole_level){
	std::string new_wall="wall ";
	sf::Vector2i mouse_coords;
	mouse_coords=sf::Mouse::getPosition(the_whole_level.window);
	sf::Vector2f mouse_true_coords=the_whole_level.window.mapPixelToCoords(mouse_coords);
	int cordx=int(floor(mouse_true_coords.x/64.f));
	int cordy=int(floor(mouse_true_coords.y/64.f));
	std::string cordx_string=std::to_string(cordx);
	std::string cordy_string=std::to_string(cordy);
	std::string cord_add_string=cordx_string+" "+cordy_string;
	if (the_whole_level.input.LShift){
		the_whole_level.player.sprite.setPosition({cordx*64.f,cordy*64.f});
		std::cout<<cordx<<" "<<cordy<<'\n';
	}
	if (the_whole_level.input.Mouse1){
		bool block_is_there=false;
		for (int i=0;i<the_whole_level.walls.size();i++){
			if (int(the_whole_level.walls[i].sprite.getPosition().x/64)==cordx && int(the_whole_level.walls[i].sprite.getPosition().y/64)==cordy){
				block_is_there=true;break;
			}
		}
		if (!block_is_there){
			new_wall+=cord_add_string;
			the_whole_level.new_walls.push_back(new_wall);
			Wall wall;
			wall.setup({cordx*64.f,cordy*64.f});
			the_whole_level.walls.push_back(wall);
		}
	}
	
	if (the_whole_level.input.Mouse2){the_whole_level.walls.pop_back();the_whole_level.new_walls.pop_back();}

	if (the_whole_level.input.F9){
		std::cout<<"start"<<'\n';
		for (int i=0;i<the_whole_level.new_walls.size();i++){
			std::cout<<the_whole_level.new_walls[i]<<'\n';
		}
		std::cout<<"end"<<'\n';
	}	
}


void Debugging(TheWholeLevel& the_whole_level){
	if (the_whole_level.input.F1){DEBUGGING_LEVELS_AND_BUILDING_THEM=true;}
	if (the_whole_level.input.F2){DEBUGGING_LEVELS_AND_BUILDING_THEM=false;}
	if (the_whole_level.input.PageUp){LoadLevel(the_whole_level);the_whole_level.camera.setup();the_whole_level.new_walls.clear();}
	if (DEBUGGING_LEVELS_AND_BUILDING_THEM){
		the_whole_level.player.debug_movement(the_whole_level);
	}
	if (CHEAT_MODE){mouse_block_placing(the_whole_level);}
}

void draw_player(TheWholeLevel& the_whole_level){
	the_whole_level.window.draw(the_whole_level.player.sprite);
	the_whole_level.window.draw(the_whole_level.player.finish.sprite);
}

void draw_walls(TheWholeLevel& the_whole_level){
	for (int i=0;i<the_whole_level.walls.size();i++){
		sf::FloatRect rect{the_whole_level.camera.view.getCenter()-the_whole_level.camera.view.getSize()/2.f,the_whole_level.camera.view.getSize()};
		if (the_whole_level.walls[i].sprite.getGlobalBounds().findIntersection(rect)){
			the_whole_level.window.draw(the_whole_level.walls[i].sprite);
		}
	}
}

void draw_checkpoints(TheWholeLevel& the_whole_level){
	sf::RectangleShape bar;
	bar.setPosition({10,1070});
	bar.setFillColor(sf::Color(0,255,0,255));
	bar.setSize({(50.f-float(the_whole_level.player.checkpoint_countdown))*5.f,64.f});
	bar.setRotation(sf::degrees(-90));
	sf::RectangleShape bar2;
	bar2.setPosition({10,1070});
	bar2.setFillColor(sf::Color(0,0,0,0));
	bar2.setOutlineThickness(10.f);
	bar2.setOutlineColor(sf::Color(0,255,0,255));
	bar2.setSize({47.f*5.f,64.f});
	bar2.setRotation(sf::degrees(-90));
	for (int i=0;i<the_whole_level.checkpoints.size();i++){
		the_whole_level.window.draw(the_whole_level.checkpoints[i].sprite);
	}
	if (the_whole_level.player.checkpoint_countdown<50){
	the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
	the_whole_level.window.draw(bar);
	the_whole_level.window.draw(bar2);
	the_whole_level.window.setView(the_whole_level.camera.view);
	}
}

int main()
{
	global_assets.LoadAllTextures();
	TheWholeLevel the_whole_level;
	the_whole_level.setup();

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	LoadLevel(the_whole_level);

	while (the_whole_level.window.isOpen())
	{
		the_whole_level.input.reset();
		while (const std::optional event = the_whole_level.window.pollEvent()){
			if (event->is<sf::Event::Closed>()){the_whole_level.window.close();}
			the_whole_level.input.read(event);
		}

		the_whole_level.dt=delta_clock.getElapsedTime()/delta_time;
		delta_clock.restart();
		if (the_whole_level.dt>5){the_whole_level.dt=5;}
	//playing state
		if (the_whole_level.player.gamestate=="playing"){
			if (CHEAT_MODE){Debugging(the_whole_level);}
			the_whole_level.player.finish_collision();
		//respawning
			the_whole_level.player.checkpoint_colliion(the_whole_level);
			the_whole_level.player.respawn_player(the_whole_level);
		//changing velocity
			the_whole_level.player.sideways_movement(the_whole_level);
			the_whole_level.player.jumpIfPossible(the_whole_level);
			the_whole_level.player.gravity(the_whole_level);
		//moving
			the_whole_level.player.sprite.move({the_whole_level.player.velocity.x*the_whole_level.dt,0.f});
			if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){the_whole_level.player.wall_collision_x(the_whole_level);}
			the_whole_level.player.sprite.move({0.f,the_whole_level.player.velocity.y*the_whole_level.dt});
			if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){the_whole_level.player.wall_collision_y(the_whole_level);}
			the_whole_level.camera.follow_player(the_whole_level);
		}
	//menu state
		if (the_whole_level.player.gamestate=="menu"){
			the_whole_level.menu.checkmouse(the_whole_level);
		}
	//check escape press
		if (the_whole_level.input.Escape){
			if (the_whole_level.player.gamestate=="playing"){
				the_whole_level.player.gamestate="escape";
			} else {
				if (the_whole_level.player.gamestate=="escape" || the_whole_level.player.gamestate=="win"){
					the_whole_level.player.gamestate="playing";
				}
			}
		}
	//check winscreen 
		if (the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.winscreen.checkmouse(the_whole_level);
		}

	//DRAWING
		the_whole_level.window.clear();
	//inside of the level
		if (the_whole_level.player.gamestate=="playing" || the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.sky.draw(the_whole_level);
			the_whole_level.window.setView(the_whole_level.camera.view);
			draw_walls(the_whole_level);
			draw_player(the_whole_level);
			draw_checkpoints(the_whole_level);	
		}
	//escape screen
		if (the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.winscreen.draw(the_whole_level);
		}
	//menu screen
		if (the_whole_level.player.gamestate=="menu"){
			the_whole_level.menu.draw(the_whole_level);
		}
		the_whole_level.window.display();
		//sf::sleep(sf::milliseconds(200));
	}
}






//FUNCTIONS

void WinScreen::checkmouse(TheWholeLevel& the_whole_level){
		sf::FloatRect button1({coords.x+24*scale,coords.y+24*scale},{79*scale,15*scale});
		sf::FloatRect button2({coords.x+24*scale,coords.y+48*scale},{79*scale,15*scale});
		sf::FloatRect button3({coords.x+24*scale,coords.y+72*scale},{79*scale,15*scale});
		sf::Vector2i mouse_coords;
		mouse_coords=sf::Mouse::getPosition(the_whole_level.window);
		sf::Vector2f mouse_true_coords=the_whole_level.window.mapPixelToCoords(mouse_coords);
		//mouse_true_coords=sf::Vector2f(mouse_coords);
		if (button1.contains(mouse_true_coords)){
			sprite.setTexture(global_assets.Win1_texture);
			if (the_whole_level.input.Mouse1){
				the_whole_level.player.current_level++;
				if (the_whole_level.player.current_level>3){the_whole_level.player.current_level=1;}
				the_whole_level.player.gamestate="playing";
				LoadLevel(the_whole_level);
			}
		} else {
			if (button2.contains(mouse_true_coords)){
				sprite.setTexture(global_assets.Win2_texture);		
				if (the_whole_level.input.Mouse1){
					the_whole_level.player.gamestate="menu";
				}		
			} else {
				if (button3.contains(mouse_true_coords)){
					sprite.setTexture(global_assets.Win3_texture);
					if (the_whole_level.input.Mouse1){
						the_whole_level.player.gamestate="playing";
						LoadLevel(the_whole_level);
					}
				} else {
					sprite.setTexture(global_assets.Win0_texture);
				}
			}
		}
	}

void WinScreen::draw(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		sprite.setPosition({coords.x,coords.y});
		sprite.setScale({scale,scale});
		the_whole_level.window.draw(sprite);
	}


	void Player::wall_collision_x(TheWholeLevel& the_whole_level){
		is_touching_right=false;
		is_touching_left=false;
		is_touching_up=false;
		is_touching_down=false;
		sf::FloatRect player_rect{sprite.getGlobalBounds()};
		for (int i=0;i<the_whole_level.walls.size();i++){
			if (player_rect.findIntersection(the_whole_level.walls[i].sprite.getGlobalBounds())){
				if (velocity.x>0){
					is_touching_right=true;
					is_touching_left=false;
					sprite.move({the_whole_level.walls[i].sprite.getPosition().x-sprite.getPosition().x-size.x,0.f});
				} else{
					is_touching_left=true;
					is_touching_right=false;
					sprite.move({the_whole_level.walls[i].sprite.getPosition().x-sprite.getPosition().x+size.x,0.f});
				}	
				//velocity.x=-velocity.x;
				if (the_whole_level.walls[i].type=="bouncy"){
					if (velocity.y>1){velocity.x*=-1;} else {velocity.x=0;}
				}
				if (the_whole_level.walls[i].type=="wall"){velocity.x=0;}

				break;
			}
		}
	}

	void Player::wall_collision_y(TheWholeLevel& the_whole_level){
		sf::FloatRect player_rect{sprite.getGlobalBounds()};
		for (int i=0;i<the_whole_level.walls.size();i++){
			if (player_rect.findIntersection(the_whole_level.walls[i].sprite.getGlobalBounds())){
				if (velocity.y>0){
					is_touching_down=true;
					is_touching_up=false;
					sprite.move({0.f,the_whole_level.walls[i].sprite.getPosition().y-sprite.getPosition().y-size.y});
				} else{
					is_touching_up=true;
					is_touching_down=false;
					sprite.move({0.f,the_whole_level.walls[i].sprite.getPosition().y-sprite.getPosition().y+size.y});
				}	
				//velocity.y=-velocity.y;
				if (the_whole_level.walls[i].type=="bouncy"){
					if (velocity.y>1){velocity.y*=-1.f;} else {velocity.y=0;}
					
				}
				if (the_whole_level.walls[i].type=="wall"){velocity.y=0;}

				break;
			}
		}
	}

	void Player::gravity(TheWholeLevel& the_whole_level){
		if (!is_touching_down & !DEBUGGING_LEVELS_AND_BUILDING_THEM){
			velocity.y+=0.5*the_whole_level.dt;
		}
	}

	void Player::sideways_movement(TheWholeLevel& the_whole_level){
		static float top_speed=10;
		static float speed_loss=0.4;
		static float acceleration=0.4;
		bool move=false;
		float mult=1;
		float lossmult=0.10;
		if (is_touching_down){
			mult=2;
			lossmult=1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || the_whole_level.input.right){
			velocity.x=std::min(velocity.x+acceleration*mult*the_whole_level.dt,top_speed);
			move=true;
		} 

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || the_whole_level.input.left){
			velocity.x=std::max(velocity.x-acceleration*mult*the_whole_level.dt,-top_speed);
			move=true;
		} 
		if (!move){velocity.x-=velocity.x*std::min(1.f,speed_loss*the_whole_level.dt*lossmult);}	
			
		if (!move && abs(velocity.x)<0.1f){velocity.x=0.f;}
	}

	void Player::debug_movement(TheWholeLevel& the_whole_level){
		static float top_speed=10;
		static float speed_loss=0.4;
		static float acceleration=0.4;
		bool move=false;
		float mult=1;
		float lossmult=0.10;
		if (is_touching_down){
			mult=2;
			lossmult=1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || the_whole_level.input.down){
			velocity.y=std::min(velocity.y+acceleration*mult*the_whole_level.dt,top_speed);
			move=true;
		} 

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || the_whole_level.input.up){
			velocity.y=std::max(velocity.y-acceleration*mult*the_whole_level.dt,-top_speed);
			move=true;
		} 
		if (!move){velocity.y-=velocity.y*std::min(1.f,speed_loss*the_whole_level.dt*lossmult);}	
			
		if (!move && abs(velocity.y)<0.1f){velocity.y=0.f;}
	};

	void Player::checkpoint_colliion(TheWholeLevel& the_whole_level){
		for (int i=0;i<the_whole_level.checkpoints.size();i++){
			if (sprite.getGlobalBounds().findIntersection(the_whole_level.checkpoints[i].sprite.getGlobalBounds())){
				if (i>=checkpoint_number){
					the_whole_level.checkpoints[i].activate();
					current_checkpoint=the_whole_level.checkpoints[i].sprite.getPosition();
					checkpoint_number=i;
				}		
			}
			if (i<checkpoint_number){the_whole_level.checkpoints[i].activate();}
		}
	}

	void Player::respawn_player(TheWholeLevel& the_whole_level){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) || the_whole_level.input.F){
			checkpoint_countdown-=2*the_whole_level.dt;
		} else {checkpoint_countdown=50;}
		if (checkpoint_countdown<=0){
			checkpoint_countdown=50;
			sprite.setPosition(current_checkpoint);
		}
	}

	void Camera::follow_player(TheWholeLevel& the_whole_level){
		static float boundry_x=100;
		static float boundry_y=200;
		boundry_x=50;boundry_y=50;
		static float boundry_speed=0.05;
		float dt=the_whole_level.dt;
		sf::Vector2f player_pos=the_whole_level.player.sprite.getPosition();
		sf::Vector2f camera_pos=view.getCenter();
		if (player_pos.y-camera_pos.y<-boundry_y){
			view.move({0.f,(player_pos.y-camera_pos.y+boundry_y)*boundry_speed*dt});
		}
		if (player_pos.y-camera_pos.y>boundry_y){
			view.move({0.f,(player_pos.y-camera_pos.y-boundry_y)*boundry_speed*dt});
		}
		if (player_pos.x-camera_pos.x<-boundry_x){
			view.move({(player_pos.x-camera_pos.x+boundry_x)*boundry_speed*dt,0.f});
		}
		if (player_pos.x-camera_pos.x>boundry_x){
			view.move({(player_pos.x-camera_pos.x-boundry_x)*boundry_speed*dt,0.f});
		}
	}

	void Sky::draw(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		//std::cout<<true_world_size_in_blocks.left<<'\n';
		sprite.setPosition({0,0});
		//sprite.setPosition({true_world_size_in_blocks.left*64,true_world_size_in_blocks.top*64});
		//scalex=((true_world_size_in_blocks.right*64+1000)-(true_world_size_in_blocks.left*64-1000))/320.f;
		//scaley=((true_world_size_in_blocks.bottom*64+1000)-(true_world_size_in_blocks.top*64-1000))/180.f;
		sprite.setScale({scalex,scaley});
		the_whole_level.window.draw(sprite);
		the_whole_level.window.setView(the_whole_level.camera.view);
	}

	void Menu::checkmouse(TheWholeLevel& the_whole_level){
		sf::FloatRect button1({96*scale,32*scale},{127*scale,19*scale});
		sf::FloatRect button2({96*scale,64*scale},{127*scale,19*scale});
		sf::FloatRect button3({96*scale,96*scale},{127*scale,19*scale});
		sf::FloatRect button4({96*scale,128*scale},{127*scale,19*scale});
		sf::Vector2i mouse_coords;
		mouse_coords=sf::Mouse::getPosition(the_whole_level.window);
		sf::Vector2f mouse_true_coords=the_whole_level.window.mapPixelToCoords(mouse_coords);
		if (button1.contains(mouse_true_coords)){
			sprite.setTexture(global_assets.Menu1_texture);
			if (the_whole_level.input.Mouse1){
				the_whole_level.player.gamestate="playing";
				LoadLevel(the_whole_level);
			}
		} else {
			if (button2.contains(mouse_true_coords)){
				sprite.setTexture(global_assets.Menu2_texture);	
				if (the_whole_level.input.Mouse1){
					the_whole_level.player.gamestate="level_selector";
				}			
			} else {
				if (button3.contains(mouse_true_coords)){
					sprite.setTexture(global_assets.Menu3_texture);
					if (the_whole_level.input.Mouse1){
						the_whole_level.player.gamestate="settings";
					}
				} else {
					if (button4.contains(mouse_true_coords)){
					sprite.setTexture(global_assets.Menu4_texture);
					if (the_whole_level.input.Mouse1){
						the_whole_level.window.close();
					}
					} else{
						sprite.setTexture(global_assets.Menu0_texture);
					}
				}
			}
		}
	}

void Menu::draw(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		sprite.setPosition({0,0});
		sprite.setScale({scale,scale});
		the_whole_level.window.draw(sprite);
	}