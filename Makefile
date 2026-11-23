main: $(wildcard *.c include/*.h) Makefile
	gcc *.c -Iinclude -Ivendor/iup/include -pthread \
		-Lvendor/iup -Lvendor/im -Lvendor/cd \
		-liupcontrols -liupcd -lcd -liupim -lim -liup \
		-Wl,--disable-new-dtags,-rpath,'$$ORIGIN/vendor/iup:$$ORIGIN/vendor/im:$$ORIGIN/vendor/cd' -o main
