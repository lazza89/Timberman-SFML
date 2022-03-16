#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

template<typename Derived, typename T>
class ResourceManager{
public:
	ResourceManager(const std::string& pathsFile){
		LoadPaths(pathsFile);
	}

	virtual ~ResourceManager(){ PurgeResources(); }

	T* GetResource(const std::string& id){
		auto res = Find(id);
		return(res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& id){
		auto path = paths.find(id);
		return(path != paths.end() ? path->second : "");
	}

	bool RequireResource(const std::string& id){
		auto res = Find(id);
		if(res){
			++res->second;
			return true;
		}
		auto path = paths.find(id);
		if (path == paths.end()){ return false; }
		T* resource = Load(path->second);
		if (!resource){ return false; }
		resources.emplace(id, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& id){
		auto res = Find(id);
		if (!res){ return false; }
		--res->second;
		if (!res->second){ Unload(id); }
		return true;
	}

	void PurgeResources(){
		std::cout << "Purging all resources:" << std::endl;
		while(resources.begin() != resources.end()){
			std::cout << "Removing: " 
				<< resources.begin()->first << std::endl;
			delete resources.begin()->second.first;
			resources.erase(resources.begin());
		}
		std::cout << "Purging finished." << std::endl;
	}

protected:
	T* Load(const std::string& path){
		return static_cast<Derived*>(this)->Load(path);
	}
private:
	std::pair<T*,unsigned int>* Find(const std::string& id){
		auto itr = resources.find(id);
		return (itr != resources.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string& id){
		auto itr = resources.find(id);
		if (itr == resources.end()){ return false; }
		delete itr->second.first;
		resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathFile){
		std::ifstream tmpPaths;
		tmpPaths.open(pathFile);
		if(tmpPaths.is_open()){
			std::string line;
			while(std::getline(tmpPaths,line)){
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				paths.emplace(pathName,path);
			}
			tmpPaths.close();
			return;
		}
		std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
	}

	std::unordered_map<std::string, 
		std::pair<T*, unsigned int>> resources;
	std::unordered_map<std::string, std::string> paths;
};