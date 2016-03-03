performance:
	g++ -std=c++1y -O3 RunPerformance.cpp ../sampletext.cpp KnuthMorrisPratt.cpp BoyerMoore.cpp TableOfPrefixes.cpp TableOfSuffixes.cpp
test:
	g++ -std=c++1y -O3 TestCode.cpp TableOfPrefixes.cpp TableOfSuffixes.cpp BoyerMoore.cpp KnuthMorrisPratt.cpp -lgmock
sample:
	g++ -std=c++1y -O3 RunSample.cpp ../sampletext.cpp KnuthMorrisPratt.cpp BoyerMoore.cpp TableOfPrefixes.cpp TableOfSuffixes.cpp
