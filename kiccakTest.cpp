
#include"header.h"


const char* sqlCreate = "CREATE TABLE IF NOT EXISTS hash_table(path PRIMARY KEY,hash_file);";
const char* sqlSelect = "SELECT * FROM hash_table;";

using namespace std;

int main(int argc, char* argv[]){
	setlocale(LC_ALL, ".1251");
	if (argc > 1){
		if (!fileExists(argv[1]))
		{
			cout << "incorrect file path" << endl;
		}
	}
	sqlite3 *db = 0;
	char *err = 0;
	if (sqlite3_open("fixedFiles.db3", &db))
		fprintf(stderr, "Error of opening/creating DB: %s\n", sqlite3_errmsg(db));
	else if (sqlite3_exec(db, sqlCreate, 0, 0, &err))
	{
		fprintf(stderr, "SQL error: %sn", err);
		sqlite3_free(err);
	}
	else{
		if (argc > 1){
			if (fileExists(argv[1])){
				writesum(argv[1], db);
			}				
			else
				cout << "incorrect file path" << endl;
		}
		else{
			cout << "File statuses:" << endl;
			if (sqlite3_exec(db, sqlSelect, callback, 0, &err))
			{
				fprintf(stderr, "SQL error: %sn", err);
				sqlite3_free(err);
			}

		}
	}
	sqlite3_close(db);
	system("pause");
	return 0;
}

int callback(void *obj, int argc, char **argv, char **szColName){
	unsigned char calc[33];
	fileHash(argv[0], calc);
	stringstream ss;
	for (int i = 0; i < 32; i++)
		ss << setfill('0') << setw(2) << hex << (int)calc[i];
	if (strcmp((char *)argv[1], (char *)ss.str().c_str()) == 0){
		cout << argv[0] << ": file is right" << endl;
	}
	else{
		cout << argv[0] << ": file was modified." << endl;
	}
	return 0;
}

void writesum(char  *fileName, sqlite3 *db){
	char sqlInsert[] = "INSERT INTO hash_table VALUES ('%s','%s'); UPDATE hash_table SET hash_file = '%s' WHERE path = '%s' ;";
	string query = "INSERT OR REPLACE INTO hash_table VALUES ('";
	unsigned char ch[33];
	fileHash(fileName, ch);
	stringstream ss;
	for (int i = 0; i < 32; i++)
		ss << setfill('0') << setw(2) << hex << (int)ch[i];
	int len = strlen(sqlInsert) + strlen(fileName) + 32 * 2;
	query += fileName;
	query += "','";
	query += ss.str();
	query += "')";
	char *errS = 0;
	if (sqlite3_exec(db, query.c_str(), 0, 0, &errS))
	{
		fprintf(stderr, "SQL error: %sn", errS);
		sqlite3_free(errS);
	}
	else{
		cout << "File was fixed" << endl;
	}
}
bool fileExists(const char *fname)
{
	return _access(fname, 0) != -1;
}

void fileHash(char  *fileName, unsigned char* ch)
{
	FILE *fp;
	int err = fopen_s(&fp, fileName, "r");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char *buf = (unsigned char *)malloc(size + 1);
	fread(buf, 1, size, fp);
	buf[size] = '\0';
	FIPS202_SHA3_256(buf, size, ch);
	stringstream ss;
	for (int i = 0; i < 32; i++)
		ss << setfill('0') << setw(2) << hex << (int)ch[i];
	ch[32] = '\0';
	if (err)
		fclose(fp);
}