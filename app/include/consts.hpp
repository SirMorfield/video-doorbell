#pragma once

// This file contains all the typedefs for environment variables

#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

enum Camera_type {
	FRONT_DOOR = 0,
	RPI,
	HALLWAY,
	N // counter
};

typedef struct {
	std::string number;
	std::string name;
} Occupant;
std::ostream&		  operator<<(std::ostream& os, const Occupant& occ);
std::vector<Occupant> read_occupants(const std::array<std::string, Camera_type::N>& cameras, size_t max_occupant_name_length);

typedef struct {
	float									window_width;
	float									window_height;
	size_t									n_occupants;
	std::vector<Occupant>					occupants;
	std::array<std::string, Camera_type::N> camera_numbers;
	size_t									max_occupant_name_length;
	std::chrono::milliseconds::rep			call_timeout;
} Constants;

const Constants& consts();
