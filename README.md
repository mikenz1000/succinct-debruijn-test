# Succinct de Bruijn test

An implementation of Succinct de Bruijn as described in this paper:
http://alexbowe.com/succinct-debruijn-graphs/

Implemented in a very simplistic way using string and vector types as an exercise in understanding the algorithm

Output
```
[section] simple debruijn test based on the example data in the paper
$$$ T
AGC C
AT$ C
CAG G
CAG T
CAT G-
CTG G
GCA A
GCA T
GCT A-
T$$ A
TCA $
TGC C
L	W
1	T
1	C
1	C
0	G
1	T
1	G-
1	G
0	A
1	T
1	A-
1	A
1	$
1	C
F[$] = 0
F[A] = 1
F[C] = 3
F[G] = 7
F[T] = 10
[passed] forward(2) (desired = 5, actual = 5)
[passed] backward(5) (desired = 2, actual = 2)
[passed] outdegree(6) (desired = 2, actual = 2)
[passed] forward(8) (desired = 12, actual = 12)
[passed] outgoing(6,T) (desired = 10, actual = 10)
```