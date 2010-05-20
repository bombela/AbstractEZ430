/*
 * lib/probeports.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef PROBEPORTS_H
#define PROBEPORTS_H

#include <vector>
#include <string>

std::vector<std::string> probePorts();

#define RFEZ430_VENDOR  0x0451
#define RFEZ430_PRODUCT 0x16A6

#endif /* PROBEPORTS_H */
