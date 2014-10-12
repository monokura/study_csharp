using System;
using DxLibDLL;

namespace Shooting{
class InputPad{
	private const int PadMax = 16;
	private int[] state;

	public InputPad(){
		state = new int[PadMax];
		for(int i = 0;i < PadMax;i++){
			state[i] = 0;
		}
	}
	~InputPad(){}

	public void SetInput(int input){
		int mul = 1;
		for(int i = 0;i < PadMax;i++){
			if((input & mul) != 0){
				state[i]++;
			}else{
				state[i] = 0;
			}
			mul *= 2;
		}
	}
	
	public bool IsOn(int index){
		if(index < 0 || PadMax <= index){
			return false;
		}

		if(state[index] > 0){
			return true;
		}else{
			return false;
		}
	}
	
	public bool IsFirst(int index){
		if(index < 0 || PadMax <= index){
			return false;
		}

		if(state[index] == 1){
			return true;
		}else{
			return false;
		}
	}
}
} // namespace Shooting