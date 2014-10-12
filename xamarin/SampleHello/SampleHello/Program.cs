using System;
using System.Drawing;
using System.Windows.Forms;

class Program
{
	static void Main ()
	{
		Application.Run (new MyForm ());
	}
}

class MyForm : Form
{
	Button button1;
	Label label;

	public MyForm()
	{
		this.Width = 800;
		this.Height = 600;
		this.Text = "sample";

		this.label = new Label();
		this.label.Text = "三角比";
		this.label.Font = new Font("MS ゴシック",100,FontStyle.Regular);
		this.label.Height = 300;
		this.label.Width = 800;

		this.button1 = new Button ();
		this.button1.Location = new Point (600, 500);
		this.button1.Size = new Size (170, 30);
		this.button1.Text = "次";
		this.button1.Click += new EventHandler (this.Button1Click);

		this.Controls.Add (this.button1);
		this.Controls.Add (label);
	}

	void Button1Click(object sender, EventArgs e)
	{
		Random rnd = new Random();
		int randomNumber1 = rnd.Next(3);
		String sankaku ="";
		switch(randomNumber1){
		case 0:
			sankaku = "sin";
			break;
		case 1:
			sankaku = "cos";
			break;
		case 2:
			sankaku = "tan";
			break;
		default:
			break;
		}

		int randomNumber2 = rnd.Next (9);
		int angle = -1;
		switch(randomNumber2){
		case 0:
			angle = 0;
			break;
		case 1:
			angle = 30;
			break;
		case 2:
			angle = 45;
			break;
		case 3:
			angle = 60;
			break;
		case 4:
			angle = 90;
			break;
		case 5:
			angle = 120;
			break;
		case 6:
			angle = 135;
			break;
		case 7:
			angle = 150;
			break;
		case 8:
			angle = 180;
			break;
		default:
			break;
		}

		this.label.Text = sankaku + angle.ToString() + "°";
	}
}