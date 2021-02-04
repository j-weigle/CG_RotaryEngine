rotary: weiglej_p5.c
	gcc -o rotary weiglej_p5.c drawfunctions.c SOIL/SOIL.c SOIL/image_DXT.c SOIL/image_helper.c SOIL/stb_image_aug.c -lglut -lGL -lGLU -lm

run: rotary
	./rotary

.PHONY: clean

clean:
	rm rotary
