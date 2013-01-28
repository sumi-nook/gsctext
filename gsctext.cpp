#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

//! x86
typedef unsigned long DWORD;

struct RScriptGSCHeader{
	DWORD FileSize;
	DWORD HeaderSize;
	DWORD BinarySize1;
	DWORD BinarySize2;
	DWORD TextSize;
};

int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "Usage : " << argv[0] << " [gsc file]" << std::endl;
		return 0;
	}
	
	std::ifstream ifs(argv[1], std::ios::binary | std::ios::in);
	if(ifs.fail()){
		std::cerr << "Error: File cannot open." << std::endl;
		return 0;
	}
	RScriptGSCHeader header;
	ifs.read(static_cast<char*>(static_cast<void*>(&header)), sizeof(RScriptGSCHeader));
	
	ifs.seekg(0);
	ifs.seekg(header.HeaderSize, std::ios::cur);
	ifs.seekg(header.BinarySize1, std::ios::cur);
	ifs.seekg(header.BinarySize2, std::ios::cur);
	
	std::unique_ptr<char> buff(new char[header.TextSize]);
	ifs.read(buff.get(), header.TextSize);
	ifs.close();
	
	std::string data(buff.get(), header.TextSize);
	
	std::size_t pos = 0;
	while((pos = data.find('\0', pos)) != std::string::npos){
		data.replace(pos, 1, "¥n");
	}
	
	std::string oldfile = argv[1];
	std::size_t len = oldfile.find(".");
	if(len == std::string::npos){
		len = oldfile.size();
	}
	std::string newfile = oldfile.substr(0, len) + ".txt";
	std::ofstream ofs(newfile, std::ios::out);
	ofs.write(data.c_str(), data.size());
	ofs.close();
	
	return 0;
}
