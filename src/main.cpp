#include <SFML/Graphics.hpp>
#include <istream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

struct Assets{
	sf::Font font;
	sf::Texture player_texture1;
	sf::Texture player_texture2;
	sf::Texture wall_texture;
	sf::Texture finish_texture;
	sf::Texture checkpoint_texture;
	sf::Texture sky_texture;
	void LoadAllTextures(){
	//	if (!font.openFromFile("assets/fonts/arial.ttf")){}
		if (!player_texture1.loadFromFile("assets/textures/PlayerTexture1.png")){}
		if (!player_texture2.loadFromFile("assets/textures/PlayerTexture1.png")){}
		if (!wall_texture.loadFromFile("assets/textures/WallTexture.png")){}
	}
};

Assets global_assets;

struct Player{
	sf::Vector2f size={50.f,50.f};	
	sf::Vector2f velocity={0.f,0.f};
	sf::Vector2f checkpoint={0.f,0.f};
	//sf::Sprite sprite(player_texture1);
	sf::RectangleShape sprite{size};
	bool is_touching_down=false;
	bool is_touching_up=false;
	bool is_touching_left=false;
	bool is_touching_right=false;
	void move(){
		sprite.move(velocity);
	}
	void setup(sf::Vector2f setup_coords){
		sprite.setPosition(setup_coords);
		checkpoint=setup_coords;

		sprite.setFillColor(sf::Color::Blue);
		//sprite.setTexture(global_assets.wall_texture);
	}
};

struct Wall{
	sf::Vector2f size={50.f,50.f};	
	sf::RectangleShape sprite{size};
	//sf::Sprite sprite;
	void setup(sf::Vector2f setup_coords,sf::Vector2f setup_size){
		sprite.setPosition(setup_coords);
		size=setup_size;
		sprite.setSize(setup_size);
		sprite.setFillColor(sf::Color::Red);
		//sprite.setTexture(global_assets.wall_texture);
	}
};


void LoadLevel(Player& player,std::vector<Wall>& walls, int level_number){
	std::string level_load_string="assets/levels/";
	level_load_string+=(std::to_string(level_number));
	level_load_string+=(".txt");
	std::ifstream file(level_load_string);
	std::string type;
	float x,y,size1,size2;
	static Wall wall;
	while (file>>type){
		if (type=="player"){
			file>>x>>y;
			player.setup({x,y});
		}
		if (type=="wall"){
			file>>x>>y>>size1>>size2;
			wall.setup({x,y},{size1,size2});
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
	sf::RenderWindow window( sf::VideoMode( { 1920, 1080 } ), "SFML works!" );
	global_assets.LoadAllTextures();
	Player player1;
	std::vector<Wall> walls;

	sf::Clock delta_clock;
	sf::Time delta_time(sf::milliseconds(1000/60));
	float dt;
	LoadLevel(player1,walls,1);
	while ( window.isOpen() )
	{
		while (const std::optional event = window.pollEvent()){
			if (event->is<sf::Event::Closed>()){window.close();}		
		}
		dt=delta_clock.getElapsedTime()/delta_time;

		window.clear();
		draw_player(player1,window);
		draw_walls(walls,window);
		window.display();
	}
}
