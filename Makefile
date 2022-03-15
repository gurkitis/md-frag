compile:
	gcc -o main -g main.c -std=c90
	
tests: compile
	time ./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt worstFit && \
	time ./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt bestFit && \
	time ./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt nextFit && \
	time ./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt firstFit && \
	time ./main -c tests/chunks/bigContent.txt -s tests/sizes/bigContent.txt randomFit && \
	\
	time ./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt worstFit && \
	time ./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt bestFit && \
	time ./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt nextFit && \
	time ./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt firstFit && \
	time ./main -c tests/chunks/max1.txt -s tests/sizes/max1.txt randomFit && \
	\
	time ./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt worstFit && \
	time ./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt bestFit && \
	time ./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt nextFit && \
	time ./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt firstFit && \
	time ./main -c tests/chunks/max2.txt -s tests/sizes/max2.txt randomFit && \
	\
	time ./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt worstFit && \
	time ./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt bestFit && \
	time ./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt nextFit && \
	time ./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt firstFit && \
	time ./main -c tests/chunks/similarSize.txt -s tests/sizes/similarSize.txt randomFit && \
	\
	time ./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt worstFit && \
	time ./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt bestFit && \
	time ./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt nextFit && \
	time ./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt firstFit && \
	time ./main -c tests/chunks/sizesCantFit.txt -s tests/sizes/sizesCantFit.txt randomFit && \
	\
	time ./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt worstFit && \
	time ./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt bestFit && \
	time ./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt nextFit && \
	time ./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt firstFit && \
	time ./main -c tests/chunks/sizesRandom.txt -s tests/sizes/sizesRandom.txt randomFit