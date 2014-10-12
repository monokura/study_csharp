using System;
using DxLibDLL;

namespace Shooting{
class Program{
	public static void Main(){
		// 画面モードの変更
		DX.ChangeWindowMode(DX.TRUE);
		// ウインドウの大きさ（default）：640*480

		// バックグラウンドでも動作
		DX.SetAlwaysRunFlag( DX.TRUE );

		if(DX.DxLib_Init() == -1){
			return ;
		}

		DX.SetDrawScreen(DX.DX_SCREEN_BACK);

		Byte[] keyStateBuf = new Byte[256];

		Game game = new Game();

		Input input = Input.Instance;

		while(DX.ProcessMessage() == 0 && DX.ScreenFlip() == 0
			&& DX.ClearDrawScreen() == 0 && DX.GetHitKeyStateAll(out keyStateBuf[0]) == 0){
			int padInput = DX.GetJoypadInputState(DX.DX_INPUT_PAD1);

			input.SetInput(keyStateBuf, padInput);

			game.Update();
			game.Draw();

			if(input.IsKeyOn(DX.KEY_INPUT_ESCAPE)){
				break;
			}
		}

		DX.DxLib_End();
	}
}
}