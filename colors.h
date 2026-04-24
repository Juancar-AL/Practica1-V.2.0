#pragma once
// Otra idea de colores en C++:
// 
// https://gist.github.com/Alfonzzoj/db207b89d56f24d9d0b17ff93e091be8
// 
//     cout << BLUE << BG_LGREEN << "This text is BLUE!     " << RESET << endl;
//     cout << BLUE << "This text is BLUE!     "<< endl;
//===Color font code===/
#define BLACK   "\x1B[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1B[37m"
#define ORANGE  "\x1B[38;2;255;128;0m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define LBLUE   "\x1B[38;2;53;149;240m"
#define DBLUE   "\x1B[38;2;11;11;100m"
#define LGREEN  "\x1B[38;2;17;245;120m"
#define DGREEN  "\x1B[38;1;50;32;120m"
#define DRED    "\x1B[38;1;128;0;0m"
#define GRAY    "\x1B[38;2;176;174;174m"
#define RESET   "\x1b[0m"
//===Color background code===/
#define BG_BLACK   "\x1B[40m"
#define BG_RED     "\x1B[41m"
#define BG_GREEN   "\x1B[42m"
#define BG_YELLOW  "\x1B[43m"
#define BG_BLUE    "\x1B[44m"
#define BG_MAGENTA "\x1B[45m"
#define BG_CYAN    "\x1B[46m"
#define BG_WHITE   "\x1B[47m"
#define BG_ORANGE  "\x1B[48;2;255;128;0m"
#define BG_LBLUE   "\x1B[48;2;53;149;240m"
#define BG_LGREEN  "\x1B[48;2;17;245;120m"
#define BG_GRAY    "\x1B[48;2;176;174;174m"
#define BG_ROSE    "\x1B[48;2;255;151;203m"


// Rosa pastel suave de Hello Kitty (RGB: 255, 182, 193)
#define HK_PINK_PASTEL  "\x1B[38;2;255;182;193m"