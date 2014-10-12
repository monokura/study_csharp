using System;
using DxLibDLL;

using System.Collections.Generic;

namespace Shooting{
class Shot{
	private bool isExist;
	private bool isEnemyExist;
	private int type;
	private int count;
	private int enemyID;

	private double baseAngle;
	private double baseSpeed;

	private int seHundle;

	List<Bullet> bulletList = new List<Bullet>();
	Bullet[] bulletArray = new Bullet[1000];

	public Shot(Enemy e){
		this.isExist = true;
		this.isEnemyExist = true;
		this.type = e.ShotType;
		this.enemyID = e.ID;
		this.count = 0;

		seHundle = DX.LoadSoundMem(@"res/se/enemy_shot.wav");
	}
 
	public void Update(Enemy e, Player p){
		if(e.IsDeath){
			this.isEnemyExist = false; // コレ以上弾丸を追加不可
		}

		switch(this.type){
			case 0: ShotPattern00(e, p); break;
			case 1: ShotPattern01(e, p); break;
			case 2: ShotPattern02(e, p); break;
			case 3: ShotPattern03(e, p); break;
			case 4: ShotPattern04(e, p); break;
			case 5: ShotPattern05(e, p); break;
			case 6: ShotPattern06(e, p); break;
			default: break;
		}

		foreach(Bullet b in bulletList){
			b.Update();
		}

		// 弾幕の消去処理
		bulletList.RemoveAll(b => !b.IsExist);

		if(bulletList.Count == 0){
			//this.isExist = false;
		}

		this.count++;
	}

	public void Draw(int originX, int originY){
		foreach(Bullet b in bulletList){
			b.Draw(originX, originY);
		}
	}

	private void ShotPattern00(Enemy e, Player p){
		// 一発だけ、自機に向かって直線移動
		if(this.count == 0){
			if(isEnemyExist){
				Bullet b = new Bullet(e);
				b.Angle = ShotAtan2(e, p);
				b.Speed = 3;
				bulletList.Add(b);
				DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
			}
		}
	}

	private void ShotPattern01(Enemy e, Player p){
		// 100カウント中に10発、自機に向かって直線発射（常に自機狙い）
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count <= 100){
				if((this.count % 10) == 0){
					Bullet b = new Bullet(e);
					b.Angle = ShotAtan2(e, p);
					b.Speed = 3;
					bulletList.Add(b);
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
				}
			}
		}
	}

	private void ShotPattern02(Enemy e, Player p){
		// 100カウント中に10発、自機に向かって直線発射（角度記憶）
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count <= 100){
				if((this.count % 10) == 0){
					if(this.count == 0){
						this.baseAngle = ShotAtan2(e, p);
					}
					Bullet b = new Bullet(e);
					b.Angle = this.baseAngle;
					b.Speed = 3;
					bulletList.Add(b);
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
				}
			}
		}
	}

	private void ShotPattern03(Enemy e, Player p){
		// 100カウント中に10発、自機に向かってスピード変化直線発射
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count <= 100){
				if((this.count % 10) == 0){
					Bullet b = new Bullet(e);
					b.Angle = ShotAtan2(e, p);
					b.Speed = 1+ 5.0/100 * this.count;
					bulletList.Add(b);
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
				}
			}

		}
	}

	private void ShotPattern04(Enemy e, Player p){
		// 0.5秒に1回ずつ円形に発射
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count < 120){
				if((this.count % 20) == 0){
					double angle = ShotAtan2(e, p);
					int shotNum = 20;
						
					for(int i = 0;i < shotNum;i++){
						Bullet b = new Bullet();					
						b.PosX = e.PosX;
						b.PosY = e.PosY;
						b.Color = e.BulletColor;
						b.Type = e.BulletType;
						b.Angle = angle + Math.PI * 2 / shotNum * i;
						b.Speed = 4;
						bulletList.Add(b);
					}
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);	
				}
			}
		}
	}

	private void ShotPattern05(Enemy e, Player p){
		// ばらまきショット
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count < 120){
				if((this.count % 2) == 0){
					Bullet b = new Bullet(e);
					b.Angle = ShotAtan2(e, p) + Rang(Math.PI/4);
					b.Speed = 3+Rang(1.5);
					bulletList.Add(b);
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
				}
			}
		}	
	}

	private void ShotPattern06(Enemy e, Player p){
		// ばらまきショット(減速)
		if(this.isEnemyExist){
			if(this.count >= 0 && this.count < 120){
				if((this.count % 2) == 0){
					Bullet b = new Bullet(e);
					b.Angle = ShotAtan2(e, p) + Rang(Math.PI/4);
					b.Speed = 4+Rang(2);
					bulletList.Add(b);
					DX.PlaySoundMem(seHundle,DX.DX_PLAYTYPE_BACK);
				}
			}
		}
		foreach(Bullet b in bulletList){
			if(b.Speed > 1.5){
				b.Speed -= 0.04;
			}
		}
	}

	private double ShotAtan2(Enemy e, Player p){
		return Math.Atan2(p.PosY - e.PosY, p.PosX - e.PosX);
	}

	public bool IsExist{
		get{ return this.isExist;}
	}

	private double Rang(double angle){
		return (-angle + angle*2 * DX.GetRand(10000)/10000.0);
	}

}
} // namespace Shooting