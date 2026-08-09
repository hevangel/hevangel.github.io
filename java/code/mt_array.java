// Macross Tetris v1.0 by Horace Chan
// file : mt_array.java
// description : game_array class
import java.awt.*;
import java.applet.*;
import java.util.*;

class mt_array
{
	protected int data[][] = new int[24][10];
	
	protected mactetris main;
	protected mt_game game;
	protected int side;
	
	protected Image buffer;
	boolean fulled_lines[] = {false,false,false,false};
	int line_counter = 0;
	
	public mt_array(mactetris m, mt_game g, int s)
	{
		main = m;
		game = g;
		side = s;
		buffer = main.createImage(160, 320);
	}

	public synchronized void reset(int raise)
	{
		int i, j, temp;

		for (i=0; i<24;i++)
		{
			if (i >= raise)
				for (j=0; j<10; j++)
					data[i][j] = 0;
			else
				for (j=0; j<10; j++)
				{
					temp = Math.abs(game.ran.nextInt())%20;
					(temp > 7) ? temp = 0: temp;
						data[i][j] = temp;
				}
		}
	}

	public synchronized void set_square(int x, int y, int col)
	{ data[x][y] = col; }

	public synchronized int get_square(int x, int y)
	{ return data[x][y]; }

	public synchronized boolean is_valid(block_info check)
	{
		boolean flag;
		int i;

		flag = true;
 		for (i=0; i<4; i++)
		{
			if (				
				(check.origin[1] + check.block[i][1] < 0) ||
				(check.origin[0] + check.block[i][0] < 0) ||
				(check.origin[0] + check.block[i][0] > 9)
				)
				flag = false;
			else if (data[check.origin[1]+check.block[i][1]]
						[check.origin[0]+check.block[i][0]] != 0)
				flag = false;
		}		
		return flag;
	}

	public synchronized int settle_block(mt_block target, boolean send_rocks)
	{
		int i, j, k, temp = -1;

		for (i=0; i<4 ; i++)
			fulled_lines[i] = false;
		line_counter = 0;
		
		for (i=0; i<4; i++)
			data[target.data.origin[1]+target.data.block[i][1]]
				[target.data.origin[0]+target.data.block[i][0]] = target.data.col;
				
		for (i=0; i<4; i++)
		{
			if (target.data.origin[1]+target.data.block[i][1] == temp)
				continue;
			
			temp = target.data.origin[1]+target.data.block[i][1];
			fulled_lines[target.data.block[i][1]+2] = (data[temp][0] != 0);
			
			for (j=1; j<10; j++)
				fulled_lines[target.data.block[i][1]+2] &= (data[temp][j] != 0);
			
			if (fulled_lines[target.data.block[i][1]+2])
			{
				unpaint_line(temp);
				line_counter++;
			}
		}

		if (line_counter <= 0)
			return 0;

		for (i=j=(target.data.origin[1]+target.data.block[3][1]); j<20; j++)
		{
			if (((j-target.data.origin[1]) >= -2) && ((j - target.data.origin[1]) <= 1))
				if (fulled_lines[j-target.data.origin[1]+2])
					continue;

			for (k=0; k<10; k++)
				data[i][k] = data[j][k];
			i++;
		}

		if ((line_counter > 1)&&(game.flag[1-side]<3)&&(game.data.send_rocks))
			game.game_array[1-side].recieve_lines(line_counter-1);

		return line_counter;
	}

	public synchronized boolean recieve_lines(int raise)
	{
		int i, j, temp;
		
		for (i=21; i>=raise; i--)
			for (j=0; j<10; j++)
				data[i][j] = data[i-raise][j];

		  for (i=0; i<raise; i++)
			for (j=0; j<10; j++)
			{
				temp = Math.abs(game.ran.nextInt())%10;
				if (temp > 7)	temp = 0;
				data[i][j] = temp;
			}
	
		game.cur_block[side].move_up();
		paint(true);
		game.cur_block[side].paint(true);
		return true;
	}

	public synchronized boolean is_over(mt_block target)
	{
		int i;
		boolean over = false;
		
		for (i=0; i<4; i++)
			if (data[target.data.origin[1]+target.data.block[i][1]]
				[target.data.origin[0]+target.data.block[i][0]] != 0)
				over = true;
		for (i=0; i<10; i++)
			if (data[20][i] != 0) 
				over = true;

		if (over)
			paint_over(target);
		
		return over;
	}

	public void paint_over(mt_block target)
	{
		Graphics g;
		int start_pos, i, j;

		g = buffer.getGraphics();
		for (i=0; i<20; i++)
			for (j=0; j<10; j++)
				paint_square(g, (16 * j), (16*(19-i)), target.data.col);
		
		(side==0) ?	start_pos = 17 : start_pos = 323;
		g = main.getGraphics();
		g.drawImage(buffer, start_pos, 43, main);
		g.setFont(main.msg_font);
		g.setColor(Color.white);
		g.fillRect(start_pos+16, 180, 128, 32);
		g.setColor(Color.black);
		g.drawRect(start_pos+18,182, 124, 28);
		g.drawString("Game Over", start_pos+32, 202);
	}

	public void unpaint_line(int line)
	{
		Graphics g;
		int start_pos;

		g = main.getGraphics();
		g.setColor(Color.black);
		(side==0) ?	start_pos = 17 : start_pos = 323;
		 g.fillRect(start_pos, 43+(16*(19-line)), 160, 16);
	}

	public void paint(boolean update)
	{
		Graphics g;
		int start_pos, i, j;
	
		g = buffer.getGraphics();
		g.setColor(Color.black);
		for (i=0; i<20; i++)
			for (j=0; j<10; j++)
				paint_square(g, (16 * j), (16*(19-i)), data[i][j]);
		
		(side==0) ?	start_pos = 17 : start_pos = 323;
		g = main.getGraphics();
		g.drawImage(buffer, start_pos, 43, main);
	}
	
	protected void paint_square(Graphics g, int x, int y, int col)
	{
		if (col == 0)
			g.fillRect(x, y, 16, 16);
		else
			g.drawImage(main.square[col-1], x, y, main);
	}
}
