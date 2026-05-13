#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include <optional>
#include <map>

bool FLY_MODE=false;
bool PLACING_BLOCKS=false;
bool CHEAT_MODE=false;
bool VSYNC_TOGGLE=true;
bool SHOW_FPS=true;

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
	bool Enter;
	bool Multiply;
	bool Tab;

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
			if (key->code == sf::Keyboard::Key::Enter){Enter=true;}
			if (key->code == sf::Keyboard::Key::Multiply){Multiply=true;}
			if (key->code == sf::Keyboard::Key::Tab){Tab=true;}
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
		F=false;
		F1=false;
		F2=false;
		R=false;
		PageUp=false;
		Enter=false;
		Multiply=false;
		Tab=false;
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
	sf::Texture Button0_texture;
	sf::Texture Button1_texture;

	sf::Texture Bouncy_texture;
	sf::Texture JumpPad_texture;
	sf::Texture Spike_texture;
	sf::Texture BlueWall_texture;
	sf::Texture RedWall_texture;
	void LoadAllTextures(){
		if (!font.openFromFile("../../assets/fonts/arial.ttf")){}
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
		if (!Button0_texture.loadFromFile("../../assets/textures/Button0.png")){}
		if (!Button1_texture.loadFromFile("../../assets/textures/Button1.png")){}
		if (!Bouncy_texture.loadFromFile("../../assets/textures/Bouncy.png")){}
		if (!Spike_texture.loadFromFile("../../assets/textures/Spike.png")){}
		if (!JumpPad_texture.loadFromFile("../../assets/textures/JumpPad.png")){}
		if (!BlueWall_texture.loadFromFile("../../assets/textures/BlueWall.png")){}
		if (!RedWall_texture.loadFromFile("../../assets/textures/RedWall.png")){}
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
struct Clocks;
struct Button;
struct Settings;

struct Button{
	sf::Sprite sprite{global_assets.Button0_texture};
	bool state=false;
	sf::Text text{global_assets.font};

	void setup(std::string text_string, sf::Vector2f coords, int char_size){
		text.setString(text_string);
		sprite.setPosition(coords);
		text.setCharacterSize(char_size);
		text.setPosition(coords+sf::Vector2f{70.f,64.f});
	}

	void draw(TheWholeLevel& the_whole_level);

	void update(TheWholeLevel& the_whole_level, bool& bool_to_change); 

};

struct Settings{
	sf::Sprite sprite{global_assets.Button0_texture};
	Button button1;
	Button button2;
	Button button3;
	Button button4;
	void setup(){
			button1.state=true;
		button1.setup("Vsync",{0,0},50);	

			button2.state=false;
		button2.setup("CHEATS",{320,0},40);

			button3.state=true;
		button3.setup("SHOW FPS",{640,0},33);

			button4.state=false;
		button4.setup("EDITOR",{960,0},45);

	}

	void update(TheWholeLevel& the_whole_level);

	void draw(TheWholeLevel& the_whole_level);
};

struct Clocks{
	bool time_analysis=true;
	sf::Clock input_clock;
	sf::Clock physics_clock;
	sf::Clock walls_draw_clock;
	sf::Clock draw_clock;
	sf::Clock display_clock;
	sf::Clock FPS_clock;
	int framecounter=0;
	float total_update_time=0;
	float cur_fps=0;
	sf::Text text{global_assets.font};

	void update(TheWholeLevel& the_whole_level);
	void draw_fps(TheWholeLevel& the_whole_level);
	
};

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
	sf::Vector2f block_cords;
	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		block_cords=(setup_coords/64.f);
	}
};

struct Wall{
	sf::Vector2f size=sf::Vector2f(global_assets.wall_texture.getSize());
	sf::Sprite sprite{global_assets.wall_texture};
	std::string type="wall"; //wall    bouncy    spike   redwall   bluewall    jumppad
	bool visible=true;
	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		if (type=="wall"){sprite.setTexture(global_assets.wall_texture);}
		if (type=="bouncy"){sprite.setTexture(global_assets.Bouncy_texture);}
		if (type=="spike"){sprite.setTexture(global_assets.Spike_texture);}
		if (type=="jumppad"){sprite.setTexture(global_assets.JumpPad_texture);}
		if (type=="bluewall"){sprite.setTexture(global_assets.BlueWall_texture);}
		if (type=="redwall"){sprite.setTexture(global_assets.RedWall_texture);}
	}
};

struct Checkpoint{
	sf::Vector2f size=sf::Vector2f(global_assets.checkpoint_texture1.getSize());	
	sf::Sprite sprite{global_assets.checkpoint_texture1};
	bool activated=false;
	sf::Vector2f block_position;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords*64.f);
		block_position=setup_coords;
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
	sf::Vector2f starting_position={0.f,0.f};
	bool is_touching_down=false;
	bool is_touching_up=false;
	bool is_touching_left=false;
	bool is_touching_right=false;
	float checkpoint_countdown=50;
	bool die=false;
	int current_level=1;
	int max_level=10;
	int cur_max_unlocked_level=1;
	int checkpoint_number=0;
	std::string gamestate="menu"; //playing   escape  win   menu   level_selector     settings
	Finish finish;
	bool able_to_jump=true;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		starting_position=(setup_coords/64.f);
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
				if (!FLY_MODE && able_to_jump){velocity.y-=15;}
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
	sf::Text text{global_assets.font};
	void setup(){
		text.setCharacterSize(50);
		text.setFillColor(sf::Color(255,255,255,255));
		text.setPosition(coords+sf::Vector2f{50.f,0.f});
	}

	void checkmouse(TheWholeLevel& the_whole_level);
	void draw(TheWholeLevel& the_whole_level);
};

struct TheWholeLevel{
	sf::RenderWindow window;
	std::string editor_block_type="wall";
	Input input;
	Player player;
	std::map <std::pair<int,int>,Wall> walls_map;
	
	std::vector<Checkpoint> checkpoints;
	Settings settings;
	Clocks clocks;
	Camera camera;
	WinScreen winscreen;
	Sky sky;
	Menu menu;
	float dt=0;
	sf::Text editor_blocks_text{global_assets.font};

	void setup(){
		window.create(sf::VideoMode({1920, 1080}), "SFML works!", sf::State::Fullscreen);
		camera.setup();
		player.current_level=1;
		settings.setup();
		editor_blocks_text.setCharacterSize(50);
		editor_blocks_text.setPosition({500,0});
		winscreen.setup();
	}
		//wall    bouncy    spike    redwall   bluewall    jumppad
	void next_type(){
		if (editor_block_type=="wall"){editor_block_type="bouncy";}
		 else{
			if (editor_block_type=="bouncy"){editor_block_type="spike";}
			 else {
				if (editor_block_type=="spike"){editor_block_type="redwall";}
				else{
					if (editor_block_type=="redwall"){editor_block_type="bluewall";} 
					else{
						if (editor_block_type=="bluewall"){editor_block_type="jumppad";} 
						else{
							if (editor_block_type=="jumppad"){editor_block_type="finish";} 
							else{
								if (editor_block_type=="finish"){editor_block_type="checkpoint";} 
								else{
									if (editor_block_type=="checkpoint"){editor_block_type="wall";} 
								} 
							} 
						} 
					} 	
				}
			 }
			
		}
	}	
	
};

void LoadLevel(TheWholeLevel& the_whole_level){
	std::string level_load_string="../../assets/levels/";
	level_load_string+=(std::to_string(the_whole_level.player.current_level));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	the_whole_level.walls_map.clear();
	the_whole_level.checkpoints.clear();
	float x,y,l;
	static Wall wall;

	while (file>>type){
		wall.type="wall";
		if (type=="player"){
			file>>x>>y;
			the_whole_level.player.setup({x*64,y*64});
		}
		if (type=="wall"){
			file>>x>>y;
			wall.type="wall";
			wall.setup({x*64,y*64});			
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="bouncy"){
			file>>x>>y;
			wall.type="bouncy";
			wall.setup({x*64,y*64});
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="spike"){
			file>>x>>y;
			wall.type="spike";
			wall.setup({x*64,y*64});
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="jumppad"){
			file>>x>>y;
			wall.type="jumppad";
			wall.setup({x*64,y*64});
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="redwall"){
			file>>x>>y;
			wall.type="redwall";
			wall.setup({x*64,y*64});
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="bluewall"){
			file>>x>>y;
			wall.type="bluewall";
			wall.setup({x*64,y*64});
			the_whole_level.walls_map[{int(x),int(y)}]=wall;
		}
		if (type=="wallline"){
			int plus=1;
			file>>x>>y>>l>>type;
			if (l<0){l=-l;plus=-1;}
			if (type=="hori"){
				for (int i=0;i<l;i++){
				wall.setup({(x+i*plus)*64,y*64});
				the_whole_level.walls_map[{int(x+i*plus),int(y)}]=wall;
				}
			} else {
				for (int i=0;i<l;i++){
				wall.setup({x*64,(y+i*plus)*64});
				the_whole_level.walls_map[{int(x),int(y+i*plus)}]=wall;
				}
			}
		}
		if (type=="checkpoint"){
			file>>x>>y;
			Checkpoint checkpoint;
			checkpoint.setup({x,y});
			the_whole_level.checkpoints.push_back(checkpoint);
		}
		if (type=="finish"){
			file>>x>>y;
			the_whole_level.player.finish.setup({x*64,y*64});
		}
	}
}

void update_level_file(TheWholeLevel& the_whole_level){
	std::string level_write_string="../../assets/levels/";
	level_write_string+=(std::to_string(the_whole_level.player.current_level));
	level_write_string+=(".txt");
	std::ofstream level_file(level_write_string);
	if (level_file.is_open()){
		level_file<<"finish "<<the_whole_level.player.finish.block_cords.x<<" "<<the_whole_level.player.finish.block_cords.y<<"\n";
		level_file<<"\n";
		level_file<<"player "<<the_whole_level.player.starting_position.x<<" "<<the_whole_level.player.starting_position.y<<"\n";
		level_file<<"\n";
		for (auto& checkpoint:the_whole_level.checkpoints){
			level_file<<"checkpoint "<<checkpoint.block_position.x<<" "<<checkpoint.block_position.y<<"\n";
		}
		level_file<<"\n";
		for (auto& wall:the_whole_level.walls_map){
			level_file<<wall.second.type<<" "<<wall.first.first<<" "<<wall.first.second<<"\n";
		}

	} else{
		std::cout<<"write_file_error\n";
	}
}

void mouse_block_placing(TheWholeLevel& the_whole_level){
	if (the_whole_level.input.Tab){the_whole_level.next_type();}
	the_whole_level.window.setView(the_whole_level.camera.view);
	sf::Vector2i mouse_coords;
	mouse_coords=sf::Mouse::getPosition(the_whole_level.window);
	sf::Vector2f mouse_true_coords=the_whole_level.window.mapPixelToCoords(mouse_coords);
	int cordx=int(floor(mouse_true_coords.x/64.f));
	int cordy=int(floor(mouse_true_coords.y/64.f));
	if (the_whole_level.input.LShift){
		the_whole_level.player.sprite.setPosition({cordx*64.f,cordy*64.f});
		std::cout<<cordx<<" "<<cordy<<'\n';
	}
	if (the_whole_level.input.Mouse1){
		if (the_whole_level.editor_block_type!="finish"){
			if (the_whole_level.editor_block_type!="player"){
				if (the_whole_level.editor_block_type!="checkpoint"){
					if (!(the_whole_level.walls_map.count({cordx,cordy})>0)){
						Wall wall;
						wall.type=the_whole_level.editor_block_type;
						wall.setup({cordx*64.f,cordy*64.f});
						the_whole_level.walls_map[{cordx,cordy}]=wall;
					}
				} else{
					Checkpoint checkpoint;
					checkpoint.setup({float(cordx),float(cordy)});
					the_whole_level.checkpoints.push_back(checkpoint);
				}
			} else{
				the_whole_level.player.setup({cordx*64.f,cordy*64.f});
			}
		} else {
			the_whole_level.player.finish.setup({cordx*64.f,cordy*64.f});
		}
		
	}
	if (the_whole_level.input.Mouse2){
		the_whole_level.walls_map.erase({cordx,cordy});
		for (int i=0;i<the_whole_level.checkpoints.size();i++){
			if (the_whole_level.checkpoints[i].block_position==sf::Vector2f({float(cordx),float(cordy)})){
				the_whole_level.checkpoints.erase(the_whole_level.checkpoints.begin()+i);
			}
		}
	}

	if (the_whole_level.input.F9){
		std::cout<<"start"<<'\n';
		for (auto& wall:the_whole_level.walls_map){
			std::cout<<wall.second.type<<" "<<wall.first.first<<" "<<wall.first.second<<'\n';
		}
		std::cout<<"end"<<'\n';
		update_level_file(the_whole_level);
	}	
	the_whole_level.editor_blocks_text.setString(the_whole_level.editor_block_type);
}


void Debugging(TheWholeLevel& the_whole_level){
	if (the_whole_level.input.F1){FLY_MODE=true;}
	if (the_whole_level.input.F2){FLY_MODE=false;}
	if (the_whole_level.input.PageUp){LoadLevel(the_whole_level);the_whole_level.camera.setup();}
	if (FLY_MODE){
		the_whole_level.player.debug_movement(the_whole_level);
	}
	if (PLACING_BLOCKS){mouse_block_placing(the_whole_level);}
}

void draw_player(TheWholeLevel& the_whole_level){
	the_whole_level.window.draw(the_whole_level.player.sprite);
	the_whole_level.window.draw(the_whole_level.player.finish.sprite);
}

void draw_walls(TheWholeLevel& the_whole_level){
		sf::Vector2f view_topleft=the_whole_level.camera.view.getCenter()-the_whole_level.camera.view.getSize()/2.f;
		int x=int(view_topleft.x/64.f);
		int y=int(view_topleft.y/64.f);
		for (int i=x-1;i<31+x;i++){
			for (int g=y-1;g<18+y;g++){
				if (the_whole_level.walls_map.count({i,g})>0){
					if (the_whole_level.walls_map[{i,g}].visible){
						the_whole_level.window.draw(the_whole_level.walls_map[{i,g}].sprite);
					}
				}
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

void fly_draw(TheWholeLevel& the_whole_level){
	the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
	static sf::Text text(global_assets.font);
	text.setString("FLY:  ON");
	text.setPosition({0,0});
	if (FLY_MODE){
		the_whole_level.window.draw(text);
	}
}

int main()
{
	global_assets.LoadAllTextures();
	TheWholeLevel the_whole_level;
	the_whole_level.setup();
	//the_whole_level.window.setVerticalSyncEnabled(true);
	//the_whole_level.window.setFramerateLimit(1);
	the_whole_level.clocks.time_analysis=true;
	the_whole_level.clocks.FPS_clock.restart();

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	LoadLevel(the_whole_level);

	while (the_whole_level.window.isOpen())
	{

			the_whole_level.clocks.input_clock.start();
		the_whole_level.input.reset();
		while (const std::optional event = the_whole_level.window.pollEvent()){
			if (event->is<sf::Event::Closed>()){the_whole_level.window.close();}
			the_whole_level.input.read(event);
		}
			the_whole_level.clocks.input_clock.stop();



			the_whole_level.clocks.physics_clock.start();
		the_whole_level.dt=delta_clock.getElapsedTime()/delta_time;
		delta_clock.restart();
		if (the_whole_level.dt>1){the_whole_level.dt=1;}


		
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
			the_whole_level.player.able_to_jump=true;
			the_whole_level.player.die=false;
			the_whole_level.player.sprite.move({the_whole_level.player.velocity.x*the_whole_level.dt,0.f});
			if (!FLY_MODE){the_whole_level.player.wall_collision_x(the_whole_level);}
			the_whole_level.player.sprite.move({0.f,the_whole_level.player.velocity.y*the_whole_level.dt});
			if (!FLY_MODE){the_whole_level.player.wall_collision_y(the_whole_level);}
			the_whole_level.camera.follow_player(the_whole_level);
		}
	//settings state
		if (the_whole_level.player.gamestate=="settings"){
			the_whole_level.settings.update(the_whole_level);
		}
		the_whole_level.window.setVerticalSyncEnabled(VSYNC_TOGGLE);



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


	//check escape
		if (the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.winscreen.checkmouse(the_whole_level);
		}
			the_whole_level.clocks.physics_clock.stop();

			the_whole_level.clocks.draw_clock.start();



	//DRAWING
		the_whole_level.window.clear();
	//inside of the level
		if (the_whole_level.player.gamestate=="playing" || the_whole_level.player.gamestate=="win" || 
			the_whole_level.player.gamestate=="escape" || the_whole_level.player.gamestate=="settings"){
				the_whole_level.sky.draw(the_whole_level);
			}
		if (the_whole_level.player.gamestate=="playing" || the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.window.setView(the_whole_level.camera.view);
				the_whole_level.clocks.draw_clock.stop();

				the_whole_level.clocks.walls_draw_clock.start();
			draw_walls(the_whole_level);
				the_whole_level.clocks.walls_draw_clock.stop();

				the_whole_level.clocks.draw_clock.start();
			draw_player(the_whole_level);

			draw_checkpoints(the_whole_level);	
		}
	//escape screen
		if (the_whole_level.player.gamestate=="win" || the_whole_level.player.gamestate=="escape"){
			the_whole_level.winscreen.draw(the_whole_level);
		}
	//settings screen
		if (the_whole_level.player.gamestate=="settings"){
			the_whole_level.settings.draw(the_whole_level);
		}
	//menu screen
		if (the_whole_level.player.gamestate=="menu"){
			the_whole_level.menu.draw(the_whole_level);
		}
		if (SHOW_FPS){the_whole_level.clocks.draw_fps(the_whole_level);}
			the_whole_level.clocks.draw_clock.stop();
	//editor
		if (PLACING_BLOCKS){
			the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
			the_whole_level.window.draw(the_whole_level.editor_blocks_text);
		}
		//display
			the_whole_level.clocks.display_clock.start();
			fly_draw(the_whole_level);
		the_whole_level.window.display();
			the_whole_level.clocks.display_clock.stop();



	//PERFORMANCE CHECK
		the_whole_level.clocks.update(the_whole_level);
	}
}






//FUNCTIONS



void Clocks::update(TheWholeLevel& the_whole_level){
	if (time_analysis && the_whole_level.input.R && CHEAT_MODE){
		std::cout<<"input: "<<input_clock.getElapsedTime().asMilliseconds()<<'\n';
		std::cout<<"physics: "<<physics_clock.getElapsedTime().asMilliseconds()<<'\n';
		std::cout<<"draw: "<<draw_clock.getElapsedTime().asMilliseconds()<<'\n';
		std::cout<<"display: "<<display_clock.getElapsedTime().asMilliseconds()<<'\n';
		std::cout<<"walls_draw: "<<walls_draw_clock.getElapsedTime().asMilliseconds()<<'\n'<<'\n';
		total_update_time=0;
		total_update_time+=input_clock.getElapsedTime().asMilliseconds();
		total_update_time+=physics_clock.getElapsedTime().asMilliseconds();
		total_update_time+=draw_clock.getElapsedTime().asMilliseconds();
		total_update_time+=display_clock.getElapsedTime().asMilliseconds();
		total_update_time+=walls_draw_clock.getElapsedTime().asMilliseconds();
		std::cout<<"input: "<<input_clock.getElapsedTime().asMilliseconds()*100.f/total_update_time<<"%"<<'\n';
		std::cout<<"physics: "<<physics_clock.getElapsedTime().asMilliseconds()*100/total_update_time<<"%"<<'\n';
		std::cout<<"draw: "<<draw_clock.getElapsedTime().asMilliseconds()*100/total_update_time<<"%"<<'\n';
		std::cout<<"display: "<<display_clock.getElapsedTime().asMilliseconds()*100/total_update_time<<"%"<<'\n';
		std::cout<<"walls_draw: "<<walls_draw_clock.getElapsedTime().asMilliseconds()*100/total_update_time<<"%"<<'\n'<<'\n';
		input_clock.restart();physics_clock.restart();draw_clock.restart();display_clock.restart();walls_draw_clock.restart();
	}
	framecounter++;
	if (FPS_clock.getElapsedTime().asSeconds()>=1){
		cur_fps=float(framecounter)/FPS_clock.getElapsedTime().asSeconds();
		FPS_clock.restart();
		framecounter=0;
	}
}

void Clocks::draw_fps(TheWholeLevel& the_whole_level){
	the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		std::string fps_string="FPS: ";
		fps_string+=std::to_string(int(cur_fps));
		text.setPosition({1650,0.f});
		text.setCharacterSize(50);
		text.setString(fps_string);
		the_whole_level.window.draw(text);
	the_whole_level.window.setView(the_whole_level.camera.view);
}

void WinScreen::checkmouse(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		sf::FloatRect button1({coords.x+24*scale,coords.y+24*scale},{79*scale,15*scale});
		sf::FloatRect button2({coords.x+24*scale,coords.y+48*scale},{79*scale,15*scale});
		sf::FloatRect button3({coords.x+24*scale,coords.y+72*scale},{79*scale,15*scale});
		sf::Vector2i mouse_coords;
		mouse_coords=sf::Mouse::getPosition(the_whole_level.window);
		sf::Vector2f mouse_true_coords=the_whole_level.window.mapPixelToCoords(mouse_coords);
		if (button1.contains(mouse_true_coords)){
			sprite.setTexture(global_assets.Win1_texture);
			if (the_whole_level.input.Mouse1){
				the_whole_level.player.current_level++;
				if (the_whole_level.player.current_level>the_whole_level.player.max_level){the_whole_level.player.current_level=1;}
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
		text.setString("level: "+std::to_string(the_whole_level.player.current_level));
		the_whole_level.window.draw(text);

	}

		//wall    bouncy    spike    redwall   bluewall    jumppad
	void Player::wall_collision_x(TheWholeLevel& the_whole_level){
		is_touching_right=false;
		is_touching_left=false;
		is_touching_up=false;
		is_touching_down=false;
		sf::FloatRect player_rect{sprite.getGlobalBounds()};
		int xcur=int((sprite.getPosition().x/64.f));
		int ycur=int((sprite.getPosition().y/64.f));
		for (int xadd=-1;xadd<=1;xadd++){
			for (int yadd=-1;yadd<=1;yadd++){
				if (the_whole_level.walls_map.count({xcur+xadd,ycur+yadd})>0){
					if (the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].visible){
						std::string curtype=the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].type;
						if (player_rect.findIntersection(the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getGlobalBounds())){
							if (velocity.x>0){
								is_touching_right=true;
								is_touching_left=false;
								sprite.move({the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getPosition().x-sprite.getPosition().x-size.x,0.f});
							} else{
								is_touching_left=true;
								is_touching_right=false;
								sprite.move({the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getPosition().x-sprite.getPosition().x+size.x,0.f});
							}	
								if (curtype=="bouncy"){
								if (abs(velocity.x)>1){velocity.x*=-1;} else {velocity.x=0;}
							}
							if (curtype=="wall"){velocity.x=0;}
							if (curtype=="jumppad"){velocity.y=-33;}
							if (curtype=="spike"){}
							return;
						}
					}
				}
			}
		}
	}

	void Player::wall_collision_y(TheWholeLevel& the_whole_level){
		sf::FloatRect player_rect{sprite.getGlobalBounds()};
		int xcur=int((sprite.getPosition().x/64.f));
		int ycur=int((sprite.getPosition().y/64.f));
		for (int xadd=-1;xadd<=1;xadd++){
			for (int yadd=-1;yadd<=1;yadd++){
				if (the_whole_level.walls_map.count({xcur+xadd,ycur+yadd})>0){
					if (the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].visible){
						std::string curtype=the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].type;
						if (player_rect.findIntersection(the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getGlobalBounds())){
							if (velocity.y>0){
								is_touching_down=true;
								is_touching_up=false;
								sprite.move({0.f,the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getPosition().y-sprite.getPosition().y-size.y});
							} else{
								is_touching_up=true;
								is_touching_down=false;
								sprite.move({0.f,the_whole_level.walls_map[{xcur+xadd,ycur+yadd}].sprite.getPosition().y-sprite.getPosition().y+size.y});
							}	
							//velocity.y=-velocity.y;
							if (curtype=="bouncy"){
								if (abs(velocity.y)>1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
									velocity.y*=-1.f;able_to_jump=false;
								} else {velocity.y=0;}
							}
							if (curtype=="wall"){velocity.y=0;}
							if (curtype=="jumppad"){velocity.y=-33;able_to_jump=false;}
							if (curtype=="spike"){die=true;velocity.y=0;}
							return;
						}
					}
				}
			}
		}
	}

	void Player::gravity(TheWholeLevel& the_whole_level){
		if (!is_touching_down & !FLY_MODE){
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
		if (checkpoint_countdown<=0 || the_whole_level.player.die){
			checkpoint_countdown=50;
			sprite.setPosition(current_checkpoint);
			velocity={0.f,0.f};
		}
	}

	void Button::draw(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
		the_whole_level.window.draw(sprite);
		the_whole_level.window.draw(text);
		the_whole_level.window.setView(the_whole_level.camera.view);
	}

	void Button::update(TheWholeLevel& the_whole_level, bool& bool_to_change){
		sf::Vector2f mcords=the_whole_level.window.mapPixelToCoords(sf::Mouse::getPosition(the_whole_level.window));
		if (sprite.getGlobalBounds().contains(mcords)){
			text.setFillColor(sf::Color(0,0,255,255));
			if (the_whole_level.input.Mouse1){bool_to_change=!bool_to_change;}
		} else {text.setFillColor(sf::Color(255,255,255,255));}
		if (bool_to_change){
			sprite.setTexture(global_assets.Button1_texture);
		} else {sprite.setTexture(global_assets.Button0_texture);}
	}

	void Settings::update(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
			button1.update(the_whole_level,VSYNC_TOGGLE);
			button2.update(the_whole_level,CHEAT_MODE);
			button3.update(the_whole_level,SHOW_FPS);
			button4.update(the_whole_level,PLACING_BLOCKS);


		if (the_whole_level.player.gamestate=="settings" && the_whole_level.input.Escape && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
			the_whole_level.player.gamestate="menu";
		}
	}

	void Settings::draw(TheWholeLevel& the_whole_level){
			button1.draw(the_whole_level);
			button2.draw(the_whole_level);
			button3.draw(the_whole_level);
			button4.draw(the_whole_level);
	}

	void Camera::follow_player(TheWholeLevel& the_whole_level){
		static float boundry_x=100;
		static float boundry_y=200;
		boundry_x=50;boundry_y=50;
		static float boundry_speed=0.1;
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
		sprite.setPosition({0,0});
		sprite.setScale({scalex,scaley});
		the_whole_level.window.draw(sprite);
		the_whole_level.window.setView(the_whole_level.camera.view);
	}

	void Menu::checkmouse(TheWholeLevel& the_whole_level){
		the_whole_level.window.setView(sf::View(sf::FloatRect({0,0},{1920,1080})));
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