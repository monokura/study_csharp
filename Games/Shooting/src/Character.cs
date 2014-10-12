using System;
using DxLibDLL;

namespace Shooting{
class Character{
	protected double width;
	protected double height;
	protected int ghIndex;
	protected int[] gh;
	protected double posX;
	protected double posY;

	public Character(){

	}

	public double PosX{
		get{ return this.posX; }
		set{ this.posX = value; }
	}

	public double PosY{
		get{ return this.posY; }
		set{ this.posY = value; }
	}

	public double Gh{
		get{ return gh[ghIndex]; }
	}

	public int GhIndex{
		get{ return this.ghIndex; }
		set{ this.ghIndex = value;}
	}

	public double Width{
		get{ return this.width; }
		set{ this.width = value;}
	}

	public double Height{
		get{ return this.height; }
		set{ this.height = value;}
	}
}
} // namespace Shooting