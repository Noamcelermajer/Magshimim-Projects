#include "DatabaseAccess.h"
#include "user.h"
#include <vector>

static int get_picture_callback(void* data, int argc, char** argv, char** azColName)
{
	Picture* tmp = static_cast<Picture*>(data);
	std::string name;
	for (int i = 0; i < argc; i++) {
		switch (i)
		{
		case 1:
			tmp->setId(std::atoi(argv[i]));
			break;
		case 2:
			name = argv[i];
			//tmp->setName(name);
			break;
		case 3:
			tmp->setPath(argv[i]);
			break;
		case 4:
			tmp->setCreationDate(argv[i]);
			break;
		}
	}
	return 0;
}
static int get_int_callback(void* data, int argc, char** argv, char** azColName)
{
	int* pdata = (int*)data;
	*pdata = std::atoi(argv[0]);
	return 0;
}
static int get_user_callback(void* data, int argc, char** argv, char** azColName)
{
	User* tmp = static_cast<User*>(data);

	for (int i = 0; i < argc; i++) {
		switch (i)
		{
		case 0:
			tmp->setId(std::atoi(argv[i]));
			break;
		case 1:
			tmp->setName(argv[i]);
			break;
		}
	}

	std::cout << std::endl;
	return 0;
}
static int print_callback(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		std::cout << azColName[i] << ":  " << argv[i] ? argv[i] : "NULL";
		std::cout << std::endl;
	}

	std::cout << std::endl;
	return 0;
}
static int open_album_callback(void* data, int argc, char** argv, char** azColName)
{
	Album* palbum = static_cast<Album*>(data);
	for (int i = 0; i < argc; i++) {
		switch (i)
		{
		case 1:
			palbum->setName(argv[i]);
			break;
		case 2:
			palbum->setOwner(std::atoi(argv[i]));
			break;
		case 3:
			palbum->setCreationDate(argv[i]);
			break;
		}
	}
	return 0;
}
static int is_exist_callback(void* data, int argc, char** argv, char** azColName)
{
	bool* tmp = static_cast<bool*>(data);
	if (argc == 0)
	{
		*tmp = false;
		return 0;
	}
	*tmp = true;
	return 0;
}
static int get_album_callback(void* data, int argc, char** argv, char** azColName)
{

	std::list<Album>* pm_albums = static_cast<std::list<Album>*>(data);
	Album tmp = Album();
	for (int i = 0; i < argc; i++) {
		switch (i)
		{
		case 1:
			tmp.setName(argv[i]);
			break;
		case 2:
			tmp.setOwner(std::atoi(argv[i]));
			break;
		case 3:
			tmp.setCreationDate(argv[i]);
			break;
		}
	}
	pm_albums->push_back(tmp);
	return 0;
}
void DatabaseAccess::createAlbum(const Album& album)
{
	std::string sqlQuery = "INSERT INTO ALBUMS (NAME, USER_ID, CREATION_DATE) VALUES (";
	std::string name = '\"' + album.getName() + "\", ";
	std::string	ownerId = std::to_string(album.getOwnerId()) + ", ";
	std::string date = '\"' + album.getCreationDate() + "\"";
	sqlQuery = sqlQuery + name + ownerId + date + ");";
	std::cout << sqlQuery.c_str() << std::endl;
	char* messageError = nullptr;
	int exitCode = sqlite3_exec(db, sqlQuery.c_str(), nullptr, nullptr, &messageError);

}
bool DatabaseAccess::open()
{
	std::string dbFileName = "galleryDB.sqlite";
	int res = sqlite3_open(dbFileName.c_str(), &db);
	if (res != SQLITE_OK)
	{
		db = nullptr;
		throw "Failed to open DB";
		return false;
	}
	sqlite3_exec(db, "PRAGMA foreign_keys=ON", nullptr, nullptr, nullptr);
	return true;
}

void DatabaseAccess::close()
{
	sqlite3_close(db);
}
void DatabaseAccess::clear()
{
	db = nullptr;
}
const std::list<Album> DatabaseAccess::getAlbums() {
	std::string Query = "SELECT * FROM ALBUMS;";

	sqlite3_exec(db, Query.c_str(), get_album_callback, (void*)&m_albums, nullptr);
	return m_albums;
}
const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	std::string Query = "SELECT * FROM ALBUMS AND USER_ID=" + std::to_string(user.getId()) + ";";
	std::list<Album> user_albums;
	sqlite3_exec(db, Query.c_str(), get_album_callback, (void*)&user_albums, nullptr);
	return user_albums;
}
void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	std::string query = "DELETE FROM ALBUMS WHERE";
	query = query + "NAME=" + albumName + "AND" + "USER_ID=" + std::to_string(userId);
	char* errmsg = nullptr;
	sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errmsg);
}
bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId) {
	bool data;
	std::string Query = "SELECT * FROM ALBUMS WHERE USER_ID=" + std::to_string(userId) + " AND NAME=\""+ albumName+"\";";
	sqlite3_exec(db, Query.c_str(), is_exist_callback, (void*)&data, nullptr);
	return data;

}
Album DatabaseAccess::openAlbum(const std::string& albumName) {
	std::string Query = "SELECT " + albumName;
	Query = Query + "FROM ALBUMS;";
	Album* tmp = new Album();
	sqlite3_exec(db, Query.c_str(), open_album_callback, (void*)tmp, nullptr);
	Album returnAlbum = *tmp;
	delete tmp;
	return returnAlbum;
}
void DatabaseAccess::closeAlbum(Album &pAlbum)
{
	std::string query = "DELETE FROM ALBUMS WHERE NAME=" + pAlbum.getName() + " AND USER_ID=" + std::to_string(pAlbum.getOwnerId()) + ";";
	sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}
void DatabaseAccess::printAlbums()
{
	std::string Query = "SELECT * FROM ALBUMS;";
	sqlite3_exec(db, Query.c_str(), print_callback, nullptr, nullptr);
}

// picture related
void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	std::list<Album> albumLst = getAlbums();
	std::string sqlQuery = "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES (", name = "\"" + picture.getName() + "\", ", path = "\"" + picture.getPath() + "\", ", creationDate = "\"" + picture.getCreationDate() + "\", ", alId = std::to_string(getAlbumIdbyName(albumLst, albumName)) + ");";
	sqlQuery = sqlQuery + name + path + creationDate + alId;
	std::cout << sqlQuery.c_str() << std::endl;
	char* messageError = nullptr;
	int exitCode = sqlite3_exec(db, sqlQuery.c_str(), nullptr, nullptr, &messageError);
}
void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
	std::list<Album> albumLst = getAlbums();
	std::string query = "DELETE FROM PICTURES WHERE ALBUM_ID=" + std::to_string(getAlbumIdbyName(albumLst, albumName)) + " AND NAME=" + "\"" + pictureName + "\"" + ";";
	std::cout << query.c_str() << std::endl;
	char* messageError = nullptr;
	int exitCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &messageError);
}
void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	std::string query = "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES(" + std::to_string(getPictureIDbyName(pictureName)) + ", " + std::to_string(userId) + ");";
	int exitCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) 
{
	std::string query = "DELETE FROM TAGS WHERE PICTURE_ID=" + std::to_string(getPictureIDbyName(pictureName)) + " AND USER_ID=" + std::to_string(userId) + ";";
	int exitCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}

// user related
void DatabaseAccess::printUsers()
{
	std::string Query = "SELECT * FROM USERS;";
	sqlite3_exec(db, Query.c_str(), print_callback, nullptr, nullptr);
}
void DatabaseAccess::createUser(User& user)
{
	std::string query = "INSERT INTO USERS (ID, NAME) VALUES (";
	query = query + std::to_string(user.getId()) + ", \"" + user.getName() + "\");";
	std::cout << query.c_str() << std::endl;
	char* messageError = nullptr;
	int exitCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &messageError);
}
void DatabaseAccess::deleteUser(const User& user)
{
	std::string query = "DELETE FROM USERS WHERE NAME=";
	query = query + "\"" + user.getName() + "\"" + ";";
	std::cout << query.c_str() << std::endl;
	char* messageError = nullptr;
	int exitCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &messageError);
}
bool DatabaseAccess::doesUserExists(int userId) {
	bool data;
	std::string Query = "SELECT * FROM USERS WHERE ID="+std::to_string(userId)+';';
	sqlite3_exec(db, Query.c_str(), is_exist_callback, (void*)&data, nullptr);
	return data;
}
User DatabaseAccess::getUser(int userId)
{
	std::string Query = "SELECT " + std::to_string(userId) + " FROM USER;";
	User* user;
	sqlite3_exec(db, Query.c_str(), get_user_callback, (void*)&user, nullptr);
	User returnUser = *user;
	delete user;
	return returnUser;
}

// user statistics
int DatabaseAccess::countAlbumsOwnedOfUser(const User& user)
{
	int count;
	std::string query = "SELECT count(*) FROM ALBUMS WHERE USER_ID=" + std::to_string(user.getId()) + ";";
	sqlite3_exec(db, query.c_str(), get_int_callback, (void*)&count, nullptr);
	return count;
}
int DatabaseAccess::countAlbumsTaggedOfUser(const User& user) 
{
	int count;
	std::string query = "SELECT count(*) FROM TAGS WHERE USER_ID=" + std::to_string(user.getId()) + ";";
	sqlite3_exec(db, query.c_str(), get_int_callback, (void*)&count, nullptr);
	return count;
}
int DatabaseAccess::countTagsOfUser(const User& user) 
{
	int count;
	std::string query = "SELECT count(*) FROM TAGS WHERE USER_ID=" + std::to_string(user.getId()) + ";";
	sqlite3_exec(db, query.c_str(), get_int_callback, (void*)&count, nullptr);
	return count;
}
float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user) 
{
	std::list<Album> tmp;
	std::string query = "SELECT * FROM ALBUMS WHERE USER_ID=" + std::to_string(user.getId()) + ";";
	sqlite3_exec(db, query.c_str(), get_album_callback, (void*)&tmp, nullptr);
	return 2.11;
}


// queries
User DatabaseAccess::getTopTaggedUser() {
	//not implemented
	return User(123,"noam");
}
Picture DatabaseAccess::getTopTaggedPicture() {
	Picture * pic;
	int tmp;
	std::string query = "SELECT * FROM TAGS GROUP BY PICTURE_ID ORDER BY count(*) DESC LIMIT 1;";
	sqlite3_exec(db, query.c_str(), get_int_callback , (void*)&tmp, nullptr);
	query = "";
	query = "SELECT * FROM PICTURES WHERE ID=";
	query += std::to_string(tmp) + ';';
	sqlite3_exec(db, query.c_str(), get_picture_callback, (void*)&pic, nullptr);
	Picture Rpic = *pic;
	delete pic;
	return Rpic;
}
std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user) {
	return std::list<Picture>();
}

//helpfull

int DatabaseAccess::getAlbumIdbyName(std::list<Album> t, std::string albumName)
{
	int id = 1;
	for (auto it = t.begin(); it != t.end(); it++)
	{
		if (it->getName() == albumName)
			return id;
		id++;
	}
}
int DatabaseAccess::getPictureIDbyName(std::string pictureName)
{
	int picId;
	std::string query = "SELECT ID FROM PICTURES WHERE NAME=";
	query = query + "\"" + pictureName + "\";";
	sqlite3_exec(db, query.c_str(), get_int_callback, (void*)&picId, nullptr);
	return picId;
}