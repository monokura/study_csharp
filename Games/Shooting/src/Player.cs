using System;
using DxLibDLL;

namespace Shooting{
class Player : Character{
	private int count;	// カウンタ
	private int speed;	// 移動量
	private int power;	// パワー
	private int point;	// ポイント
	private int score;	// スコア
	private int num;	// 残機数
	private int mutelicnt; // 無敵状態とカウント
	private int muney;	// お金
	
	public Player() : this(0.0 ,0.0){}

	public Player(double x, double y){
		/** MEMO :
		* LoadDivGraph( char *FileName , int AllNum , int XNum , int YNum ,int XSize , int YSize , int *HandleBuf );
		* FileName 		:　分割読み込みする画像ファイル文字列のポインタ
		* AllNum		:　画像の分割総数
		* XNum ,YNum	:　画像の横向きに対する分割数と縦に対する分割数
		* SizeX ,SizeY　	:　分割された画像一つの大きさ
		* HandleBuf		:　分割読み込みして得たグラフィックハンドルを保存するint型の配列へのポインタ
		* (例)
		* int gh[12];
		* LoadDivGraph("hoge.jpg", 12, 3, 4, 30, 40, gh );
		* 画像[hoge.jpg]が横30px,縦40pxで横に3段,縦に4段で12分割される
		* 格納されるグラフィックハンドラ配列の中身は
		* | 0  1  2|
		* | 3  4  5|
		* | 6  7  8|
		* |10 11 12|
	 	*/

	 	this.posX = x;
		this.posY = y;

	 	this.width = 73;
	 	this.height = 73;
	 	this.ghIndex = 0;
	 	this.speed = 4;
	 	this.count = 0;

	 	int xNum = 4;
	 	int yNum = 3;
	 	int allNum = xNum*yNum;
	 	int xSize = 73;
	 	int ySize = 73;

	 	gh = new int[allNum];
		const string imageStr = @"res/img/char/0.png";
		DX.LoadDivGraph(imageStr, allNum, xNum, yNum, xSize, ySize, out gh[0]);
	}

	public int Count{
		get{ return this.count; }
		set{ this.count = value;}
	}

	public int Speed{
		get{ return this.speed; }
		set{ this.speed = value;}
	}
}
} // namespace Shooting