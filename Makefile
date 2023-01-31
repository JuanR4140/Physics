all:
	gcc main.c sprite.c -o main -lallegro -lallegro_primitives

run:
	gcc main.c sprite.c -o main -lallegro -lallegro_primitives
	./main

clean:
	rm main
