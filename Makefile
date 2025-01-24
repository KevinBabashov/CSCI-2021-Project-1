CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
AN = proj1
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: test clean clean-tests

spell_check: spell_check.o dictionary.o
	$(CC) -o $@ $^

spell_check.o: spell_check.c
	$(CC) -c $<

dictionary.o: dictionary.c dictionary.h
	$(CC) -c $<

test-setup:
	@chmod u+x testius
	@rm -f test_dictionary.txt test_dictionary_large.txt

ifdef testnum
test: spell_check test-setup
	./testius test_cases/tests.json -v -n "$(testnum)"
else
test: spell_check test-setup
	./testius test_cases/tests.json
endif

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/test_cases/*" "$(CWD)/testius"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

clean:
	rm -f *.o spell_check

clean-tests:
	rm -rf test_results/
	rm -f test_dictionary.txt test_dictionary_large.txt
