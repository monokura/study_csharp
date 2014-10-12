using System;
using DxLibDLL;

namespace Shooting{
class Bullet{
	private bool isExist;
	private int type;
	private int count;
	private int color;
	private int state;
	private int till;
	private int effect;
	private double posX;
	private double posY;
	private double angle;
	private double speed;
	private double baseAngle;
	private double remSpeed;

	private int ghIndex;
	private int[][] gh;

	public Bullet(){
		this.type = 0;
		this.angle = 0;
		this.isExist = true;
		this.posX = 0;
		this.posY = 0;
		this.color = 0;
		this.count = 0;
		this.speed = 1;
		InitGh();
	}

	public Bullet(Enemy e){
		this.type = e.BulletType;
		this.angle = 0;
		this.isExist = true;
		this.posX = e.PosX;
		this.posY = e.PosY;
		this.color = e.BulletColor;
		this.count = 0;
		this.speed = 1;
		InitGh();
	}

	public void InitGh(){
		gh = new int[10][];
		gh[0] = new int[5];
		gh[1] = new int[6];
		gh[2] = new int[10];
		gh[3] = new int[5];
		gh[4] = new int[10];
		gh[5] = new int[3];
		gh[6] = new int[3];
		gh[7] = new int[9];
		gh[8] = new int[10];
		gh[9] = new int[3];
		DX.LoadDivGraph(@"res/img/bullet/b0.png" , 5 , 5 , 1 , 76 , 76 ,out gh[0][0]);
		DX.LoadDivGraph(@"res/img/bullet/b1.png" , 6 , 6 , 1 , 22 , 22 ,out gh[1][0]);
		DX.LoadDivGraph(@"res/img/bullet/b2.png" ,10 ,10 , 1 ,  5 ,120 ,out gh[2][0]);
		DX.LoadDivGraph(@"res/img/bullet/b3.png" , 5 , 5 , 1 , 19 , 34 ,out gh[3][0]);
		DX.LoadDivGraph(@"res/img/bullet/b4.png" ,10 ,10 , 1 , 38 , 38 ,out gh[4][0]);
		DX.LoadDivGraph(@"res/img/bullet/b5.png" , 3 , 3 , 1 , 14 , 16 ,out gh[5][0]);
		DX.LoadDivGraph(@"res/img/bullet/b6.png" , 3 , 3 , 1 , 14 , 18 ,out gh[6][0]);
		DX.LoadDivGraph(@"res/img/bullet/b7.png" , 9 , 9 , 1 , 16 , 16 ,out gh[7][0]);
		DX.LoadDivGraph(@"res/img/bullet/b8.png" ,10 ,10 , 1 , 12 , 18 ,out gh[8][0]);
		DX.LoadDivGraph(@"res/img/bullet/b9.png" , 3 , 3 , 1 , 13 , 19 ,out gh[9][0]);		
	}

	public void Update(){
		this.posX += Math.Cos(this.angle) * this.speed;
		this.posY += Math.Sin(this.angle) * this.speed;

		// 消去判定
		if(this.till < this.count){
			if(this.posX < -50 || this.posX > 384 + 50){
				this.isExist = false;
			}

			if(this.posY < -50 || this.posY > 448 + 50){
				this.isExist = false;
			}
		}

		this.count++;
	}

	public void Draw(int originX, int originY){
		if(this.effect == 1){
			DX.SetDrawBlendMode(DX.DX_BLENDMODE_ADD, 255);
		}

		DX.DrawRotaGraphF(
			(int)(originX + this.posX),
			(int)(originY + this.posY),
			1.0f, this.angle + Math.PI/2,
			(int)this.gh[this.type][this.color], DX.TRUE
		);

		if(this.effect == 1){
			DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, 0);
		}


	}

	public double PosX{
		get{ return this.posX; }
		set{ this.posX = value;}
	}

	public double PosY{
		get{ return this.posY; }
		set{ this.posY = value;}
	}

	public int Type{
		get{ return this.type; }
		set{ this.type = value;}
	}

	public int Color{
		get{ return this.color; }
		set{ this.color = value;}
	}

	public bool IsExist{
		get{ return this.isExist;}
	}


	public double Speed{
		get{ return this.speed; }
		set{ this.speed = value;}
	}

	public double Angle{
		get{ return this.angle; }
		set{ this.angle = value;}
	}
}
} // namespace Shooting