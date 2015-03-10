all: test_conf

test_conf:
	g++ test_conf.cc -o test_conf -L../ -lconf -L ../../utilities -lutilities -I../
#	g++ test_conf.cc -o test_conf ../libconf.a -I../

clean:
	rm -rf test_conf
