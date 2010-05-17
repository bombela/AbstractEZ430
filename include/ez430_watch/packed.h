/*
 * packed.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef PACKED_H
#define PACKED_H

#ifdef __GNUG__
	#define PACKED __attribute__((packed))
#elif VISUAL_COMPILER
	#define PACKED __declspec(align(8))
#else
	#error PACKED macro not supported for this compiler
#endif

#endif /* PACKED_H */
