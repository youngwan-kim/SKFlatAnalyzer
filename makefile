all : comp

comp:
	@-(source bin/Compile.sh)

clean:
	@-(rm lib/*)
