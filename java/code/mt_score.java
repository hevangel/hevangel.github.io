// Macross Tetris v1.0 by Horace Chan
// file : mt_score.java
// description : score class
import java.awt.*;
import java.applet.*;
import java.util.*;
import java.net.*;
import java.io.*;

public class mt_score
{			  
	protected mactetris main;
	
	protected info new_data;
	protected int flag, cur_pos;
	protected score_info score_data[] = new score_info[12];
	protected boolean input_name[] = {false, false};
	protected Font score_font;

	public mt_score(mactetris m)
	{
		int i = 0;
		main = m;
		for (i=0;i<12;i++)
			score_data[i] = new score_info(); 
		score_font = new Font("Dialog",Font.BOLD,14);
	}

	public void set_data(info d)
	{
		Graphics g;
		int i;

		g = main.getGraphics();
		g.setFont(main.msg_font);
		g.setColor(Color.white);
		g.drawString("Loading Data, please wait...",100, 160);
	
		flag = 0;
		for (i=0; i<12; i++)
		{
			score_data[i].name = "";
			score_data[i].level = 0;
			score_data[i].lines = 0;
			score_data[i].state = -1;	
		}

		new_data = d;

		score_data[10].name = new String("Left player");
		score_data[10].level = new_data.level[0];
		score_data[10].lines = new_data.lines[0];
		score_data[10].score = new_data.score[0];
		score_data[10].state = 0;				 
		score_data[11].name = new String("Right player");
		score_data[11].level = new_data.level[1];
		score_data[11].lines = new_data.lines[1];
		score_data[11].score = new_data.score[1];
		score_data[11].state = 0;

		read_database();
		sort_database();
		cur_pos = 0;
		for (i=0; i<10; i++)
			if (score_data[i].state != -1)
			{
				cur_pos = i;
				break;
			}
		if ((score_data[10].state != -1)&&(score_data[0].state != 1)) 
			flag = 2;
		else
			flag = 1;
		paint(g);		   
	}

	public void read_database()
	{
		int i=0;
		
		String readin, sztemp, szline;
		StringTokenizer st_readin, st_line;
		byte buf[] = new byte[5000];
		
		readin = new String("");
		
		try {
			URL url = new URL("http://www.csclub.uwaterloo.ca/~yhchan/cgi-bin/score.dat");
			DataInputStream s = new DataInputStream(url.openStream());
			while ((szline = s.readLine()) != null)
			{ readin = readin + szline + "\n"; }
			s.close();
		} catch (Exception e)
		{ System.out.println("File open error!"); }

//		for (i=0; i<10; i++)
//			readin += ".Horace Chan"+i+".0.0."+(10-i)*50+".\n";
//		System.out.println(readin);

		st_readin = new StringTokenizer(readin,"\n");
		for (i=0; i<10; i++)
		{
			try {
				szline = st_readin.nextToken();
				st_line = new StringTokenizer(szline, ".");
				score_data[i].name = st_line.nextToken();
				sztemp = st_line.nextToken();
				score_data[i].level = Integer.parseInt(sztemp);			
				sztemp = st_line.nextToken();
				score_data[i].lines = Integer.parseInt(sztemp);
				sztemp = st_line.nextToken();
				score_data[i].score = Integer.parseInt(sztemp);
			} catch (NoSuchElementException e)
			{ System.out.println("Database currupted"); }
		}
		System.out.println("Retriving data finished");

		System.gc();
 	}

	public void sort_database()
	{
		int i;
					 
		for (i=10; i>0; i--)
		{
			if (score_data[i].score > score_data[i-1].score)
				swap_data(score_data[i], score_data[i-1]); 
			else
				break;
		}

		for (i=11; i>0; i--)
		{
			if (score_data[i].score > score_data[i-1].score)
				swap_data(score_data[i], score_data[i-1]); 
			else
				break;
		}
	}

	public void swap_data(score_info a, score_info b)
	{
		String sztemp;
		int htemp;

		sztemp = a.name;
		a.name = b.name;
		b.name = sztemp;

		htemp = a.level;
		a.level = b.level;
		b.level = htemp;

		htemp = a.lines;
		a.lines = b.lines;
		b.lines = htemp;

		htemp = a.score;
		a.score = b.score;
		b.score = htemp;

		htemp = a.state;
		a.state = b.state;
		b.state = htemp;
	}

	public void write_database()
	{
		int i,j;

		StringBuffer sztemp = new StringBuffer("");
		String readin = "", inputLine = "";

		for (i=0; i<10; i++)
		{
			if (score_data[i].state == -1)
				continue;
			sztemp.setLength(0);			
			sztemp.append("place="+i+"&");
			sztemp.append("data="+score_data[i].name+"."
						+score_data[i].level+"."
						+score_data[i].lines+"."
						+score_data[i].score);

			for (j=0; j<sztemp.length(); j++)
				if (sztemp.charAt(j) == ' ')
					sztemp.setCharAt(j,'+');

//			System.out.println(sztemp.toString());
  			try
			{
				URL url = new URL("http://www.csclub.uwaterloo.ca/cgi-bin/cgiwrap/yhchan/score.cgi?"+sztemp.toString());
//				System.out.println(url);
				
				URLConnection urlConnection = url.openConnection();
				DataInputStream s = new DataInputStream(urlConnection.getInputStream());
				while ((inputLine = s.readLine()) != null)
				{ readin = readin + inputLine+"\n"; }
				s.close();
//				System.out.println(readin);
			}
			catch (Exception e)
			{ System.out.println("File writing error!"); }
		}
		main.ch_state(new_data, 1);
	}

	public void keyDown(int key)
	{
		if (flag == 1)
		{
			if (score_data[cur_pos].state == 0)
			{
				score_data[cur_pos].state = 1;
				score_data[cur_pos].name = new String("");
			}
			if (key == 10)
			{
				display_input(true,false);
				for (;cur_pos < 11;)
				{
					if (score_data[++cur_pos].state != -1)
					{
						display_input(true,true);
						break;
					}
					if (cur_pos == 10)
					{
						flag = 2;
						break;
					}
				}
			}
			else if (key == 8)
			{
				score_data[cur_pos].name = score_data[cur_pos].name.substring(0,score_data[cur_pos].name.length()-1);
				display_input(true,true);
			}
			else if ((score_data[cur_pos].name.length() < 20)&&
					(((key > 47)&&(key < 58))||
					 ((key > 40)&&(key < 91))|| 
					 ((key > 96)&&(key < 123))||
					 (key == 32)))
			{
				score_data[cur_pos].name += (char)key;
				display_input(true,true);
			}
		}
	}

	public void mouse_down(Event e, int x, int y)
	{
		Graphics g;

		if (flag == 2)
			if (is_inRect(x, y, 230, 330, 40, 22))
			{	
				g = main.getGraphics();
				g.drawImage(main.butok[1], 230, 330,main);
				g.setFont(main.msg_font);
				g.setColor(Color.cyan);
				g.drawString("Updating Data, please wait...",100, 160);
				write_database();
			}
	}

	public void display_input(boolean update, boolean check)
	{
		Graphics g;
		g = main.getGraphics();

		if (update)
		{
			g.clipRect(25,85+cur_pos*18,180,18);
			g.drawImage(main.score_bg, 0, 0, main);
		}

		if (check)
		{
			g.setColor(Color.red);
			g.fill3DRect(25, 85+cur_pos*18, 12, 12, true);
		}
		g.setFont(score_font);
		g.setColor(Color.yellow);
		g.drawString(score_data[cur_pos].name,45,95+cur_pos*18);
	}

	public void display_scores(Graphics g)
	{
		StringBuffer sztemp;
		String sztemp2;
		int i, j;

		g.setFont(score_font);
		for (i=0; i<10; i++)
		{
			if (score_data[i].state == -1)
				g.setColor(Color.white);
			else
				g.setColor(Color.yellow);
			
			g.drawString(score_data[i].name,45,95+i*18);
			sztemp = new StringBuffer(String.valueOf(score_data[i].level));
			while(sztemp.length() < 2)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(),230,95+i*18);
			sztemp = new StringBuffer(String.valueOf(score_data[i].lines));
			while(sztemp.length() < 3)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(),320,95+i*18);
			sztemp = new StringBuffer(String.valueOf(score_data[i].score));
			while(sztemp.length() < 5)
				sztemp.insert(0,'0');
			g.drawString(sztemp.toString(),395,95+i*18);
		}
		
		g.setColor(Color.yellow);
		g.drawString("Left player score: ", 100, 293);
		g.drawString("Right player score: ", 100, 311);
		sztemp2 = String.valueOf(new_data.score[0]);
		g.drawString(sztemp2, 300, 293);
		sztemp2 = String.valueOf(new_data.score[1]); 
		g.drawString(sztemp2, 300, 311);
	}

	public void	paint(Graphics g)
	{
		System.out.println("Paint score screen: "+flag);
		
		if ((main.ready > 2)&&(flag > 0))
		{	
			g.drawImage(main.score_bg, 0, 0, main);
			g.drawImage(main.butok[0], 230, 330,main);
			display_scores(g);
			if (flag == 1)
				display_input(false, true);
		}
		else
		{
			g.setColor(Color.black);
			g.fillRect(0,0,500,380);
			g.setFont(main.msg_font);
			g.setColor(Color.white);
			g.drawString("Loading Data, please wait...",100, 160);
		}
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
