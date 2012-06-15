all: pricer
	

debug:
	gcc -o pricer -ggdb pricer.c

prof:
	gcc -o pricer -pg pricer.c

pricer:
	gcc -o pricer -O3 pricer.c

clean:
	[ -e pricer ] && rm pricer || true
