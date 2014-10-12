using System;
using DxLibDLL;

class Program{
	public static void Main(){
		DX.ChangeWindowMode(DX.TRUE);
		if(DX.DxLib_Init() == -1){
			return;
		}

		while(DX.ProcessMessage() == 0){
			if(DX.CheckHitKey(DX.KEY_INPUT_ESCAPE) == 1){
				break;
			}
		}

		DX.DxLib_End();
	}
}