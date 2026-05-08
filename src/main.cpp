#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>


bool DEBUGGING_LEVELS_AND_BUILDING_THEM=false;

struct Input{
	bool M;
	bool Escape;
	bool Mouse1;
	bool Mouse2;
	bool F9;
	bool LShift;

	void read(const std::optional<sf::Event>& event){
		if (const auto* key=event->getIf<sf::Event::KeyPressed>()){
			if (key->code == sf::Keyboard::Key::M){M=true;}
			if (key->code == sf::Keyboard::Key::Escape){Escape=true;}
			if (key->code == sf::Keyboard::Key::F9){F9=true;}
			if (key->code == sf::Keyboard::Key::LShift){LShift=true;}
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
	}

};

struct Assets{
	sf::Font font;
	sf::Texture player_texture;
	sf::Texture wall_texture;
	sf::Texture finish_texture;
	sf::Texture checkpoint_texture;
	sf::Texture sky_texture;
	void LoadAllTextures(){
	//	if (!font.openFromFile("assets/fonts/arial.ttf")){}
		if (!player_texture.loadFromFile("assets/textures/PlayerTexture.png")){}
		if (!wall_texture.loadFromFile("assets/textures/WallTexture.png")){}
		if (!finish_texture.loadFromFile("assets/textures/FinishTexture.png")){}
	}
};

Assets global_assets;

bool AABBcheck(sf::Vector2f cords1,sf::Vector2f size1,sf::Vector2f cords2,sf::Vector2f size2){
	if (cords1.x+size1.x>cords2.x and cords1.x<cords2.x+size2.x and cords1.y+size1.y>cords2.y and cords1.y<cords2.y+size2.y){
		return true;
	}
		return false;
}
struct Wall{
	sf::Vector2f size=sf::Vector2f(global_assets.wall_texture.getSize());
	sf::Sprite sprite{global_assets.wall_texture};

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
	}
};

struct Player{
	sf::Vector2f size=sf::Vector2f(global_assets.player_texture.getSize());	
	sf::Vector2f velocity={0.f,0.f};
	sf::Vector2f checkpoint={0.f,0.f};
	sf::Sprite sprite{global_assets.player_texture};
	bool is_touching_down=false;
	bool is_touching_up=false;
	bool is_touching_left=false;
	bool is_touching_right=false;

	void setup(sf::Vector2f setup_coords){
		if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){sprite.setPosition(setup_coords);}
		checkpoint=setup_coords;
		velocity={0.f,0.f};
		checkpoint={0.f,0.f};
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
				velocity.x=0;
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
				velocity.y=0;
			}
		}
	}

	void gravity(float dt){
		if (!is_touching_down and !DEBUGGING_LEVELS_AND_BUILDING_THEM){
			velocity.y+=0.5*dt;
		}
	}
	
	void jumpIfPossible(){
		if (is_touching_down){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
				velocity.y-=15;
			}
		}
	}
	void sideways_movement(float dt){
		static float top_speed=10;
		static float speed_loss=0.4;
		static float acceleration=0.4;
		float mult=1;
		float lossmult=0.10;
		if (is_touching_down){
			mult=2;
			lossmult=1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
			velocity.x=std::min(velocity.x+acceleration*mult*dt,top_speed);
		} else{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
				velocity.x=std::max(velocity.x-acceleration*mult*dt,-top_speed);
			} else{
				velocity.x-=velocity.x*speed_loss*dt*lossmult;		
			}
			if (abs(velocity.x)<0.1f){velocity.x=0.f;}
		}
	}

	void debug_movement(float dt){
		static float top_speed=10;
		static float speed_loss=0.4;
		static float acceleration=0.4;
		float mult=1;
		float lossmult=0.10;
		if (is_touching_down){
			mult=2;
			lossmult=1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
			velocity.y=std::min(velocity.y+acceleration*mult*dt,top_speed);
		} else{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
				velocity.y=std::max(velocity.y-acceleration*mult*dt,-top_speed);
			} else{
				velocity.y-=velocity.y*speed_loss*dt*lossmult;		
			}
			if (abs(velocity.y)<0.1f){velocity.y=0.f;}
		}
	};

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
		sf::Vector2f player_pos=player.sprite.getPosition();
		sf::Vector2f camera_pos=view.getCenter();
		if (player_pos.y-camera_pos.y<-boundry_y){
			view.move({0.f,player_pos.y-camera_pos.y+boundry_y});
		}
		if (player_pos.y-camera_pos.y>boundry_y){
			view.move({0.f,player_pos.y-camera_pos.y-boundry_y});
		}
		if (player_pos.x-camera_pos.x<-boundry_x){
			view.move({player_pos.x-camera_pos.x+boundry_x,0.f});
		}
		if (player_pos.x-camera_pos.x>boundry_x){
			view.move({player_pos.x-camera_pos.x-boundry_x,0.f});
		}
	}
};

void LoadLevel(Player& player,std::vector<Wall>& walls, int level_number){
	std::string level_load_string="assets/levels/";
	level_load_string+=(std::to_string(level_number));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	walls.clear();
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
	}
}

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
			if (int(walls[i].sprite.getPosition().x/64)==cordx and int(walls[i].sprite.getPosition().y/64)==cordy){
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

void Debugging(Player& player,std::vector<Wall>& walls, Camera& camera, float dt, sf::RenderWindow& window, Input& input,std::vector<std::string>& new_walls){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)){DEBUGGING_LEVELS_AND_BUILDING_THEM=true;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)){DEBUGGING_LEVELS_AND_BUILDING_THEM=false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){LoadLevel(player,walls,1);camera.setup();new_walls.clear();}
	if (DEBUGGING_LEVELS_AND_BUILDING_THEM){
		player.debug_movement(dt);
		mouse_block_placing(window,walls,new_walls,input,player);
	}
	
}

void draw_player(Player& player,sf::RenderWindow& window){
	window.draw(player.sprite);
}

void draw_walls(std::vector<Wall>& walls,sf::RenderWindow& window){
	for (int i=0;i<walls.size();i++){
		window.draw(walls[i].sprite);
	}
}

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1920, 1080 } ), "SFML works!",sf::State::Fullscreen);
	window.setVerticalSyncEnabled(true);
	global_assets.LoadAllTextures();
	Input input;
	Player player;
	std::vector<Wall> walls;
	Camera camera;
	camera.setup();
	std::vector<std::string> new_walls;

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	float dt;
	LoadLevel(player,walls,1);

	while (window.isOpen())
	{
		input.reset();
		while (const std::optional event = window.pollEvent()){
			if (event->is<sf::Event::Closed>()){window.close();}
			input.read(event);
		}

		Debugging(player,walls,camera,dt,window,input,new_walls);

		dt=delta_clock.getElapsedTime()/delta_time;
		delta_clock.restart();
		if (dt>5){dt=5;}
		player.sideways_movement(dt);
		player.jumpIfPossible();
		player.gravity(dt);
		player.sprite.move({player.velocity.x*dt,0.f});
		if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){player.wall_collision_x(walls,dt);}
		player.sprite.move({0.f,player.velocity.y*dt});
		if (!DEBUGGING_LEVELS_AND_BUILDING_THEM){player.wall_collision_y(walls,dt);}
		

		camera.follow_player(player);
		window.clear();
		window.setView(camera.view);
		draw_walls(walls,window);
		draw_player(player,window);
		window.display();
		//sf::sleep(sf::milliseconds(200));
	}
}
