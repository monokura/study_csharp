using System;
using DxLibDLL;

namespace Shooting{
class Field{
	// 大きさ
	private int width;
	private int height;
	// 左上の位置
	private int originPosX;
	private int originPosY;

	public Field(){
		width = 384;
		height = 448;
		originPosX = 32;
		originPosY = 16;
	}

	~Field(){
		// nothing to do
	}

	public int Width{
		get{ return width; }
	}

	public int Height{
		get{ return height; }
	}

	public int OriginPosX{
		get{ return originPosX; }
	}

	public int OriginPosY{
		get{ return originPosY; }
	}
} // class Frame
} // namespace Shooting