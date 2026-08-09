// Macross Tetris v1.0 by Horace Chan
// file : mactetris.java
// description : main program

import java.awt.*;
import java.applet.*;
import java.util.*;

public class mactetris extends Applet implements Runnable
{
	protected Thread main_thread = null;
	protected mt_menu menu_scr = null;
	protected mt_game game_scr = null;
	protected mt_score score_scr = null;
	protected mt_thread block_thread[] = new mt_thread[2];
	protected int ready;// 0 - not ready, 1 - menu ok, 2 - game ok, 3 -score ok
	protected int state;// 0 - not_ready, 1 - menu, 2 - game, 3 - score

	protected Font msg_font, scr_font;
// Menu Images
	protected Image menu_bg;
	protected Image face[] = new Image[4];
	protected Image opt[][] = new Image[5][2];
	protected Image but[][] = new Image[4][2];
	protected Image line_blocks;
	
// Game Images
	protected Image game_bg, game_bg_next;
	protected Image square[] = new Image[7];
	protected Image s_block[] = new Image[7];
	protected Image sigma;

// Score Images
	protected Image score_bg;
	protected Image butok[] = new Image[2];
		
	protected MediaTracker tracker[] = new MediaTracker[3];
	
	public void init()
	{
		int i;

		System.out.println("Macross Tetris v1.0 by Horace Chan");
		state = 1;		
		for (i=0; i<3; i++)
			tracker[i] = new MediaTracker(this);

		msg_font = new Font("Dialog",Font.BOLD,16);
		scr_font = new Font("Courier",Font.BOLD,14);
		menu_scr = new mt_menu(this);

		block_thread[0] = new mt_thread(0);
		block_thread[1] = new mt_thread(1);
		block_thread[0].set_menu(menu_scr);
		block_thread[1].set_menu(menu_scr);
		block_thread[0].set_state(1);
		block_thread[1].set_state(1);
	}

	public synchronized void ch_state(info data, int to_state)
	{
		Graphics g;
		switch(to_state)
		{
			case 1:
				state = to_state;
				menu_scr.reset();
				block_thread[0].set_level(1);
				block_thread[1].set_level(1);
				block_thread[0].set_state(1);
				block_thread[1].set_state(1);
				repaint();
			break;
			case 2:
				state = to_state;
				block_thread[0].set_state(0);
				block_thread[1].set_state(0);

				g = this.getGraphics();
				g.setFont(msg_font);		
				g.setColor(Color.white);
				g.drawString("Loading Images, please wait...",100, 160);
		
				if (game_scr == null)
				{
					System.out.println("Create game");
					game_scr = new mt_game(this);
					block_thread[0].set_game(game_scr);
					block_thread[1].set_game(game_scr);
					game_scr.reset(data);
				}
				else
					game_scr.reset(data);
			break;
			case 3:
				state = to_state;
				block_thread[0].set_state(0);
				block_thread[1].set_state(0);

				if (score_scr == null)
					score_scr = new mt_score(this);
				score_scr.set_data(data);
			break;
		}
	}

	public void start()
	{
		if (main_thread == null)
		{
			main_thread = new Thread(this);
			main_thread.start();
		}
	}

	public boolean mouseDown(Event e, int x, int y)
	{
		switch (state)
		{
			case 1:
				if (ready > 0)
					menu_scr.mouse_down(e, x, y);
			break;
			case 3:
				if (ready > 2)
					score_scr.mouse_down(e, x, y);
			break;
		}
		return true;
	}

	public boolean keyDown(Event e, int key)
	{
		switch (state)
		{
			case 2:
				if (ready > 1)
					game_scr.keyDown(key);
			break;
			case 3:
				if (ready > 2)
					score_scr.keyDown(key);
			break;
		}
		return true;
	}

	public void paint(Graphics g)
	{
		int i;

		switch (state)
		{
			case 1:
				menu_scr.paint(g);
			break;
			case 2:
				game_scr.draw_screen();
			break;
			case 3:
				score_scr.paint(g);
			break;
		}
	}

	public void run()
	{
		int i;
//Load menu images
		menu_bg = getImage(getDocumentBase(),"pic/menu_bg.jpg");
		tracker[0].addImage(menu_bg, 1);
		for (i=0; i<4; i++)
		{
			face[i] = getImage(getDocumentBase(), "pic/face"+(i+1)+".jpg");
			tracker[0].addImage(face[i], (i+2));
		}
		for (i=0; i<4; i++)
		{
			opt[i][0] = getImage(getDocumentBase(), "pic/opt"+(i+1)+"w.gif");
			opt[i][1] = getImage(getDocumentBase(), "pic/opt"+(i+1)+"c.gif");
			tracker[0].addImage(opt[i][0], (i*2+6));
			tracker[0].addImage(opt[i][1], (i*2+7));
		}
		for (i=0; i<4; i++)
		{
			but[i][0] = getImage(getDocumentBase(), "pic/but"+(i+1)+"w.gif");
			but[i][1] = getImage(getDocumentBase(), "pic/but"+(i+1)+"c.gif");
			tracker[0].addImage(but[i][0], (i*2+16));
			tracker[0].addImage(but[i][1], (i*2+17));
		}
		line_blocks = getImage(getDocumentBase(), "pic/line_blocks.gif");
		tracker[0].addImage(line_blocks, 26);
		s_block[0] = getImage(getDocumentBase(), "pic/block1.gif");
		tracker[0].addImage(s_block[0], 27);
		opt[4][0] = getImage(getDocumentBase(), "pic/opt5c.gif");
		tracker[0].addImage(opt[4][0], 28);
		opt[4][1] = getImage(getDocumentBase(), "pic/opt6c.gif");
		tracker[0].addImage(opt[4][1], 29);

		try { tracker[0].waitForAll(); }
		catch (InterruptedException e)
		{
			showStatus("Error! : image failed to load");
			return;
		}

		ready++;
		repaint();
		block_thread[0].start();
		block_thread[1].start();

		System.out.println("Load menu image finished");		
		
//Load game images
		game_bg = getImage(getDocumentBase(),"pic/level1_bg.jpg");
		tracker[1].addImage(game_bg, 1);
		sigma = getImage(getDocumentBase(),"pic/sigma.gif");
		tracker[1].addImage(sigma, 2);
		for (i=0; i<7; i++)
		{
			square[i] = getImage(getDocumentBase(),"pic/square"+(i+1)+".gif");
			tracker[1].addImage(square[i], (i+3));
		}
		for (i=1; i<7; i++)
		{
			s_block[i] = getImage(getDocumentBase(),"pic/block"+(i+1)+".gif");
			tracker[1].addImage(s_block[i], (i+9));
		}
				
		try { tracker[1].waitForAll(); }
		catch (InterruptedException e)
		{
			showStatus("Error loading image!");
			return;
		}
		ready++;
		if ((state == 2) && (game_scr != null))
		{	
			repaint();
			block_thread[0].set_state(2);
			block_thread[1].set_state(2);
		}
		System.out.println("Load game image finished");		
		
	//Load score images
		score_bg = getImage(getDocumentBase(),"pic/score_bg.jpg");
		tracker[2].addImage(score_bg, 1);
		butok[0] = getImage(getDocumentBase(),"pic/butokw.gif");
		tracker[2].addImage(butok[0], 2);
		butok[1] = getImage(getDocumentBase(),"pic/butokc.gif");
		tracker[2].addImage(butok[1], 2);
		
		try { tracker[2].waitForAll(); }
		catch (InterruptedException e)
		{
			showStatus("Error loading image!");
			return;
		}
	
		ready++;
		if ((state == 3) && (score_scr != null))
			repaint();
		System.out.println("Load screen image finished");		
	}
}

