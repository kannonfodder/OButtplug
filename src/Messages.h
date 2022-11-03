#pragma once



struct Ping {
	int Id;
};
struct PingMessage {
	Ping Ping;
	
};

void to_json(json& j, const PingMessage& pm) {
	json arry;
	arry.push_back(json{ {"Ping",json{{"Id",pm.Ping.Id}}} } );	
	j = arry;
}