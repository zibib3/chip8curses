# hello-world-chip8
This will be emulator for chip8 architecture.

In this build, I Can play tetris and TIC-TAC-TOE on the emulator in an comfortable way,
So I decide This is the time to write instruction in the README:


keybinding:
	q:	quit
	ENTER:	pause the game.
	<F5>: 	At pause mode, step one instruction.
	':	resume the game.
	2,3,4,5,
	w,e,r,t,
	s,d,f,g,
	x,c,v,b:	the keyboard of the chip-80.

If the keyboard is not working, please make sure that you are typing lower-case english characters.

install instruction:
	You can run install.sh. It isn't realy installing anything, it's just compiling the code to the current directory.
run instruction:
	just type ./emulator-name rom-path



my next targets are:
1) Now that I know how to use all the required things, I can rewrite parts of the code to make the code more flexiable and more
   robust. - done.
2) I need to make the control in the games more easy. - partially done.
3) write a documentation on the functions. - low priority. currently I'm writing the code mostly for me, and I know my code.
4) Add suport for more instructions
5) add cross-platform support.


note:
	my emulator is look slower than other emulators, because currently I'm doing sleep after any oppcode to mimick the operation speed of the cheap-80.
	Maybe in the future I will change it to make the emulator faster.
