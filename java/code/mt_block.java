// Macross Tetris v1.0 by Horace Chan
// file : mt_block.java
// description : block class
import java.awt.*;
import java.applet.*;
import java.util.*;

public class mt_block
{
	protected block_info data, temp_data;

	protected mactetris main;
	protected mt_game game;
	protected int side;
	
	public mt_block(mactetris m, mt_game g, int s)
	{
		main = m; 
		game = g;
		side = s;
		data = new block_info();
		temp_data = new block_info();
	}

	public synchronized mt_block new_block(int type)
	{
		data.state = 0;

		switch (type)
		{
			case 0:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = -1;	data.block[0][1] = 0; 
				data.block[1][0] = 0;	data.block[1][1] = 0;
				data.block[2][0] = 1;	data.block[2][1] = 0; 
				data.block[3][0] = -1;	data.block[3][1] = -1; 
				data.col = 1;
			break;
			case 1:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = -1;	data.block[0][1] = 0; 
				data.block[1][0] = 0;	data.block[1][1] = 0;
				data.block[2][0] = 1;	data.block[2][1] = 0; 
				data.block[3][0] = 1;	data.block[3][1] = -1; 
				data.col = 2;
			break;
			case 2:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = -1;	data.block[0][1] = 0; 
				data.block[1][0] = 0;	data.block[1][1] = 0;
				data.block[2][0] = -1;	data.block[2][1] = -1; 
				data.block[3][0] = 0;	data.block[3][1] = -1; 
				data.col = 3;
			break;
			case 3:
				data.origin[0] = 5; data.origin[1] = 19;
				data.block[0][0] = -1;	data.block[0][1] = 0; 
				data.block[1][0] = -2;	data.block[1][1] = 0; 
				data.block[2][0] = 0;	data.block[2][1] = 0;
				data.block[3][0] = 1;	data.block[3][1] = 0; 
				data.col = 4;
			break;
			case 4:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = 0;	data.block[0][1] = 0;
				data.block[1][0] = 1;	data.block[1][1] = 0; 
				data.block[2][0] = -1;	data.block[2][1] = -1; 
				data.block[3][0] = 0;	data.block[3][1] = -1; 
				data.col = 5;
			break;
			case 5:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = -1;	data.block[0][1] = 0;
				data.block[1][0] = 0;	data.block[1][1] = 0; 
				data.block[2][0] = 0;	data.block[2][1] = -1; 
				data.block[3][0] = 1;	data.block[3][1] = -1; 
				data.col = 6;
			break;
			case 6:
				data.origin[0] = 5; data.origin[1] = 20;
				data.block[0][0] = -1;	data.block[0][1] = 0;
				data.block[1][0] = 0;	data.block[1][1] = 0; 
				data.block[2][0] = 1;	data.block[2][1] = 0; 
				data.block[3][0] = 0;	data.block[3][1] = -1; 
				data.col = 7;
			break;
		}

		return this;
	}

	public synchronized mt_block copy_block(mt_block source)
	{
		int i;

		data.origin[0] = source.data.origin[0];
		data.origin[1] = source.data.origin[1];
		for (i=0; i<4; i++)
		{
			data.block[i][0] = source.data.block[i][0];
			data.block[i][1] = source.data.block[i][1];
		}

		data.col = source.data.col;
		data.state = source.data.state;
		return this;
	}

	public synchronized void copy_data(block_info dest, block_info sour)
	{
		int i;

		dest.origin[0] = sour.origin[0];
		dest.origin[1] = sour.origin[1];
		for (i=0; i<4; i++)
		{
			dest.block[i][0] = sour.block[i][0];
			dest.block[i][1] = sour.block[i][1];
		}

		dest.col = sour.col;
		dest.state = sour.state;
	}

	public synchronized int move_horz(int x)
	{
		copy_data(temp_data, data);
		temp_data.origin[0] += x;
		if (game.game_array[side].is_valid(temp_data))
		{
			unpaint(true);
			data.origin[0] += x;
			paint(true);
			return x;
		}
		return 0;
	}

	public synchronized int move_down(boolean drop_one)
	{
		int line_dropped;

		copy_data(temp_data, data);
		if(drop_one)
		{
			temp_data.origin[1]--;
			if (game.game_array[side].is_valid(temp_data))
			{
				unpaint(true);
				data.origin[1]--;
				paint(true);
				return 1;
			}
		}
		else
		{
			do
			{
				temp_data.origin[1]--;
			} while (game.game_array[side].is_valid(temp_data));

			line_dropped = data.origin[1] - temp_data.origin[1] - 1;
	
			if (line_dropped > 0)
			{
				unpaint(true);
				data.origin[1] = temp_data.origin[1] + 1;
				paint(true);
				return line_dropped;
			}
		}
		return 0;
	}

	public synchronized void move_up()
	{
		while (!game.game_array[side].is_valid(data))
			data.origin[1]++;
		return;
	}

	public synchronized int rotate(boolean direction)
	{
		if (data.col == 3)
			return 3;

		copy_data(temp_data, data);
		(direction) ? temp_data.state++ : temp_data.state--;
		switch (temp_data.col)
		{
			case 1: case 2: case 7:
				if (temp_data.state > 3)
					temp_data.state = 0; 
				if (temp_data.state < 0)
					temp_data.state = 3;
			break;
			case 4: case 5: case 6:
				if (temp_data.state > 1)
					temp_data.state = 0; 
				if (temp_data.state < 0)
					temp_data.state = 1;
			break;
		}
		
		switch(temp_data.col)
		{
			case 1:
				switch (temp_data.state)
				{
					case 0:
						temp_data.block[0][0] = -1;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = -1;	temp_data.block[3][1] = -1; 
					break;
					case 1:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
						temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
					break;
					case 2:
						temp_data.block[0][0] = 1;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = -1;	temp_data.block[1][1] = -1; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
						temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
					break;
					case 3:
						temp_data.block[0][0] = -1;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 1; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;
				}
			break;
			case 2:
				switch (temp_data.state)
				{
					case 0:
						temp_data.block[0][0] = -1;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
					break;
					case 1:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = 1;	temp_data.block[1][1] = 1; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;
					case 2:
						temp_data.block[0][0] = -1;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = -1;	temp_data.block[1][1] = -1; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
						temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
					break;
					case 3:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = -1;	temp_data.block[2][1] = -1; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;
				}
			break;
			case 7:
				switch (temp_data.state)
				{
					case 0:
						temp_data.block[0][0] = -1;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;
					case 1:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;
					case 2:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 0; 
						temp_data.block[1][0] = -1;	temp_data.block[1][1] = -1; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
						temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
					break;
					case 3:
						temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
						temp_data.block[1][0] = -1;	temp_data.block[1][1] = 0; 
						temp_data.block[2][0] = 0;	temp_data.block[2][1] = 0; 
						temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
					break;							
				}
			break;
			case 4:
				if (temp_data.state == 0)
				{
					temp_data.block[0][0] = -2;	temp_data.block[0][1] = 0; 
					temp_data.block[1][0] = -1;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = 0;	temp_data.block[2][1] = 0; 
					temp_data.block[3][0] = 1;	temp_data.block[3][1] = 0; 
				}
				else
				{
					temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
					temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
					temp_data.block[3][0] = 0;	temp_data.block[3][1] = -2; 
				}
			break;
			case 5:
				if (temp_data.state == 0)
				{
					temp_data.block[0][0] = 0;	temp_data.block[0][1] = 0; 
					temp_data.block[1][0] = 1;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = -1;	temp_data.block[2][1] = -1; 
					temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
				}
				else
				{
					temp_data.block[0][0] = 0;	temp_data.block[0][1] = 1; 
					temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
					temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
				}
			break;
			case 6:
				if (temp_data.state == 0)
				{
					temp_data.block[0][0] = -1;	temp_data.block[0][1] = 0; 
					temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = 0;	temp_data.block[2][1] = -1; 
					temp_data.block[3][0] = 1;	temp_data.block[3][1] = -1; 
				}
				else
				{
					temp_data.block[0][0] = 1;	temp_data.block[0][1] = 1; 
					temp_data.block[1][0] = 0;	temp_data.block[1][1] = 0; 
					temp_data.block[2][0] = 1;	temp_data.block[2][1] = 0; 
					temp_data.block[3][0] = 0;	temp_data.block[3][1] = -1; 
				}
			break;
		}

		if (game.game_array[side].is_valid(temp_data))
		{
			unpaint(true);
			copy_data(data, temp_data);
			paint(true);
		}

		return data.col;
	}

	public void unpaint(boolean in_area)
	{
		Graphics g;
		int start_pos, x, y, i;

		g = main.getGraphics();
		if (in_area)
		{
			(side==0) ? start_pos = 17 : start_pos = 323;
			g.setColor(Color.black);
			for (i=0; i<4; i++)
			{
				x = start_pos+16*(data.origin[0]+data.block[i][0]);
				y = 43+16*(19-(data.origin[1]+data.block[i][1]));
				if (y >= 43)
					paint_square(g, x, y, 0);
			}
		}
		else
		{
			(side == 0) ? start_pos = 180 : start_pos = 250;
			g.clipRect(start_pos,32,70,32);
			g.drawImage(main.game_bg, 0, 0, main);
		}
	}
	
	public void paint(boolean in_area)
	{
		Graphics g;
		int start_pos, x, y, i;

		g = main.getGraphics();
		if (in_area)
		{
			(side==0) ? start_pos = 17 : start_pos = 323;
			for (i=0; i<4; i++)
			{
				x = start_pos+16*(data.origin[0]+data.block[i][0]);
				y = 43+16*(19-(data.origin[1]+data.block[i][1]));
				if (y >= 43)
					paint_square(g, x, y, data.col);
			}
		}
		else
		{
			(side == 0) ? start_pos = 180 : start_pos = 250;
			for (i=0; i<4; i++)
			{
				x = start_pos+35+16*data.block[i][0];
				y = 32-16*data.block[i][1];
				paint_square(g, x, y, data.col);
			}
		}
	}

	protected void paint_square(Graphics g, int x, int y, int col)
	{
		if (col > 0)
			g.drawImage(main.square[col-1], x, y, main);
		else
			g.fillRect(x, y, 16, 16);
	}
}



