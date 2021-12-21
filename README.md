First run this command in root folder to create Makefile file from .txt template

	cp Makefile.txt Makefile

Then change *BA_GRAPH_PATH = ...* in Makefile to absolute path of ba_graph library on your device. For example
first row will look like this:

    BA_GRAPH_PATH = /Users/erehulka/Documents/Informatika/ZIMA2/RP/ba-graph

Then, to compile, run `make all`. New runnable *c_main.out* will be created. Run it to run all of the tests.