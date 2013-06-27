#include <iostream>

/* 
 * cheese:	1 << 0 , 1 << 1 , 1 << 2 , 1 << 3
 *		
 *		1 << 4 , 1 << 5 , 1 << 6 , 1 << 7
 *
 */

using namespace std;

#define O1	1 << 0
#define O2	1 << 1
#define O3	1 << 2
#define O4	1 << 3
#define X1	1 << 4
#define X2	1 << 5
#define X3	1 << 6
#define X4	1 << 7

const int MAX_BOARD_SIZE= 4;
int board_size= 3;

class Move {
public:
	unsigned int hash_val;
	int x;
	int y;
	int size;	// 1, 2, 3, 4
	int color;	// 0 for O, 1 for X
};

unsigned int board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

void dump_matrix( unsigned int board[][MAX_BOARD_SIZE])
{
	for( int i= 0; i< board_size; i++)
	{
		for( int j= 0; j< board_size; j++)
			cout << board[ j][ i] << " ";
		cout << endl;
	}
}

void dump_board( unsigned int board[][MAX_BOARD_SIZE])
{
	cout << "(0,0)" << endl;
	cout << "  +-----+";
	for( int i= 1; i< board_size; i++)
		cout << "-----+";
	cout << endl;

	for( int y= 0; y< board_size; y++)
	{
		cout << "  |";
		for( int x= 0; x< board_size; x++)
		{
			if( board[x][y] & X4)
				cout << " xxxx|";
			else if( board[x][y] & O4)
				cout << " oooo|";
			else if( board[x][y] & X3)
				cout << " xxx |";
			else if( board[x][y] & O3)
				cout << " ooo |";
			else if( board[x][y] & X2)
				cout << "  xx |";
			else if( board[x][y] & O2)
				cout << "  oo |";
			else if( board[x][y] & X1)
				cout << "  x  |";
			else if( board[x][y] & O1)
				cout << "  o  |";
			else
				cout << "(" << x << "," << y << ")|";
		}
		cout << endl;
		cout << "  +-----+";
		for( int i= 1; i< board_size; i++)
			cout << "-----+";
		cout << endl;
	}
}

bool is_valid_move( unsigned int board[][MAX_BOARD_SIZE], const Move& m)
{
	if( ( m.size < 1) || ( m.size > board_size))
		return false;
	if( ( m.x < 0) || ( m.x > board_size- 1))
		return false;
	if( ( m.y < 0) || ( m.y > board_size- 1))
		return false;

	if( board[ m.x][ m.y] == 0)
		return true;

	unsigned int val= (1 << ( m.size -1));

	if( (val > (board[ m.x][ m.y] & 0x0f)) && (val > (board[ m.x][ m.y] & 0xf0) >> 4))
		return true;

	return false;
}

bool is_equal( unsigned int m1, unsigned int m2)
{
	if( ( m1 == 0) || ( m2 == 0))
		return false;

	if( ( m1 <= 0xf) && (m2 <= 0xf))
		return true;
	else if( ( m1 > 0xf) && (m2 > 0xf))
		return true;

	return false;
}

bool add_move( unsigned int board[][MAX_BOARD_SIZE], const Move& m)
{
	int val= ((1 << (m.size- 1)));
	val <<= (m.color==0)?0:4;
	board[ m.x][ m.y]= (board[ m.x][ m.y] | val);

	return true;
}

bool remove_move( unsigned int board[][MAX_BOARD_SIZE], Move& m)
{
	unsigned int val= board[ m.x][ m.y];

	if( val == 0)
		return false;

	m.color= 0;
	if( val >= 0xf0)
	{
		m.color= 1;
		val >>= 4;
	}

	int i;
	for( i= board_size; i> 0; i--)
		if( (1 << i) & val)
			break;
	m.size= i+ 1;

	if( m.color == 0)
		board[ m.x][ m.y] = board[ m.x][ m.y] - (1 << i);
	else
		board[ m.x][ m.y] = board[ m.x][ m.y] - (1 << i+4);

	return true;
}

bool is_win( unsigned int board[][MAX_BOARD_SIZE])
{
	for( int x= 0; x < board_size; x++)
	{
		for( int y= 0; y < board_size; y++)
		{
			// x+ board_size <= board_size
			// '-'
			if( x == 0)
			{
				int k;
				for( k= x+ 1; k < board_size; k++)
					if( !is_equal( board[ x][ y], board[ k][ y]))
						break;
				if( k == board_size)
					return true;
			}
			// y+ board_size <= board_size
			// '|'
			if( y == 0)
			{
				int k;
				for( k= y+ 1; k < board_size; k++)
					if( !is_equal( board[ x][ y], board[ x][ k]))
						break;
				if( k == board_size)
					return true;
			}
			// '\' 
			if( ( x == 0) && ( y == 0))
			{
				int k;
				for( k= x+ 1; k < board_size; k++)
					if( !is_equal( board[ x][ y], board[ k][ k]))
						break;
				if( k == board_size)
					return true;
			}
			// '/'
			if( ( x+ 1 == board_size) && ( y == 0))
			{
				int k;
				for( k= 1; k < board_size; k++)
					if( !is_equal( board[ x][ y], board[ x- k][ y+ k]))
						break;
				if( k == board_size)
					return true;
			}

		}
	}

	return false;
}

int main( void)
{
	for( int i= 0; i < MAX_BOARD_SIZE; i++)
		for( int j= 0; j< MAX_BOARD_SIZE; j++)
			board[i][j]= 0;

	int color= 0;
	while( 1)
	{
//		dump_matrix( board);
		dump_board( board);

		cout << "color: " << color << endl;
		cout << "1 - put new stone; 2 - move stone:";

		int input;
		do {
			cin >> input;
		} while( input != 1 && input != 2);

		if( input ==1 )
		{
			int x, y, size;
			Move m;

			cout << "x(0.." << board_size-1 << ") y(0.." << board_size-1 << ") size(1.." << board_size << "): ";
			cin >> x >> y >> size;
			m.x= x;
			m.y= y;
			m.color= color;
			m.size= size;

			if( is_valid_move( board, m))
			{
				add_move( board, m);
				color= 1- color;
			}
		}
		else if( input == 2)
		{
			int x1, y1, x2, y2;
			Move m;
			
			cout << "x(0.." << board_size-1 << ") y(0.." << board_size-1 << ")" << " x(0.." << board_size-1 << ") y(0.." << board_size-1 << "):";
			cin >> x1 >> y1 >> x2 >> y2;

			m.x= x1;
			m.y= y1;
			if( !remove_move( board, m))
				continue;

			if( m.color != color)
			{
				add_move( board, m);
				continue;
			}

			m.x= x2;
			m.y= y2;
			if( !is_valid_move( board, m))
			{
				m.x= x1;
				m.y= y1;
				add_move( board, m);
			}
			if( is_valid_move( board, m))
			{
				add_move( board, m);
				color= 1- color;
			}
		}

		if( is_win( board))
			break;
	}

	cout << "color: " << 1- color << " win!!!" << endl;

	return 0;
}

