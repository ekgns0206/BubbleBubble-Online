#include "Map.h"

Map::Map(const std::string& filename)
{
	this->load(filename);
}

Map::~Map()
{
	if(this->_mapTiles == NULL)
		return;

	for (int row = 0; row < this->_row; row++)
	{
		for (int col = 0; col < this->_col; col++)
		{
			if(this->_mapTiles[row][col] != NULL)
				delete this->_mapTiles[row][col];
		}
		delete[] this->_mapTiles[row];
	}
	delete[] this->_mapTiles;

	for(auto i = this->_enemies.begin(); i != this->_enemies.end(); i++)
		delete *i;
	this->_enemies.clear();
}

//
// load
//	map ������ �о���δ�.
//
// Parameters
//	map ���� ���
//
// Return
//	������ true, ���н� false
//
bool Map::load(const std::string & filename)
{
	try
	{
		std::ifstream				file(filename, std::ios::binary);
		if(file.is_open() == false)
			return false;

		char		bgmNameLength;
		file.read(&bgmNameLength, sizeof(char));
		if (bgmNameLength != 0)
		{
			char*	bgmName = new char[bgmNameLength + 1];
			file.read(bgmName, bgmNameLength);
			bgmName[bgmNameLength] = 0;

			this->_bgmName = bgmName;
			delete[] bgmName;
		}


		// �࿭�� ������ Ÿ���� ũ�⸦ ��´�.
		file.read((char*)&this->_row, sizeof(int));
		file.read((char*)&this->_col, sizeof(int));
		file.read((char*)&this->_tileWidth, sizeof(int));
		file.read((char*)&this->_tileHeight, sizeof(int));


		this->_mapTiles					= new Tile**[this->_row];		// �ุŭ �Ҵ�
		for (int row = 0; row < this->_row; row++)
		{
			this->_mapTiles[row]		= new Tile*[this->_col];			// ����ŭ �Ҵ�
			for (int col = 0; col < this->_col; col++)
			{
				char used;
				file.read(&used, sizeof(char));

				if(used == false)
				{
					this->_mapTiles[row][col]	= NULL;
				}
				else
				{
					char id;
					file.read(&id, sizeof(char));

					char flags;
					file.read(&flags, sizeof(char));
					this->_mapTiles[row][col] = new Tile(id, flags);
				}
			}
		}

		int respawnPointCount = 0;
		file.read((char*)&respawnPointCount, sizeof(int));
		for(int i = 0; i < respawnPointCount; i++)
		{
			int index = 0;
			file.read((char*)&index, sizeof(int));

			file.read((char*)&this->_respawnPoint[index].x, sizeof(int));
			file.read((char*)&this->_respawnPoint[index].y, sizeof(int));
		}

		int enemyCount = 0;
		file.read((char*)&enemyCount, sizeof(int));
		for(int i = 0; i < enemyCount; i++)
		{
			char enemyNameLength = 0;
			file.read(&enemyNameLength, sizeof(char));

			char* enemyName = new char[enemyNameLength + 1];
			file.read(enemyName, enemyNameLength);
			enemyName[enemyNameLength] = 0;

			int x, y;
			file.read((char*)&x, sizeof(int));
			file.read((char*)&y, sizeof(int));
			this->_enemies.push_back(new Enemy(-1, enemyName, point(x, y)));

			delete[] enemyName;
		}
	}
	catch(std::exception&)
	{}
}

const std::string& Map::bgm() const
{
	return this->_bgmName;
}

//
// row
//	���� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ���� ����
//
int Map::row() const
{
	return this->_row;
}

//
// col
//	���� ��´�.
//
// Parameters
//	����
//
// Return
//	���� ���� ����
//
int Map::col() const
{
	return this->_col;
}

//
// tileWidth
//	Ÿ���� ���̸� ���Ѵ�.
//
// Parameters
//	����
//
// Return
//	Ÿ���� ���̸� ����
//
int Map::tileWidth() const
{
	return this->_tileWidth;
}

//
// tileHeight
//	Ÿ���� ���̸� ���Ѵ�.
//
// Parameters
//	����
//
// Return
//	Ÿ���� ���̸� ����
//
int Map::tileHeight() const
{
	return this->_tileHeight;
}

//
// get
//	�ش� ��ġ�� Ÿ���� ��������Ʈ �̸��� ��´�.
//
// Parameters
//	row		��
//	col		��
//
// Return
//	������ ��������Ʈ �̸�, ���н� NULL�� ����
//
const Map::Tile* Map::get(int row, int col) const
{
	return this->_mapTiles[row][col];
}

const point & Map::respawnPoint(int index) const
{
	// TODO: insert return statement here
	return this->_respawnPoint[index];
}

const std::vector<Enemy*>& Map::enemies() const
{
	return this->_enemies;
}

const rect_f Map::collisionRect(int row, int col)
{
	return rect_f(col * this->_tileWidth, row * this->_tileHeight, this->_tileWidth, this->_tileHeight);
}

const Map::Tile ** Map::operator[](int index) const
{
	return const_cast<const Map::Tile**>(this->_mapTiles[index]);
}

Map::Tile::Tile()
{
	this->_id = 0;
	this->_flags = 0;
}

Map::Tile::Tile(int id, int flags)
{
	this->_id = id;
	this->_flags = flags;
}

Map::Tile::~Tile()
{
}

int Map::Tile::id() const
{
	return this->_id;
}

int Map::Tile::flag() const
{
	return this->_flags;
}
