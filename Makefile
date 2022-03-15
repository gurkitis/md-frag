compile:
	gcc -o main -g main.c -std=c90
	
tests: compile
	./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt worstFit
	./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt bestFit
	./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt nextFit
	./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt firstFit
	./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt randomFit

	./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt worstFit
	./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt bestFit
	./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt nextFit
	./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt firstFit
	./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt randomFit

	./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt worstFit
	./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt bestFit
	./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt nextFit
	./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt firstFit
	./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt randomFit

	./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt worstFit
	./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt bestFit
	./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt nextFit
	./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt firstFit
	./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt randomFit

	./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt worstFit
	./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt bestFit
	./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt nextFit
	./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt firstFit
	./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt randomFit
	
	./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt worstFit
	./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt bestFit
	./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt nextFit
	./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt firstFit
	./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt randomFit
