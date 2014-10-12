using System;
using DxLibDLL;

namespace Shooting{
class EnemyParam{
	// excelデータの列と対応
	private enum ParamIndex{
		READY_TIME,
		PATTERN,
		TYPE,
		POS_X,
		POS_Y,
		SPEED,
		SHOT_TIME,
		SHOT_TYPE,
		COLOR,
		HP,
		BULLET_TYPE,
		WAIT_TIME,
		ITEM_0,	ITEM_1,	ITEM_2,	ITEM_3,	ITEM_4,	ITEM_5,
	};

	public int pattern;		//
	public int muki;		//
	public int type;		//
	public int HP;			//
	public int[] item;		//

	// 移動系
	public double posX;		// 
	public double posY;		// 
	public double vx;		// 
	public double vy;		// 
	public double speed;	// 
	public double angle;	// 

	public int shotTime;	// 弾幕開始時間
	public int shotType;	// 弾幕の種類
	public int bulletType;	// 弾の種類
	public int color;		// 色
	public int state;		// 状態
	public int readyTime;	// 待機時間
	public int waitTime;	// 停滞時間

	public EnemyParam(string[] enemyParamStr){
		pattern = int.Parse(enemyParamStr[(int)ParamIndex.PATTERN]);
		muki	= 1;
		type	= int.Parse(enemyParamStr[(int)ParamIndex.TYPE]);
		HP		= int.Parse(enemyParamStr[(int)ParamIndex.HP]);

		posX	= int.Parse(enemyParamStr[(int)ParamIndex.POS_X]);
		posY	= int.Parse(enemyParamStr[(int)ParamIndex.POS_Y]);
		vx 		= 0;
		vy 		= 0;
		speed 	= int.Parse(enemyParamStr[(int)ParamIndex.SPEED]);
		angle 	= 0;

		shotTime	= int.Parse(enemyParamStr[(int)ParamIndex.SHOT_TIME]);
		shotType	= int.Parse(enemyParamStr[(int)ParamIndex.SHOT_TYPE]);
		bulletType  = int.Parse(enemyParamStr[(int)ParamIndex.BULLET_TYPE]);
		color	  	= int.Parse(enemyParamStr[(int)ParamIndex.COLOR]);
		readyTime	= int.Parse(enemyParamStr[(int)ParamIndex.READY_TIME]);
		waitTime	= int.Parse(enemyParamStr[(int)ParamIndex.WAIT_TIME]);

		item = new int[6];
		item[0] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_0]);
		item[1] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_1]);
		item[2] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_2]);
		item[3] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_3]);
		item[4] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_4]);
		item[5] = int.Parse(enemyParamStr[(int)ParamIndex.ITEM_5]);
		
	}

	~EnemyParam(){
		// nothing to do
	}

} // class EnemyParam
} // namespace Shooting