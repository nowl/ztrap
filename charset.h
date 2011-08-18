#ifndef __CHARSET_H__
#define __CHARSET_H__

/* This is all kind of ugly but it gets the job done.. */

#define CS_X_SIZE     22
#define CS_Y_SIZE     22
#define CS_X_GAP      20
#define CS_Y_GAP       7
#define CS_OFFSET_X   34
#define CS_OFFSET_Y   34

#define CS_BUILD(x, y)   "charset.png", CS_OFFSET_X + (CS_X_SIZE + CS_X_GAP) * x, CS_OFFSET_Y + (CS_Y_SIZE + CS_Y_GAP) * y, CS_X_SIZE, CS_Y_SIZE

#define CS_a            CS_BUILD(0, 0)
#define CS_b            CS_BUILD(1, 0)
#define CS_c            CS_BUILD(2, 0)
#define CS_d            CS_BUILD(3, 0)
#define CS_e            CS_BUILD(4, 0)
#define CS_f            CS_BUILD(5, 0)
#define CS_g            CS_BUILD(6, 0)
#define CS_h            CS_BUILD(7, 0)
#define CS_i            CS_BUILD(8, 0)
#define CS_j            CS_BUILD(9, 0)
#define CS_k            CS_BUILD(10, 0)
#define CS_l            CS_BUILD(11, 0)
#define CS_m            CS_BUILD(12, 0)
#define CS_n            CS_BUILD(0, 1)
#define CS_o            CS_BUILD(1, 1)
#define CS_p            CS_BUILD(2, 1)
#define CS_q            CS_BUILD(3, 1)
#define CS_r            CS_BUILD(4, 1)
#define CS_s            CS_BUILD(5, 1)
#define CS_t            CS_BUILD(6, 1)
#define CS_u            CS_BUILD(7, 1)
#define CS_v            CS_BUILD(8, 1)
#define CS_w            CS_BUILD(9, 1)
#define CS_x            CS_BUILD(10, 1)
#define CS_y            CS_BUILD(11, 1)
#define CS_z            CS_BUILD(12, 1)
#define CS_A            CS_BUILD(0, 2)
#define CS_B            CS_BUILD(1, 2)
#define CS_C            CS_BUILD(2, 2)
#define CS_D            CS_BUILD(3, 2)
#define CS_E            CS_BUILD(4, 2)
#define CS_F            CS_BUILD(5, 2)
#define CS_G            CS_BUILD(6, 2)
#define CS_H            CS_BUILD(7, 2)
#define CS_I            CS_BUILD(8, 2)
#define CS_J            CS_BUILD(9, 2)
#define CS_K            CS_BUILD(10, 2)
#define CS_L            CS_BUILD(11, 2)
#define CS_M            CS_BUILD(12, 2)
#define CS_N            CS_BUILD(0, 3)
#define CS_O            CS_BUILD(1, 3)
#define CS_P            CS_BUILD(2, 3)
#define CS_Q            CS_BUILD(3, 3)
#define CS_R            CS_BUILD(4, 3)
#define CS_S            CS_BUILD(5, 3)
#define CS_T            CS_BUILD(6, 3)
#define CS_U            CS_BUILD(7, 3)
#define CS_V            CS_BUILD(8, 3)
#define CS_W            CS_BUILD(9, 3)
#define CS_X            CS_BUILD(10, 3)
#define CS_Y            CS_BUILD(11, 3)
#define CS_Z            CS_BUILD(12, 3)
#define CS_BACKQUOTE    CS_BUILD(0, 4)
#define CS_1            CS_BUILD(1, 4)
#define CS_2            CS_BUILD(2, 4)
#define CS_3            CS_BUILD(3, 4)
#define CS_4            CS_BUILD(4, 4)
#define CS_5            CS_BUILD(5, 4)
#define CS_6            CS_BUILD(6, 4)
#define CS_7            CS_BUILD(7, 4)
#define CS_8            CS_BUILD(8, 4)
#define CS_9            CS_BUILD(9, 4)
#define CS_0            CS_BUILD(10, 4)
#define CS_MINUS        CS_BUILD(11, 4)
#define CS_EQUALS       CS_BUILD(12, 4)
#define CS_TILDE        CS_BUILD(0, 5)
#define CS_EXCLAMATION  CS_BUILD(1, 5)
#define CS_AT           CS_BUILD(2, 5)
#define CS_HASH         CS_BUILD(3, 5)
#define CS_STRING       CS_BUILD(4, 5)
#define CS_PERCENT      CS_BUILD(5, 5)
#define CS_CAROT        CS_BUILD(6, 5)
#define CS_AMPERSAND    CS_BUILD(7, 5)
#define CS_ASTERISK     CS_BUILD(8, 5)
#define CS_LEFT_PAREN   CS_BUILD(9, 5)
#define CS_RIGHT_PAREN  CS_BUILD(10, 5)
#define CS_UNDERSCORE   CS_BUILD(11, 5)
#define CS_PLUS         CS_BUILD(12, 5)
#define CS_LEFT_SQUARE  CS_BUILD(0, 6)
#define CS_RIGHT_SQUARE CS_BUILD(1, 6)
#define CS_BACKSLASH    CS_BUILD(2, 6)
#define CS_SEMICOLON    CS_BUILD(3, 6)
#define CS_SINGLE_QUOTE CS_BUILD(4, 6)
#define CS_COMMA        CS_BUILD(5, 6)
#define CS_DOT          CS_BUILD(6, 6)
#define CS_SLASH        CS_BUILD(7, 6)
#define CS_LEFT_BRACE   CS_BUILD(0, 7)
#define CS_RIGHT_BRACE  CS_BUILD(1, 7)
#define CS_PIPE         CS_BUILD(2, 7)
#define CS_COLON        CS_BUILD(3, 7)
#define CS_QUOTE        CS_BUILD(4, 7)
#define CS_LESS_THAN    CS_BUILD(5, 7)
#define CS_GREATER_THAN CS_BUILD(6, 7)
#define CS_QUESTION     CS_BUILD(7, 7)


#endif  /* __CHARSET_H__ */
