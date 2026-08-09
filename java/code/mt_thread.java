// Macross Tetris v1.0 by Horace Chan
// file : mt_thread.java
// description : thread class

public class mt_thread extends Thread
{
	mt_menu menu;
	mt_game game;
	protected int side;	// 0 - left, 1 - right
	protected int state = 0;
	protected int level = 1;

	public mt_thread(int s)
	{ side = s;	}

 	public synchronized void set_menu(mt_menu m)
	{ menu = m; }

	public synchronized void set_game(mt_game g)
	{ game = g; }
	
	public synchronized void set_state(int s)
	{ state = s; }

	public synchronized void set_level(int l)
	{ level = l; }

	public void run()
	{
		while (true)
		{
			switch (state)
			{
				case 1:
					menu.drop_block(side);
				break;
				case 2:
					game.drop_block(side);
				break;
				default:
				break;
			}

			try { sleep((12-level)*80); }
			catch (InterruptedException e) {};
		}
	}
}

