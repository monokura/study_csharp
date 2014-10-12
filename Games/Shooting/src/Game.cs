using System;
using DxLibDLL;

namespace Shooting{

class Game{
	private int windowWitdh;
	private int windowHeight;
	private bool endFlag;
	private Control control;
	
	public Game(){
		control = new Control();
	}
	~Game(){
		// nothing to do
	}
	public void Update(){
		control.Update();
	}
	public void Draw(){
		control.Draw();
	}
	public bool getEndFlag(){
		return true;
	}
} // class Game

} // namespace Shooting