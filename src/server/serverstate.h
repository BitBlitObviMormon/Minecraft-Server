#pragma once
enum class ServerState
{
	Handshaking = 0, 
	Status = 1,
	Login = 2,
	Play = 3
};