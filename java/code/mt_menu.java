// Macross Tetris v1.0 by Horace Chan
// file : mt_menu.java
// description : menu class

import java.awt.*;
import java.applet.*;
import java.util.*;

public class mt_menu
{
	protected mactetris main;
 	protected info data;
	protected int block_pos[] = {20,20};
	
	public mt_menu(mactetris m) 
	{
		int i;
		main = m;
		
		data = new info();
	}

	public void reset()
	{
		data.score[0] = data.score[1] = 0;
		data.lines[0] = data.lines[1] = 0;
		data.level[0] = data.level[1] = 1;
	}

	public void paint(Graphics g)
	{
		int temp;

		if (main.ready > 0)
		{
		 	g.drawImage(main.menu_bg, 0, 0, main);
			g.drawImage(main.face[data.player[0]],230,5,main);
			g.drawImage(main.face[data.player[1]],340,5,main);

			(data.sound) ? temp = 1: temp = 0;
				g.drawImage(main.opt[0][temp],5,70,main);
			(data.statistic) ? temp = 1: temp = 0;
				g.drawImage(main.opt[1][temp],5,90,main);
			(data.send_rocks) ? temp = 1: temp = 0;
				g.drawImage(main.opt[2][temp],5,110,main);
			(data.same_pieces) ? temp = 1: temp = 0;
				g.drawImage(main.opt[3][temp],5,130,main);
			
			(data.slow_down[0]) ? temp = 1: temp = 0;
				g.drawImage(main.but[0][temp],276,80,main);
			(data.slow_down[1]) ? temp = 1: temp = 0;
				g.drawImage(main.but[0][temp],320,80,main);
			(data.show_next[0]) ? temp = 1: temp = 0;
				g.drawImage(main.but[1][temp],272,110,main);
			(data.show_next[1]) ? temp = 1: temp = 0;
				g.drawImage(main.but[1][temp],316,110,main);

			g.setColor(Color.red);
			g.drawRect(284,(167+17*(9-data.level[0])),12,12);
			g.drawRect(328,(167+17*(9-data.level[1])),12,12);

			g.drawImage(main.but[3][0],267,153,main);
			g.drawImage(main.but[3][0],267,225,main);
			g.drawImage(main.but[3][0],267,261,main);
			g.drawImage(main.but[3][0],267,309,main);
			g.drawImage(main.but[3][1],267,(153+12*(13-data.start_line[0])),main);

			g.drawImage(main.but[2][0],350,153,main);
			g.drawImage(main.but[2][0],350,225,main);
			g.drawImage(main.but[2][0],350,261,main);
			g.drawImage(main.but[2][0],350,309,main);
			g.drawImage(main.but[2][1],350,(153+12*(13-data.start_line[1])),main);
			
			g.clipRect(143, (157+12*(13-data.start_line[0])), 120, 12*data.start_line[0]);
			g.drawImage(main.line_blocks, 143, (157+12*(13-data.start_line[0])), main);

			g = main.getGraphics();
			g.clipRect(363, (157+12*(13-data.start_line[1])), 120, 12*data.start_line[1]);
			g.drawImage(main.line_blocks, 363, (157+12*(13-data.start_line[1])), main);

		} 
		else
		{
			g.setColor(Color.black);
			g.fillRect(0,0,500,380);
			g.setColor(Color.white);
			g.setFont(main.msg_font);
			g.drawString("Macross Tetris v1.0 - by Horace Chan",50, 200);
			g.drawString("Loading Images, please wait...",50, 215);
		}
	}

	public synchronized boolean mouse_down(Event e, int x, int y)
	{
		Graphics g;
		int temp;
		
		g = main.getGraphics();
		
		if (is_inRect(x,y,230,5,48,48))
		{
			data.player[0]++;
			if (data.player[0] > 3)
				data.player[0] = 0;
			g.drawImage(main.face[data.player[0]],230,5,main);
		}
		
		else if (is_inRect(x,y,340,5,48,48))
		{	 
			data.player[1]++;
			if (data.player[1] > 3)
				data.player[1] = 0;
			g.drawImage(main.face[data.player[1]],340,5,main);
		}

		else if (is_inRect(x,y,5,70,108,16))
		{
			data.sound = !data.sound;
			(data.sound) ? temp = 1: temp = 0;
				g.drawImage(main.opt[0][temp],5,70,main);
		}

		else if (is_inRect(x,y,5,90,126,16))
		{
			data.statistic = !data.statistic;
			(data.statistic) ? temp = 1: temp = 0;
				g.drawImage(main.opt[1][temp],5,90,main);
		}

		else if (is_inRect(x,y,5,110,108,16))
		{
			data.send_rocks = !data.send_rocks;
			(data.send_rocks) ? temp = 1: temp = 0;
				g.drawImage(main.opt[2][temp],5,110,main);
		}

		else if (is_inRect(x,y,5,130,107,16))
		{
			data.same_pieces = !data.same_pieces;
			(data.same_pieces) ? temp = 1: temp = 0;
				g.drawImage(main.opt[3][temp],5,130,main);
		}

		else if (is_inRect(x,y,276,80,27,24))
		{
			data.slow_down[0] = !data.slow_down[0];
			(data.slow_down[0]) ? temp = 1: temp = 0;
				g.drawImage(main.but[0][temp],276,80,main);
		}

		else if (is_inRect(x,y,320,80,27,25))
		{
			data.slow_down[1] = !data.slow_down[1];
			(data.slow_down[1]) ? temp = 1: temp = 0;
				g.drawImage(main.but[0][temp],320,80,main);
		}

		else if (is_inRect(x,y,272,110,35,21))
		{
			data.show_next[0] = !data.show_next[0];
			(data.show_next[0]) ? temp = 1: temp = 0;
				g.drawImage(main.but[1][temp],272,110,main);
		}

		else if (is_inRect(x,y,316,110,35,21))
		{
			data.show_next[1] = !data.show_next[1];
			(data.show_next[1]) ? temp = 1: temp = 0;
				g.drawImage(main.but[1][temp],316,110,main);
		}

		else if (is_inRect(x,y,284,167,12,153))
		{								
			data.level[0] = 9 - (y-167)/17;
			g.clipRect(284,167,14,153);
			g.drawImage(main.menu_bg, 0, 0, main);

			g = main.getGraphics();
			g.setColor(Color.red);
			g.drawRect(284,(167+17*(9-data.level[0])),12,12);

			main.block_thread[0].set_level(data.level[0]);
		}

		else if (is_inRect(x,y,328,167,12,153))
		{
			data.level[1] = 9 - (y-167)/17;
			g.clipRect(328,167,14,153);
			g.drawImage(main.menu_bg, 0, 0, main);
			
			g = main.getGraphics();
			g.setColor(Color.red);
			g.drawRect(328,(167+17*(9-data.level[1])),12,12);

			main.block_thread[1].set_level(data.level[1]);
		}

		else if (is_inRect(x,y,267,153,10,168))
		{
			g.drawImage(main.but[3][0],267,(153+12*(13-data.start_line[0])),main);
			g.clipRect(143, 73, 120, 240);
			g.drawImage(main.menu_bg, 0, 0, main);
			if (is_inRect(x,y,267,153,10,10))
				data.start_line[0] = 13;
			else if (is_inRect(x,y,267,225,10,10))
				data.start_line[0] = 7;
			else if (is_inRect(x,y,267,261,10,10))
				data.start_line[0] = 4;
			else if (is_inRect(x,y,267,309,10,10))
				data.start_line[0] = 0;
						
			g = main.getGraphics();
			g.drawImage(main.but[3][1],267,(153+12*(13-data.start_line[0])),main);
			g.clipRect(143, (157+12*(13-data.start_line[0])), 120, 12*data.start_line[0]);
			g.drawImage(main.line_blocks, 143, (157+12*(13-data.start_line[0])), main);
			block_pos[0] = 20;
		}
			
		else if (is_inRect(x,y,350,153,10,168))
		{
			g.drawImage(main.but[2][0],350,(153+12*(13-data.start_line[1])),main);
			g.clipRect(363, 73, 120, 240);
			g.drawImage(main.menu_bg, 0, 0, main);
			if (is_inRect(x,y,350,153,10,10))
				data.start_line[1] = 13;
			else if (is_inRect(x,y,350,225,10,10))
				data.start_line[1] = 7;
			else if (is_inRect(x,y,350,261,10,10))
				data.start_line[1] = 4;
			else if (is_inRect(x,y,350,309,10,10))
				data.start_line[1] = 0;
						
			g = main.getGraphics();
			g.drawImage(main.but[2][1],350,(153+12*(13-data.start_line[1])),main);
			g.clipRect(363, (157+12*(13-data.start_line[1])), 120, 12*data.start_line[1]);
			g.drawImage(main.line_blocks, 363, (157+12*(13-data.start_line[1])), main);
			block_pos[1] = 20;
		}

		else if (is_inRect(x,y,105,335,129,19))
		{
			g.drawImage(main.opt[4][0],105,335,main);
			main.ch_state(data, 2);
		}

		else if (is_inRect(x,y,280,335,134,25))
		{
			g.drawImage(main.opt[4][1],280,335,main);
			main.ch_state(data, 3);
		}

		return true;
	}

	protected synchronized void drop_block(int side)
	{
		Graphics g;
		int start_pos;
		
		(side == 0) ? start_pos = 191 : start_pos = 411;
		
		g = main.getGraphics();
		g.clipRect(start_pos, 73+12*(20-block_pos[side]), 48, 24);
		g.drawImage(main.menu_bg, 0, 0, main);
		
		block_pos[side]--;
		if (block_pos[side] <= data.start_line[side] +1)
			block_pos[side] = 20;
			
		g = main.getGraphics();
		g.drawImage(main.s_block[0], start_pos, 73+12*(20-block_pos[side]), 48, 24, main);
		return;
	}
	
	protected boolean is_inRect(int x1, int y1, int x, int y, int w, int h)
	{
		if (
				(x1 >= x) && (x1 <= x+w) &&
				(y1 >= y) && (y1 <= y+h)
			)
			return true;
		else
			return false;
	}

}
