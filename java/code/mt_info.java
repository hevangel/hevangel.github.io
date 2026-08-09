// Macross Tetris v1.0 by Horace Chan
// file : mt_info.java
// description : infomration class

class info
{
	public int player[] = {0,0};
	public int level[] = {1,1};
	public int start_line[] = {0,0};
	public boolean slow_down[] = {false, false};
	public boolean show_next[] = {true, true};
	public boolean sound = false;
	public boolean statistic = true;
	public boolean send_rocks = false;
	public boolean same_pieces = false;
	public int score[] = {0,0};
	public int lines[] = {0,0};
}

class block_info
{
	public int origin[] = {0,0};
	public int block[][] = {{0,0},{0,0},{0,0},{0,0}};
	public int col, state;
}

class score_info
{
	public String name = "";
	public int level = 0;
	public int lines = 0;
	public int score = 0;
	public int state = -1;
}

