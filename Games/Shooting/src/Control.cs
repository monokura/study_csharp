using System;
using System.IO;
using System.Collections.Generic;
using DxLibDLL;


namespace Shooting{
class Control{
	private	Player player;
	private List<Enemy> enemyList = new List<Enemy>();
	private List<EnemyParam> enemyParamList =  new List<EnemyParam>(); 
	private Field field = new Field();
	private Frame frame = new Frame();
	private int stageCounter;	// 経過時間をカウント
	private int enemyCounter;	// 敵のユニークIDをつくるためのカウンタ

	// 弾丸系の定数
	private const int ShotBulletMax = 1000;	// 敵1匹が持つ弾丸の最大数
	private const int ShotMax = 30;			// 画面に表示できる敵の弾丸の最大数
	private const int ShotTypeMax = 1;		// ショットの種類の最大数
	private const int SeMax = 100;			// 効果音の最大数
	private const int EnemyPatternMax = 11;	// 敵行動パターンの最大数

	public Control(){
		player = new Player(200, 300);
		LoadEnemyParamList();
		stageCounter = 0;
		enemyCounter = 0;
	}

	public void Update(){
		RegistEnemy();
		MovePlayer();
		UpdateEnemies();

		frame.Update();
		stageCounter++;
	}

	public void Draw(){
		// キャラクターの描画
		DX.DrawRotaGraphF(
			(int)(field.OriginPosX + player.PosX),
			(int)(field.OriginPosY + player.PosY),
			1.0f, 0.0f, (int)player.Gh, DX.TRUE
		);

		foreach(Enemy e in enemyList){
			e.DrawShot(field.OriginPosX, field.OriginPosY);

			if(e.IsDeath){
				// 死んでいるなら描画をスキップ
				continue;
			}
			DX.DrawRotaGraphF(
				(int)(field.OriginPosX + e.PosX),
				(int)(field.OriginPosY + e.PosY),
				1.0f, 0.0f,(int) e.Gh, DX.TRUE
			);
		}

		frame.Draw();		
	}

	private void RegistEnemy(){
		foreach(EnemyParam ep in enemyParamList){
			if(ep.readyTime == stageCounter){
				Enemy e = new Enemy(ep, enemyCounter);
				enemyList.Add(e);
				enemyCounter++;
			}
		}
	}

	private void MovePlayer(){
		Input input = Input.Instance;

		bool xMoveFlag = false;
		bool yMoveFlag = false;

		double dx = 0;
		double dy = 0;

		player.Count++;
		player.GhIndex = (player.Count % 24) / 6;

		if(input.IsOn(Input.InputButton.LEFT)){
			dx -= player.Speed; 
			xMoveFlag = true;
			player.GhIndex += 8;
		}else if(input.IsOn(Input.InputButton.RIGHT)){
			dx += player.Speed; 
			xMoveFlag = true;
			player.GhIndex += 4;
		}
		if(input.IsOn(Input.InputButton.UP)){
			dy -= player.Speed; 
			yMoveFlag = true;
		}else if(input.IsOn(Input.InputButton.DOWN)){
			dy += player.Speed; 
			yMoveFlag = true;
		}

		if(xMoveFlag & yMoveFlag){
			// 斜め移動
			dx *= 0.71;
			dy *= 0.71;
		}

		if(input.IsOn(Input.InputButton.C)){
			// スロウモード
			dx /= 2;
			dy /= 2;
		}

		// 移動
		player.PosX += dx;
		player.PosY += dy;

		// 移動範囲チェック
		if(player.PosX < (player.Width /2)){
			player.PosX = (player.Width /2);
		}else if(player.PosX > field.Width - (player.Width /2)){
			player.PosX = field.Width - (player.Width /2);
		}

		if(player.PosY < (player.Height /2)){
			player.PosY = (player.Height /2);
		}else if(player.PosY > field.Height - (player.Height /2)){
			player.PosY = field.Height - (player.Height /2);
		}
	}

	private void UpdateEnemies(){
		foreach(Enemy e in enemyList){
			e.UpdateShot(player);

			if(e.IsDeath){
				continue;
			}

			e.Move();
			e.RegistShot();
			e.Count++;
		}

		// とりあえず敵の削除は最後に一括
		enemyList.RemoveAll(e => (e.IsDeath && !e.HaveShots));
	}

	private void LoadEnemyParamList(){
		const string filename = @"res/csv/14章/storyH0.csv";

		Console.WriteLine("Read file : " + filename);

		// StreamReader の新しいインスタンスを生成する
		System.IO.StreamReader cReader = (
			new System.IO.StreamReader(filename, System.Text.Encoding.Default)
		);

		// 区切り文字
		char[] delimiterChars = {','};

		// 最初の2行は項目なので捨てる
		cReader.ReadLine();
		cReader.ReadLine();
		// 読み込みできる文字がなくなるまで繰り返す
		while (cReader.Peek() >= 0) {
			// ファイルを 1 行ずつ読み込む(\n,\rは含まれない)
			string stBuffer = cReader.ReadLine();
			// 読み込んだものを追加で格納する
			string[] enemyParamStr = stBuffer.Split(delimiterChars);
			enemyParamList.Add(new EnemyParam(enemyParamStr));
		}

		// cReader を閉じる (正しくは オブジェクトの破棄を保証する を参照)
		cReader.Close();
	}

} // class Control

} // namespace Shooting