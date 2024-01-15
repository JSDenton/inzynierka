#include "config.h"

vector<float> import_object(string path) {
	ifstream objectFile;
	objectFile.open("vertexShader.txt");
	vector<float> object;
	if (objectFile.is_open()) {
		string line = "";
		while (getline(objectFile, line)) {
			
		}
	}
	objectFile.close();
}