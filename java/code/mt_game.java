// Macross Tetris v1.0 by Horace Chan
// file : mt_game.java
// description : game class

import java.awt.*;
import java.applet.*;
import java.util.*;

public class mt_game 
{
	protected mactetris main;
	protected info data;

	protected int flag[] = {0, 0};
	protected int drop_bonus[] = {0,0};
	protected int game_stat[][] = new int[2][8];
	
	protected mt_block cur_block[] = new mt_block[2];
	protected mt_block next_block[] = new mt_block[2];
	protected mt_array game_array[] = new mt_array[2];

	protected Vector queue[] = new Vector[2];
	protected Random ran;
	protected Date date;

	public mt_game(mactetris m)
	{
		main = m;

		date = new Date();
		ran = new Random();
		ran.setSeed(date.getSeconds());
		queue[0] = new Vector();
		queue[1] = new Vector();
		
		cur_block[0] = new mt_block(m, this, 0);
		cur_block[1] = new mt_block(m, this, 1);
		next_block[0] = new mt_block(m, this, 0);
		next_block[1] = new mt_block(m, this, 1);
		game_array[0] = new mt_array(m, this, 0);
		game_array[1] = new mt_array(m, this, 1);
	}

	public void reset(info d)
	{
		int i, temp;

		data = d;
		game_array[0].reset(data.start_line[0]);
		game_array[1].reset(data.start_line[1]);
		flag[0] = flag[1] = 0;
		drop_bonus[0] = drop_bonus[1] = 0;
		for (i=0; i<7; i++)
			game_stat[0][i] = game_stat[1][i] = 0;

		temp = Math.abs(ran.nextInt()%7);
		cur_block[0].new_block(temp);
		game_stat[0][temp]++;
		if (!data.same_pieces)
			temp = Math.abs(ran.nextInt()%7);
		cur_block[1].new_block(temp);
		game_stat[1][temp]++;
		
		temp = Math.abs(ran.nextInt()%7);
		next_block[0].new_block(temp);
		game_stat[0][temp]++;
		if (!data.same_pieces)
			temp = Math.abs(ran.nextInt()%7);
		next_block[1].new_block(temp);
		game_stat[1][temp]++;

		game_stat[0][7] = game_stat[1][7] = 2;
		
		if (main.ready > 1)
		{
			main.block_thread[0].set_state(2);
			main.block_thread[1].set_state(2);
			draw_screen();
		}
	}

	public synchronized void create_block(int side)
	{
		Integer storage;
		int temp;

		cur_block[side].copy_block(next_block[side]);

		if (data.same_pieces)
		{
			if (queue[side].isEmpty())
			{
				storage = new Integer(Math.abs(ran.nextInt()%7));
				if (flag[1-side] < 3)
					queue[1-side].addElement(storage);
				temp = storage.intValue();
			}
			else
			{
				storage = (Integer)queue[side].firstElement();
				queue[side].removeElementAt(0);
				temp = storage.intValue();
				storage = null;
				System.gc();
			}
		}
		else
			temp = Math.abs(ran.nextInt()%7);
				
		next_block[side].new_block(temp);

		drop_bonus[side] = 0;
		game_stat[side][temp]++;
		game_stat[side][7]++;
		cur_block[side].paint(true);
		if (data.show_next[side])
			display_next(side,true);
		if (data.statistic)
			display_statistic(side,temp,true);
	}
	
	public synchronized void keyDown(int key)
	{
		int temp;
		
		switch (key)
		{
			case 97:	//left left
				cur_block[0].move_horz(-1);
			break;
			case 100:	//left right
				cur_block[0].move_horz(+1);
			break;
			case 115:	//left rotate
				cur_block[0].rotate(true);
			break;
			case 119:	//left rotate reverse
				cur_block[0].rotate(false);
			break;
			case 120:	//left down
			case 32:
				if (data.slow_down[0])
					drop_bonus[0] += cur_block[0].move_down(true);
				else
					drop_bonus[0] += cur_block[0].move_down(false);
			break;
			case 101:	//left level up
				main.block_thread[0].set_level(++data.level[0]);
				display_level(0, true);
			break;
			case 122:	//left toggle show next
				data.show_next[0] = !data.show_next[0];
				if (data.show_next[0])
					next_block[0].paint(false);
				else
					next_block[0].unpaint(false);
			break;
			case 99:	//left toggle slow down
				data.slow_down[0] = !data.slow_down[0];
			break;
			case 52:	//right left
				cur_block[1].move_horz(-1);
			break;
			case 54:	//right right
				cur_block[1].move_horz(+1);
			break;
			case 53:	//right rotate
				cur_block[1].rotate(true);
			break;
			case 56:	//right rotate reverse
				cur_block[1].rotate(false);
			break;
			case 50:	//right down
			case 10:
				if (data.slow_down[1])
					temp = drop_bonus[1] = cur_block[1].move_down(true);
				else
					drop_bonus[1] += cur_block[1].move_down(false);
			break;
			case 57:	//right level up
				main.block_thread[1].set_level(++data.level[1]);
				display_level(1, true);
			break;
			case 49:	//right toggle show next
				data.show_next[1] = !data.show_next[1];
				if (data.show_next[1])
					next_block[1].paint(false);
				else
					next_block[1].unpaint(false);
			break;
			case 51:	//right toggle slow down
				data.slow_down[1] = !data.slow_down[1];
			break;
			default:
				System.out.println("Key : "+key);
		}
	}

	public synchronized void drop_block(int side)
	{
		int line_count, temp;

		switch (flag[side])
		{
			case 0:
				if (cur_block[side].move_down(true) == 0)
				{
					line_count = game_array[side].settle_block(cur_block[side], data.send_rocks);
					
					if (!data.show_next[side])
						data.score[side] += data.level[side];
					data.score[side] += 2 + (2 * data.level[side]) + (drop_bonus[side] * data.level[side] / 10 );
					
					if (line_count > 0)
					{
						data.lines[side] += line_count;
						flag[side] = 1;
						switch(line_count)	{
							case 1:	data.score[side] += data.level[side] * 10;	break;
							case 2:	data.score[side] += data.level[side] * 25;	break;
							case 3:	data.score[side] += data.level[side] * 75;	break;
							case 4:	data.score[side] += data.level[side] * 300;	break;
						}
						game_array[side].paint(true);
					}
					else
					{
						if (game_array[side].is_over(next_block[side]))
						{
							game_over(side);
							return;
						}
					}
					create_block(side);
					display_score(side, true);
				}
			break;
			case 1:
			 	if (game_array[side].is_over(next_block[side]))
				{
					game_over(side);
					return;
				}
				flag[side] = 0;
				display_lines(side, true);
				if ((data.lines[side] / 10) >= data.level[side])
				{
					main.block_thread[side].set_level(++data.level[side]);
					display_level(side, true);
				}
			break;
		}	
	}

	public synchronized void game_over(int side)
	{
		flag[side] = 3;
		main.block_thread[side].set_state(0);
		if ((flag[0] == 3) && (flag[1] == 3))
		{
			if (data.send_rocks)
				data.player[side] += data.player[1-side];
			main.ch_state(data, 3);
		}
	}

	public void display_level(int side, boolean update)
	{
		Graphics g;
		StringBuffer sztemp;
		int start_pos;
		
		(side == 0) ? start_pos = 210 : start_pos = 275;
		sztemp = new StringBuffer(String.valueOf(data.level[side]));
		if (sztemp.length() < 2)
			sztemp.insert(0,'0');

		if (update)
		{
			g = main.getGraphics();
			g.clipRect(start_pos,64,16,18);
			g.drawImage(main.game_bg, 0, 0, main);
		}
		
		g = main.getGraphics();
		g.setFont(main.scr_font);
		g.setColor(Color.white);
		g.drawString(sztemp.toString(),start_pos,82);
	}
	
	public void display_score(int side, boolean update)
	{
		Graphics g;
		StringBuffer sztemp;
		int start_pos;
		
		(side == 0) ? start_pos = 185 : start_pos= 275;
		sztemp = new StringBuffer(String.valueOf(data.score[side]));
		while(sztemp.length() < 5)
			sztemp.insert(0,'0');

		if (update)
		{
			g = main.getGraphics();
			g.clipRect(start_pos,86,40,18);
			g.drawImage(main.game_bg, 0, 0, main);
		}
		
		g = main.getGraphics();
		g.setFont(main.scr_font);
		g.setColor(Color.white);
		g.drawString(sztemp.toString(),start_pos,104);
	}

	public void display_lines(int side, boolean update)
	{
		Graphics g;
		StringBuffer sztemp;
		int start_pos;
		
		(side == 0) ? start_pos = 200 : start_pos = 275;
		sztemp = new StringBuffer(String.valueOf(data.lines[side]));
		while(sztemp.length() < 3)
			sztemp.insert(0,'0');

		if (update)
		{
			g = main.getGraphics();
			g.clipRect(start_pos,108,24,18);
			g.drawImage(main.game_bg, 0, 0, main);
		}
		
		g = main.getGraphics();
		g.setFont(main.scr_font);
		g.setColor(Color.white);
		g.drawString(sztemp.toString(),start_pos,126);
	}
 
	public void display_next(int side, boolean update)
	{
		if (update)
			next_block[side].unpaint(false);
		next_block[side].paint(false);
	}

	public void display_statistic(int side, int select, boolean update)
	{
		Graphics g;
		StringBuffer sztemp;
		int start_pos, i;

		g = main.getGraphics();
		if (!update)
		{
			g.drawImage(main.s_block[0],232,145+0*28,main);
			g.drawImage(main.s_block[1],232,145+1*28,main);
			g.drawImage(main.s_block[2],226,145+2*28,main);
			g.drawImage(main.s_block[3],226,145+3*28+7,main);
			g.drawImage(main.s_block[4],232,145+4*28,main);
			g.drawImage(main.s_block[5],232,145+5*28,main);
			g.drawImage(main.s_block[6],232,145+6*28,main);
			g.drawImage(main.sigma,242,350,main);

			g.setFont(main.scr_font);
			g.setColor(Color.white);
			for (i=0; i<7; i++)
			{
  				sztemp = new StringBuffer(String.valueOf(game_stat[0][i]));
				while(sztemp.length() < 3)
					sztemp.insert(0,'0');
				g.drawString(sztemp.toString(), 195, 158+i*28);
				sztemp = new StringBuffer(String.valueOf(game_stat[1][i]));
				while(sztemp.length() < 3)
					sztemp.insert(0,'0');
				g.drawString(sztemp.toString(), 280, 158+i*28);
			}
			
			sztemp = new StringBuffer(String.valueOf(game_stat[0][7]));
			while(sztemp.length() < 4)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(),195, 358);
			sztemp = new StringBuffer(String.valueOf(game_stat[1][7]));
			while(sztemp.length() < 4)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(),274, 358);
		}
		else
		{
			(side == 0) ? start_pos = 195 : start_pos = 280;
			g.clipRect(start_pos, 140+select*28, 32, 20);
			g.drawImage(main.game_bg, 0, 0, main);
	 		
			g = main.getGraphics();
			g.setFont(main.scr_font);
			g.setColor(Color.white);
			sztemp = new StringBuffer(String.valueOf(game_stat[side][select]));
			while(sztemp.length() < 3)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(), start_pos, 158+select*28);
										
			(side == 0) ? start_pos = 195 : start_pos = 274;
			g.clipRect(start_pos, 340, 40, 20);
			g.drawImage(main.game_bg, 0, 0, main);
			
			g = main.getGraphics();
			g.setFont(main.scr_font);
			g.setColor(Color.white);
			sztemp = new StringBuffer(String.valueOf(game_stat[side][7]));
			while(sztemp.length() < 4)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(), start_pos, 358);
		}
	}
	
	public void draw_screen()
	{
		Graphics g;
		int i;

		g = main.getGraphics();
		
		if (main.ready > 1)
		{
			g.drawImage(main.game_bg, 0, 0, main);
			if (flag[0] < 3)
			{
				game_array[0].paint(true); 
				cur_block[0].paint(true);
			}
			else
				game_array[0].paint_over(cur_block[0]);
			if (flag[1] < 3)
			{
				game_array[1].paint(true);
				cur_block[1].paint(true);
			}
			else
				game_array[1].paint_over(cur_block[1]);
			display_level(0,false);
			display_level(1,false);
			display_score(0,false);
			display_score(1,false);
			display_lines(0,false);
			display_lines(1,false);
			if (data.show_next[0])
				display_next(0,false);
			if (data.show_next[1])
				display_next(1,false);
			if (data.statistic)
				display_statistic(0,0,false);
		}
		else
		{
			g.setColor(Color.black);
			g.fillRect(0,0,500,380);
			g.setFont(main.msg_font);
			g.setColor(Color.white);
			g.drawString("Loading Images, please wait...",100, 160);
		}
	}

	protected void paint_square(Graphics g, int x, int y, int col)
	{
		if (col == 0)
			g.fillRect(x, y, 16, 16);
		else
			g.drawImage(main.square[col-1], x, y, main);
	}
}
