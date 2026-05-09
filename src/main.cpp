#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

bool DEBUGGING_LEVELS_AND_BUILDING_THEM=false;
bool CHEAT_MODE=true;

struct Input{
	bool M;
	bool Escape;
	bool Mouse1;
	bool Mouse2;
	bool F9;
	bool LShift;
	bool left;
	bool right;
	bool down;
	bool up;

	void read(const std::optional<sf::Event>& event){
		if (const auto* key=event->getIf<sf::Event::KeyPressed>()){
			if (key->code == sf::Keyboard::Key::M){M=true;}
			if (key->code == sf::Keyboard::Key::Escape){Escape=true;}
			if (key->code == sf::Keyboard::Key::F9){F9=true;}
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
	sf::Texture sky_texture;
	sf::Texture Win0_texture;
	sf::Texture Win1_texture;
	sf::Texture Win2_texture;
	sf::Texture Win3_texture;
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
	}
};

Assets global_assets;

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
	int checkpoint_countdown=50;
	int current_level=1;
	int checkpoint_number=0;
	std::string gamestate="playing"; //playing   escape  win   menu   level_selector
	Finish finish;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		current_checkpoint=setup_coords;
		velocity={0.f,0.f};
		current_checkpoint=setup_coords;
		checkpoint_number=0;
	}
	
	void wall_collision_x(std::vector<Wall>& walls,float dt){
		is_touching_right=false;
		is_touching_left=false;
		is_touching_up=false;
		is_touching_down=false;
		for (int i=0;i<walls.size();i++){
			if (sprite.getGlobalBounds().findIntersection(walls[i].sprite.getGlobalBounds())){
				if (velocity.x>0){
					is_touching_right=true;
					is_touching_left=false;
					sprite.move({walls[i].sprite.getPosition().x-sprite.getPosition().x-size.x,0.f});
				} else{
					is_touching_left=true;
					is_touching_right=false;
					sprite.move({walls[i].sprite.getPosition().x-sprite.getPosition().x+size.x,0.f});
				}	
				//velocity.x=-velocity.x;
				if (walls[i].type=="bouncy"){
					if (velocity.y>1){velocity.x*=-1;} else {velocity.x=0;}
				}
				if (walls[i].type=="wall"){velocity.x=0;}
			}
		}
	}

	void wall_collision_y(std::vector<Wall>& walls, float dt){
		for (int i=0;i<walls.size();i++){
			if (sprite.getGlobalBounds().findIntersection(walls[i].sprite.getGlobalBounds())){
				if (velocity.y>0){
					is_touching_down=true;
					is_touching_up=false;
					sprite.move({0.f,walls[i].sprite.getPosition().y-sprite.getPosition().y-size.y});
				} else{
					is_touching_up=true;
					is_touching_down=false;
					sprite.move({0.f,walls[i].sprite.getPosition().y-sprite.getPosition().y+size.y});
				}	
				//velocity.y=-velocity.y;
				if (walls[i].type=="bouncy"){
					if (velocity.y>1){velocity.y*=-1.f;} else {velocity.y=0;}
					
				}
				if (walls[i].type=="wall"){velocity.y=0;}
			}
		}
	}

	void gravity(float dt){
		if (!is_touching_down & !DEBUGGING_LEVELS_AND_BUILDING_THEM){
			velocity.y+=0.5*dt;
		}
	}
	
	void jumpIfPossible(Input input){
		if (is_touching_down){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
				if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){velocity.y-=15;}
			}
		}
	}
	void sideways_movement(float dt, Input input){
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
			velocity.x=std::min(velocity.x+acceleration*mult*dt,top_speed);
			move=true;
		} 

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
			velocity.x=std::max(velocity.x-acceleration*mult*dt,-top_speed);
			move=true;
		} 
		if (!move){velocity.x-=velocity.x*speed_loss*dt*lossmult;}	
			
		if (!move && abs(velocity.x)<0.1f){velocity.x=0.f;}
	}

	void debug_movement(float dt){
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
			velocity.y=std::min(velocity.y+acceleration*mult*dt,top_speed);
			move=true;
		} 

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
			velocity.y=std::max(velocity.y-acceleration*mult*dt,-top_speed);
			move=true;
		} 
		if (!move){velocity.y-=velocity.y*speed_loss*dt*lossmult;}	
			
		if (!move && abs(velocity.y)<0.1f){velocity.y=0.f;}
	};

	void checkpoint_colliion(std::vector<Checkpoint>& checkpoints){
		for (int i=0;i<checkpoints.size();i++){
			if (sprite.getGlobalBounds().findIntersection(checkpoints[i].sprite.getGlobalBounds())){
				if (i>=checkpoint_number){
					checkpoints[i].activate();
					current_checkpoint=checkpoints[i].sprite.getPosition();
					checkpoint_number=i;
				}		
			}
			if (i<checkpoint_number){checkpoints[i].activate();}
		}
	}

	void respawn_player(){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){
			checkpoint_countdown-=2;
		} else {checkpoint_countdown=50;}
		if (checkpoint_countdown<=0){
			checkpoint_countdown=50;
			sprite.setPosition(current_checkpoint);
		}
	}

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

	void follow_player(Player& player){
		static float boundry_x=100;
		static float boundry_y=200;
		boundry_x=50;boundry_y=50;
		static float boundry_speed=0.05;
		sf::Vector2f player_pos=player.sprite.getPosition();
		sf::Vector2f camera_pos=view.getCenter();
		if (player_pos.y-camera_pos.y<-boundry_y){
			view.move({0.f,(player_pos.y-camera_pos.y+boundry_y)*boundry_speed});
		}
		if (player_pos.y-camera_pos.y>boundry_y){
			view.move({0.f,(player_pos.y-camera_pos.y-boundry_y)*boundry_speed});
		}
		if (player_pos.x-camera_pos.x<-boundry_x){
			view.move({(player_pos.x-camera_pos.x+boundry_x)*boundry_speed,0.f});
		}
		if (player_pos.x-camera_pos.x>boundry_x){
			view.move({(player_pos.x-camera_pos.x-boundry_x)*boundry_speed,0.f});
		}
	}
};

struct WorldSize{
	float left=0;
	float right=0;
	float top=0;
	float bottom=0;
	void setup(float x,float y){
		left=x;
		right=x;
		top=y;
		bottom=y;
	}

	void change(float x,float y){
		right=std::max(right,x);
		bottom=std::max(bottom,y);
		left=std::min(right,x);
		top=std::min(bottom,y);
	}

};
WorldSize true_world_size_in_blocks;
struct Sky{
	sf::Vector2f size=sf::Vector2f(global_assets.sky_texture.getSize());	
	sf::Sprite sprite{global_assets.sky_texture};
	float scalex=6;
	float scaley=6;
	void draw(sf::RenderWindow& window,Camera& camera,Player& player,float dt){
		std::cout<<true_world_size_in_blocks.left<<'\n';
		sprite.setPosition({true_world_size_in_blocks.left*64,true_world_size_in_blocks.top*64});
		scalex=((true_world_size_in_blocks.right*64+1000)-(true_world_size_in_blocks.left*64-1000))/320.f;
		scaley=((true_world_size_in_blocks.bottom*64+1000)-(true_world_size_in_blocks.top*64-1000))/180.f;
		window.setView(camera.view);
		sprite.setScale({scalex,scaley});
		window.draw(sprite);
	}
};

struct TheWholeLevel{
	sf::RenderWindow window;
	Input input;
	Player player;
	std::vector<Wall> walls;
	std::vector<Checkpoint> checkpoints;
	Camera camera;
	std::vector<std::string> new_walls;
	Sky sky;

	void setup(){
		window=sf::RenderWindow(sf::VideoMode( { 1920, 1080 } ), "SFML works!",sf::State::Fullscreen);
		camera.setup();
		player.current_level=2;
	}
};

void LoadLevel(Player& player,std::vector<Wall>& walls,std::vector<Checkpoint>& checkpoints){
	std::string level_load_string="../../assets/levels/";
	level_load_string+=(std::to_string(player.current_level));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	walls.clear();
	checkpoints.clear();
	float x,y,l;
	static Wall wall;

	while (file>>type){
		if (type=="player"){
			file>>x>>y;
			player.setup({x*64,y*64});
		}
		if (type=="wall"){
			file>>x>>y;
			wall.setup({x*64,y*64});
			walls.push_back(wall);
		}
		if (type=="wallline"){
			int plus=1;
			file>>x>>y>>l>>type;
			if (l<0){l=-l;plus=-1;}
			if (type=="hori"){
				for (int i=0;i<l;i++){
				wall.setup({(x+i*plus)*64,y*64});
				walls.push_back(wall);
				}
			} else {
				for (int i=0;i<l;i++){
				wall.setup({x*64,(y+i*plus)*64});
				walls.push_back(wall);
				}
			}
		}
		if (type=="checkpoint"){
			file>>x>>y;
			Checkpoint checkpoint;
			checkpoint.setup({x*64,y*64});
			checkpoints.push_back(checkpoint);
		}
		if (type=="finish"){
			file>>x>>y;
			player.finish.setup({x*64,y*64});
		}

		true_world_size_in_blocks.change(x,y);
	}
}

struct WinScreen{
	sf::Vector2f size=sf::Vector2f(global_assets.Win0_texture.getSize());
	sf::Sprite sprite{global_assets.Win0_texture};
	int state=0;
	sf::Vector2f coords=sf::Vector2f({600,200});
	float scale=5;

	void checkmouse(sf::RenderWindow& window, Player& player, Input& input,std::vector<Wall>& walls,std::vector<Checkpoint>& checkpoints){
		sf::FloatRect button1({coords.x+24*scale,coords.y+24*scale},{79*scale,15*scale});
		sf::FloatRect button2({coords.x+24*scale,coords.y+48*scale},{79*scale,15*scale});
		sf::FloatRect button3({coords.x+24*scale,coords.y+72*scale},{79*scale,15*scale});
		sf::Vector2i mouse_coords;
		mouse_coords=sf::Mouse::getPosition(window);
		sf::Vector2f mouse_true_coords=window.mapPixelToCoords(mouse_coords);
		//mouse_true_coords=sf::Vector2f(mouse_coords);
		if (button1.contains(mouse_true_coords)){
			sprite.setTexture(global_assets.Win1_texture);
			if (input.Mouse1){
				player.current_level++;
				if (player.current_level>3){player.current_level=1;}
				player.gamestate="playing";
				LoadLevel(player,walls,checkpoints);
			}
		} else {
			if (button2.contains(mouse_true_coords)){
				sprite.setTexture(global_assets.Win2_texture);				
			} else {
				if (button3.contains(mouse_true_coords)){
					sprite.setTexture(global_assets.Win3_texture);
					if (input.Mouse1){
						player.gamestate="playing";
						LoadLevel(player,walls,checkpoints);
					}
				} else {
					sprite.setTexture(global_assets.Win0_texture);
				}
			}
		}
	}
	void draw(sf::RenderWindow& window){
		window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		sprite.setPosition({coords.x,coords.y});
		sprite.setScale({scale,scale});
		window.draw(sprite);
	}
};

void mouse_block_placing(sf::RenderWindow& window,std::vector<Wall>& walls,std::vector<std::string>& new_walls,Input& input,Player& player){
	std::string new_wall="wall ";
	sf::Vector2i mouse_coords;
	mouse_coords=sf::Mouse::getPosition(window);
	sf::Vector2f mouse_true_coords=window.mapPixelToCoords(mouse_coords);
	int cordx=int(floor(mouse_true_coords.x/64.f));
	int cordy=int(floor(mouse_true_coords.y/64.f));
	std::string cordx_string=std::to_string(cordx);
	std::string cordy_string=std::to_string(cordy);
	std::string cord_add_string=cordx_string+" "+cordy_string;
	if (input.LShift){
		player.sprite.setPosition({cordx*64.f,cordy*64.f});
		std::cout<<cordx<<" "<<cordy<<'\n';
	}
	if (input.Mouse1){
		bool block_is_there=false;
		for (int i=0;i<walls.size();i++){
			if (int(walls[i].sprite.getPosition().x/64)==cordx && int(walls[i].sprite.getPosition().y/64)==cordy){
				block_is_there=true;break;
			}
		}
		if (!block_is_there){
			new_wall+=cord_add_string;
			new_walls.push_back(new_wall);
			Wall wall;
			wall.setup({cordx*64.f,cordy*64.f});
			walls.push_back(wall);
		}
	}
	
	if (input.Mouse2){walls.pop_back();new_walls.pop_back();}

	if (input.F9){
		std::cout<<"start"<<'\n';
		for (int i=0;i<new_walls.size();i++){
			std::cout<<new_walls[i]<<'\n';
		}
		std::cout<<"end"<<'\n';
	}	
}


void Debugging(Player& player,std::vector<Wall>& walls, Camera& camera, float dt, sf::RenderWindow& window, Input& input,
	std::vector<std::string>& new_walls,std::vector<Checkpoint>& checkpoints){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)){DEBUGGING_LEVELS_AND_BUILDING_THEM=true;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)){DEBUGGING_LEVELS_AND_BUILDING_THEM=false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageUp)){LoadLevel(player,walls,checkpoints);camera.setup();new_walls.clear();}
	if (DEBUGGING_LEVELS_AND_BUILDING_THEM){
		player.debug_movement(dt);
	}
	if (CHEAT_MODE){mouse_block_placing(window,walls,new_walls,input,player);}
}

void draw_player(Player& player,sf::RenderWindow& window){
	window.draw(player.sprite);
	window.draw(player.finish.sprite);
}

void draw_walls(std::vector<Wall>& walls,sf::RenderWindow& window){
	for (int i=0;i<walls.size();i++){
		window.draw(walls[i].sprite);
	}
}

void draw_checkpoints(std::vector<Checkpoint>& checkpoints,sf::RenderWindow& window, Player& player,Camera& camera){
	sf::RectangleShape bar;
	bar.setPosition({10,1070});
	bar.setFillColor(sf::Color(0,255,0,255));
	bar.setSize({(50.f-float(player.checkpoint_countdown))*5.f,64.f});
	bar.setRotation(sf::degrees(-90));
	sf::RectangleShape bar2;
	bar2.setPosition({10,1070});
	bar2.setFillColor(sf::Color(0,0,0,0));
	bar2.setOutlineThickness(10.f);
	bar2.setOutlineColor(sf::Color(0,255,0,255));
	bar2.setSize({47.f*5.f,64.f});
	bar2.setRotation(sf::degrees(-90));
	for (int i=0;i<checkpoints.size();i++){
		window.draw(checkpoints[i].sprite);
	}
	if (player.checkpoint_countdown<50){
	window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
	window.draw(bar);
	window.draw(bar2);
	window.setView(camera.view);
	}
}

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1920, 1080 } ), "SFML works!",sf::State::Fullscreen);
	window.setVerticalSyncEnabled(true);
	global_assets.LoadAllTextures();
	Input input;
	Player player;
	player.current_level=2;
	std::vector<Wall> walls;
	std::vector<Checkpoint> checkpoints;
	Camera camera;
	camera.setup();
	std::vector<std::string> new_walls;
	WinScreen winscreen;
	Sky sky;

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	float dt;
	LoadLevel(player,walls,checkpoints);

	while (window.isOpen())
	{
		input.reset();
		while (const std::optional event = window.pollEvent()){
			if (event->is<sf::Event::Closed>()){window.close();}
			input.read(event);
		}

		dt=delta_clock.getElapsedTime()/delta_time;
		delta_clock.restart();
		if (dt>5){dt=5;}

		if (player.gamestate=="playing"){
		if (CHEAT_MODE){Debugging(player,walls,camera,dt,window,input,new_walls,checkpoints);}
		player.finish_collision();
		player.checkpoint_colliion(checkpoints);
		player.respawn_player();
		player.sideways_movement(dt,input);
		player.jumpIfPossible(input);
		player.gravity(dt);
		player.sprite.move({player.velocity.x*dt,0.f});
		if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){player.wall_collision_x(walls,dt);}
		player.sprite.move({0.f,player.velocity.y*dt});
		if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){player.wall_collision_y(walls,dt);}
		camera.follow_player(player);
		}
		if (input.Escape){
			if (player.gamestate=="playing"){
				player.gamestate="escape";
			} else {
				if (player.gamestate=="escape"){
					player.gamestate="playing";
				}
			}
		}
		if (player.gamestate=="win" || player.gamestate=="escape"){
			winscreen.checkmouse(window,player,input,walls,checkpoints);
		}

		
		window.clear();
		sky.draw(window,camera,player,dt);
		if (player.gamestate=="playing" || player.gamestate=="win" || player.gamestate=="escape"){
		window.setView(camera.view);
		draw_walls(walls,window);
		draw_player(player,window);
		draw_checkpoints(checkpoints,window,player,camera);	
		}
		if (player.gamestate=="win" || player.gamestate=="escape"){
			winscreen.draw(window);
		}
		window.display();
		//sf::sleep(sf::milliseconds(200));
	}
}
