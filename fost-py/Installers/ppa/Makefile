build:
	./Boost/build 39 0
	./fost-py/compile release

clean:
	rm -rf ./Boost/1_39_0/build.v2
	rm -rf ./build.tmp
	rm -rf ./dist

install:
	cp -Rv ./dist $DESTDIR

.PHONY: build clean install
