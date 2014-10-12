using System;
using DxLibDLL;


namespace Shooting{
class Frame{
	// グラフィックハンドル
	private int gh1;
	private int gh2;
	private int gh3;
	private int gh4;

	public Frame(){
		gh1 = DX.LoadGraph("res/img/board/10.png");
		gh2 = DX.LoadGraph("res/img/board/11.png");
		gh3 = DX.LoadGraph("res/img/board/12.png");
		gh4 = DX.LoadGraph("res/img/board/20.png");
	}
	~Frame(){
		// nothing to do
	}

	public void Update(){
		// nothing to do
	}
	
	public void Draw(){
		DX.DrawGraph(  0,  0, gh1, DX.FALSE);
		DX.DrawGraph(  0, 16, gh2, DX.FALSE);
		DX.DrawGraph(  0,464, gh3, DX.FALSE);
		DX.DrawGraph(416,  0, gh4, DX.FALSE);
	}

} // class Frame
} // namespace Shooting