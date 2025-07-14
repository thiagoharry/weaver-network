doc:
	magitex-cweb weaver-network.cweb
	dvipdf weaver-network.dvi
doc_en:
	tex weaver-network_en.cweb
	dvipdf weaver-network_en.dvi
test:
	ctangle weaver-network.cweb
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c src/network.c  -o test_network
	./test_network
test_en:
	ctangle weaver-network_en.cweb
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c src/network.c -o test_network 
	./test_network
test_web:
	ctangle weaver-network.cweb
	emcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -s ASYNCIFY=1 -Wall -O2 tests/test.c   -o docs/test_network.html
test_web_en:
	ctangle weaver-network_en.cweb
	emcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -s ASYNCIFY=1 -Wall -O2 tests/test.c -o docs/test_network.html
clean:
	rm -f *~ *.core *.scn *.dvi *.idx *.log 
