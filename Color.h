/*
 * Color.h
 *
 *  Created on: Apr 25, 2015
 *      Author: paracite
 */

#ifndef COLOR_H_
#define COLOR_H_

#define RESET              "\033[00m"
#define BLACK              "\033[30m"      /* Black */
#define RED                "\033[31m"      /* Red */
#define GREEN              "\033[32m"      /* Green */
#define YELLOW             "\033[33m"      /* Yellow */
#define BLUE               "\033[34m"      /* Blue */
#define MAGENTA            "\033[35m"      /* Magenta */
#define CYAN               "\033[36m"      /* Cyan */
#define WHITE              "\033[37m"      /* White */
#define BOLD               "\033[01m"
#define CLRSCR             std::cout << "\033[2J" << std::endl
#define CLR(colr,msg)      std::cout << colr << msg << "\033[0m"

#endif /* COLOR_H_ */
