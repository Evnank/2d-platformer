#include <SFML/Graphics.hpp>
#include <istream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

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
	sf::Vector2f velocity={0.f,-10.f};
	sf::Vector2f checkpoint={0.f,0.f};
	sf::Sprite sprite{global_assets.player_texture};
	bool is_touching_down=false;
	bool is_touching_up=false;
	bool is_touching_left=false;
	bool is_touching_right=false;

	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		checkpoint=setup_coords;
	}
	
	void wall_collision_x(std::vector<Wall>& walls){
		for (int i=0;i<walls.size();i++){
			if (sprite.getGlobalBounds().findIntersection(walls[i].sprite.getGlobalBounds())){
				if (velocity.x>0){
					is_touching_right=true;is_touching_left=false;
					
					sprite.setPosition({sprite.getPosition().x-velocity.x,sprite.getPosition().y});
				} else{
					is_touching_left=true;is_touching_right=false;
					sprite.setPosition({sprite.getPosition().x-velocity.x,sprite.getPosition().y});
				}	
				velocity.x=-velocity.x;
				//velocity.x=0;
			}
		}
	}
	void wall_collision_y(std::vector<Wall>& walls){
		for (int i=0;i<walls.size();i++){
			if (sprite.getGlobalBounds().findIntersection(walls[i].sprite.getGlobalBounds())){
				if (velocity.y>0){
					is_touching_down=true;is_touching_up=false;
					
					sprite.setPosition({sprite.getPosition().x,sprite.getPosition().y-velocity.y});
				} else{
					is_touching_up=true;is_touching_down=false;
					sprite.setPosition({sprite.getPosition().x,sprite.getPosition().y-velocity.y});
				}	
				velocity.y=-velocity.y;
				//velocity.y=0;
			}
		}
	}

};

void LoadLevel(Player& player,std::vector<Wall>& walls, int level_number){
	std::string level_load_string="assets/levels/";
	level_load_string+=(std::to_string(level_number));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	float x,y;
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
	Player player;
	std::vector<Wall> walls;

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	float dt;
	LoadLevel(player,walls,1);
	while (window.isOpen())
	{

		while (const std::optional event = window.pollEvent()){
			if (event->is<sf::Event::Closed>()){window.close();}		
		}

		dt=delta_clock.getElapsedTime()/delta_time;
		delta_clock.restart();
		if (dt>5){dt=5;}
		player.sprite.move({player.velocity.x*dt,0.f});
		player.wall_collision_x(walls);
		player.sprite.move({0.f,player.velocity.y*dt});
		player.wall_collision_y(walls);
		

		window.clear();
		draw_walls(walls,window);
		draw_player(player,window);
		window.display();
		//sf::sleep(sf::milliseconds(100));
	}
}
