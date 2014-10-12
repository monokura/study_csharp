using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

using Microsoft.Win32;


namespace Sankakuhi
{
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window
	{
		Random random = new Random();
		private int nameCounter = 0;
		private DispatcherTimer timer = null;
		private int counter = 0;

		private MediaPlayer mediaPlayer1 = new MediaPlayer();
		private MediaPlayer mediaPlayer2 = new MediaPlayer();

		private string previous_sankaku = "";
		private int previous_angle = -1;

		public MainWindow()
		{
			InitializeComponent();

			Uri uri1 = new Uri(@"../../res/sound/sound02.wav", UriKind.Relative);
			Uri uri2 = new Uri(@"../../res/sound/sound04.wav", UriKind.Relative);
			
			mediaPlayer1.Open(uri1);
			mediaPlayer2.Open(uri2);
			
			timer = new DispatcherTimer(DispatcherPriority.Normal, this.Dispatcher);
			timer.Interval = new TimeSpan(10000000);
			timer.Tick += new EventHandler(DispatcherTimer_Tick);
		}

		private void NextButton_Click(object sender, RoutedEventArgs e)
		{
			updateWindow();
		}

		private void updateWindow()
		{
			counter = 3;
			this.time.Content = counter.ToString();
			timer.Start();

			mediaPlayer1.Stop();
			mediaPlayer1.Play();

			answerer.Content = createName();
			question.Content = createQuestion();
		}

		private string createQuestion()
		{
			string sankaku = createSankaku();
			int angle = createAngle();

			while((angle == previous_angle) && (sankaku == previous_sankaku)){
				sankaku = createSankaku();
				angle = createAngle();	
			}

			previous_sankaku = sankaku;
			previous_angle = angle;

			return sankaku + angle.ToString() + "°";
		}

		private void DispatcherTimer_Tick(object sender, EventArgs e)
		{
			counter--;

			
			this.time.Content = counter.ToString();
			if (counter == 0)
			{
				timer.Stop();
				mediaPlayer2.Stop();
				mediaPlayer2.Play();
			}
			else
			{
				mediaPlayer1.Stop();
				mediaPlayer1.Play();
			}

		}

		private String createName()
		{
			//int randomNumber = random.Next(3); 
			String name = "";
			switch(nameCounter)
			{
				case 0: name = "くの"; break;
				case 1: name = "わたじゅん"; break;
				case 2: name = "こささ"; break;
				default: name = "error"; break;
				
			}
			nameCounter++;
			if(nameCounter >= 3){
				nameCounter = 0;
			}

			return name;
		}

		private string createSankaku()
		{
			int randomNumber = random.Next(3);
			String sankaku = "";
			switch(randomNumber){
				case 0: sankaku = "sin"; break;
				case 1: sankaku = "cos"; break;
				case 2:	sankaku = "tan"; break;
				default: sankaku = "error"; break;
			}
			return sankaku;
		}

		private int createAngle()
		{
			int randomNumber = random.Next(15);
			int angle = -1;
			switch (randomNumber)
			{
				case 0: angle = 0; break;
				case 1: angle = 30; break;
				case 2: angle = 30; break;
				case 3: angle = 45; break;
				case 4: angle = 45; break;
				case 5: angle = 60; break;
				case 6: angle = 60; break;
				case 7: angle = 90; break;
				case 8: angle = 120; break;
				case 9: angle = 120; break;
				case 10: angle = 135; break;
				case 11: angle = 135; break;
				case 12: angle = 150; break;
				case 13: angle = 150; break;
				case 14: angle = 0; break;
				default: break;
			}

			return angle;
		}

		private void Grid_KeyDown(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Enter)
			{
				updateWindow();
			} 
			else if (e.Key == Key.Escape)
			{
				Environment.Exit(0);
			}
		}

		private void MenuItem_end_Click(object sender, RoutedEventArgs e)
		{
			Environment.Exit(0);
		}

		private void MenuItem_option_Click(object sender, RoutedEventArgs e)
		{

		}

	}
}
