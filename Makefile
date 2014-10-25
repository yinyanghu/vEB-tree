all: veb gen naive

test: veb gen naive
	./gen > in
	./naive < in > ans
	./veb < in > out
	diff ans out

veb: veb.c
	gcc veb.c -lm -o veb

gen: gen.c
	gcc gen.c -o gen

naive: naive.c
	gcc naive.c -o naive

clean:
	rm -f gen a.out veb naive
	rm -f in out ans
