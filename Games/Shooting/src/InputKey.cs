using System;
using DxLibDLL;

namespace Shooting{
class InputKey{
	private const int KeyMax = 256;
	private int[] state;

	public InputKey(){
		state = new int[KeyMax];
		for(int i = 0;i < KeyMax;i++){
			state[i] = 0;
		}
	}

	public void SetInput(Byte[] _key){
		for(int i = 0;i < KeyMax;i++){
			if(_key[i] == 1){
				state[i]++;
			}else{
				state[i] = 0;
			}
		}
	}

	public bool IsOn(int index){
		if(index < 0 || KeyMax <= index){
			return false;
		}

		if(state[index] > 0){
			return true;
		}else{
			return false;
		}
	}

	public bool IsFirst(int index){
		if(index < 0 || KeyMax <= index){
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