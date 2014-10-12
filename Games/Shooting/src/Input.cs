using System;
using DxLibDLL;

namespace Shooting{
//　singleton
class Input{
	private static Input instance;
	private InputKey inputKey;
	private InputPad inputPad;

	private const int InputMax = 12;
	private int[] configPad;
	private int[] configKey;
	
	public enum InputButton{
		DOWN,LEFT,RIGHT,UP,A,B,C,D,R,L,START,SELECT,NONE,
	};

	private Input(){
		inputKey = new InputKey();
		inputPad = new InputPad();

		// キー割り当ての設定
		// Todo:外部ファイルから読み込むようにする
		configPad = new int[InputMax];
		configKey = new int[InputMax];
		for(int i = 0;i < InputMax;i++){
			configPad[i] = -1;
			configKey[i] = -1;
		}
		configPad[(int)InputButton.DOWN] 	= DX.PAD_INPUT_DOWN;	configKey[(int)InputButton.DOWN] 	= DX.KEY_INPUT_DOWN;
		configPad[(int)InputButton.LEFT] 	= DX.PAD_INPUT_LEFT;	configKey[(int)InputButton.LEFT] 	= DX.KEY_INPUT_LEFT;
		configPad[(int)InputButton.RIGHT] 	= DX.PAD_INPUT_RIGHT;	configKey[(int)InputButton.RIGHT] 	= DX.KEY_INPUT_RIGHT;
		configPad[(int)InputButton.UP] 		= DX.PAD_INPUT_UP;		configKey[(int)InputButton.UP] 		= DX.KEY_INPUT_UP;
		configPad[(int)InputButton.A] 		= DX.PAD_INPUT_1;		configKey[(int)InputButton.A] 		= DX.KEY_INPUT_A;
		configPad[(int)InputButton.B] 		= DX.PAD_INPUT_2;		configKey[(int)InputButton.B] 		= DX.KEY_INPUT_S;
		configPad[(int)InputButton.C] 		= DX.PAD_INPUT_3;		configKey[(int)InputButton.C] 		= DX.KEY_INPUT_D;
		configPad[(int)InputButton.D] 		= DX.PAD_INPUT_4;		configKey[(int)InputButton.D] 		= DX.KEY_INPUT_F;
		configPad[(int)InputButton.START] 	= DX.PAD_INPUT_5;		configKey[(int)InputButton.START] 	= DX.KEY_INPUT_SPACE;
	}

	public static Input Instance{
		get{
			if(instance == null){
				instance = new Input();
			}
			return instance;
		}
	}

	public bool IsOn(InputButton _input){
		bool keyFlag = inputKey.IsOn(configKey[(int)_input]);
		bool padFlag = inputPad.IsOn(configPad[(int)_input]);
		return keyFlag | padFlag;
	}

	public bool IsFirst(InputButton _input){
		bool keyFlag = inputKey.IsFirst(configKey[(int)_input]);
		bool padFlag = inputPad.IsFirst(configPad[(int)_input]);
		return keyFlag | padFlag;
	}
	
	public bool IsKeyOn(int _index){
		return inputKey.IsOn(_index);
	}
	
	public void SetInput(Byte[] _key, int _pad){
		inputKey.SetInput(_key);
		inputPad.SetInput(_pad);
	}
}
} // namespace Shooting