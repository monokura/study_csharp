
namespace Sample{
	delegate void UpdateEventHandler();

	class Document{
		public event UpdateEventHandler Updated;

		public void Update(){
			Updated();
		}
	}

	class TextView{
		public void Notify(){
			System.Console.WriteLine("TextView");
		}
	}

	class GraphView{
		public void Notify(){
			System.Console.WriteLine("GraphView");
		}
	}

	class MyApp{
		public static void Main(){
			Document doc = new Document();
			TextView textView = new TextView();
			GraphView graphView = new GraphView();

			doc.Updated += new UpdateEventHandler(textView.Notify);	// デリゲート一つ目を登録
			doc.Updated += new UpdateEventHandler(graphView.Notify); // デリゲート２つ目を登録
		
			System.Console.WriteLine("Start...");
			doc.Update();
		}
	}
}