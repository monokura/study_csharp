using System;
using DxLibDLL;
using System.Collections.Generic;

namespace Shooting{
class Enemy : Character{
	private bool isDeath;		// 生きているか否か
	private bool haveShots;		// 弾丸が存在するか否か
	private int count;		//
	private int pattern;	//
	private int muki;		//
	private int type;		//
	private int HP;
	private int HPMax;		//
	private int[] item;	//

	// 移動系
	private double vx;		// 
	private double vy;		// 
	private double speed;	// 
	private double angle;	// 

	private List<Shot> shotList = new List<Shot>();
	private int shotTime;	// 弾幕開始時間
	private int shotType;	// 弾幕の種類
	private int bulletType;	// 弾の種類
	private int bulletColor;// 色
	private int state;		// 状態
	private int readyTime;	// 待機時間
	private int waitTime;		// 停滞時間

	private int id;

	public Enemy(EnemyParam ep, int id){
		gh = new int[9];
		const string imageStr = @"res/img/enemy/0.png";
		DX.LoadDivGraph(imageStr , 9, 3, 3, 32, 32, out gh[0]);

		isDeath	= false;
		haveShots = false;
		count		= 0;
		pattern = ep.pattern;
		muki	= ep.muki;
		type	= ep.type;
		HP		= ep.HP;
		HPMax	= HP;

		posX	= ep.posX;
		posY	= ep.posY;
		vx 		= ep.vx;
		vy 		= ep.vy;
		speed 	= ep.speed;
		angle 	= ep.angle;

		shotTime	= ep.shotTime;
		shotType	= ep.shotType;
		bulletType  = ep.bulletType;
		bulletColor	= ep.color;
		readyTime	= ep.readyTime;
		waitTime	= ep.waitTime;
		
		item = new int[6];
		for(int i = 0;i < 6;i++){
			item[i] = ep.item[i];
		}

		this.id = id;
	}

	~Enemy(){
	}

	public void Move(){
		switch(pattern){
			case 0: MovePattern00(); break;
			case 1: MovePattern01(); break;
			case 2: MovePattern02(); break;
			case 3: MovePattern03(); break;
			case 4: MovePattern04(); break;
			case 5: MovePattern05(); break;
			case 6: MovePattern06(); break;
			case 7: MovePattern07(); break;
			case 8: MovePattern08(); break;
			case 9: MovePattern09(); break;
			case 10: MovePattern10(); break;
			default: break;
		}

		this.posX += this.vx;
		this.posY += this.vy;

		ghIndex = muki*3 + count%18/6;

		//敵が画面から外れたら消す
		if(posX < -50 || 384+50 < posX || posY < -50 || 448+50 < posY){
			isDeath = true;
			Console.WriteLine("Death : Enemy id " + this.id);
		}
	}

	public void RegistShot(){
		if(this.shotTime == this.count){
			shotList.Add(new Shot(this));
			haveShots = true;
			Console.WriteLine("Shot! : " + this.id);
		}
	}

	public void UpdateShot(Player p){
		foreach(Shot s in shotList){
			s.Update(this, p);
		}

		shotList.RemoveAll(s => !s.IsExist);

		if(shotList.Count == 0){
			haveShots = false;
		}
	}

	public void DrawShot(int originX, int originY){
		foreach(Shot s in shotList){
			s.Draw(originX, originY);
		}
	}

	// ============ setter,getter郡 ===================

	public bool IsDeath{
		get{ return isDeath;}
	}

	public bool HaveShots{
		get{ return haveShots;}
	}

	public int ID{
		get{ return this.id;}
	}

	public int Count{
		get{ return this.count; }
		set{ this.count = value;}
	}

	public int ShotTime{
		get{ return this.shotTime;}
	}

	public int ShotType{
		get{ return this.shotType;}
	}

	public int BulletType{
		get{ return this.bulletType;}
	}

	public int BulletColor{
		get{ return this.bulletColor;}
	}

	// ============== ここからMovePattern =================

	private void MovePattern00(){
		// 下がって停止して上がる
		if(this.count == 0){
			this.vy = 3; // 下がる
		}
		if(this.count == 40){
			this.vy = 0; // 停止
		}
		if(this.count == 40+this.waitTime){
			this.vy = -3; // 待機時間停止して上がる
		}
	}

	private void MovePattern01(){
		// 下がって停止して左下へ
		if(this.count == 0){
			this.vy = 3; // 下がる
		}
		if(this.count == 40){
			this.vy = 0; // 停止
		}
		if(this.count == 40+this.waitTime){
			//待機時間停止して左下・左向き
			this.vx = -1;
			this.vy = 2;
			this.muki = 0;
		}
	}
	private void MovePattern02(){
		// 下がって停止して右下へ
		if(this.count == 0){
			this.vy = 3; // 下がる
		}
		if(this.count == 40){
			this.vy = 0; // 停止
		}
		if(this.count == 40+this.waitTime){
			//待機時間停止して右下・右向き
			this.vx = 1;
			this.vy = 2;
			this.muki = 2;
		}
	}
	private void MovePattern03(){
		// 素早く降りて左へ
		if(this.count == 0){
			this.vy = 5; // 下がる
		}
		if(this.count == 30){
			this.muki = 0; // 停止
		}
		if(this.count < 100){
			this.vx -= 5/100.0; // 左向き加速
			this.vy -= 5/100.0; // 減速
		}
	}
	private void MovePattern04(){
		// 素早く降りて右へ
		if(this.count == 0){
			this.vy = 5; // 下がる
		}
		if(this.count == 30){
			this.muki = 2; // 停止
		}
		if(this.count < 100){
			this.vx += 5/100.0; // 右向き加速
			this.vy -= 5/100.0; // 減速
		}
	}
	private void MovePattern05(){
		// 斜め左下へ
		if(this.count == 0){
			this.vx -= 1;
			this.vy = 2;
			this.muki = 0;
		}
	}
	private void MovePattern06(){
		// 斜め右下へ
		if(this.count == 0){
			this.vx += 1;
			this.vy = 2;
			this.muki = 2;
		}
	}
	private void MovePattern07(){
		// 停滞してそのまま左上に
		if(this.count == this.waitTime){
			this.vx = -0.7;
			this.vy = -0.3;
			this.muki = 0;
		}
	}
	private void MovePattern08(){
		// 停滞してそのまま右上に
		if(this.count == this.waitTime){
			this.vx = 0.7;
			this.vy = -0.3;
			this.muki = 2;
		}
	}
	private void MovePattern09(){
		// そのまま降りていく
		if(this.count == this.waitTime){
			this.vy = -1;
		}
	}

	private void MovePattern10(){
		// 下がってきてウロウロして上がっていく
		if(this.count == 0){
			this.vy = 4;
		}
		if(this.count == 40){
			this.vy = 0;
		}
		if(this.count >= 40){
			if(this.count %60 == 0){
				int r = Math.Cos(this.angle) < 0 ? 0 : 1;
				this.speed = 6 + rang(2);
				this.angle = rang(Math.PI/4) + Math.PI*r;
				this.muki = 2 - 2*r;
			}
			this.speed *= 0.95;
			// 角度と速度からvx,vyを生成
			this.vx = Math.Cos(this.angle) * this.speed;
			this.vy = Math.Sin(this.angle) * this.speed;

		}
		if(this.count >= 40 + this.waitTime){
			this.vy -= 0.05 * (this.count - 40 -this.waitTime);
		}


	}

	private double rang(double angle){
		return (-angle + angle*2 * DX.GetRand(10000)/10000.0);
	}

}
} // namespace Shooting