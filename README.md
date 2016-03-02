# Succinct de Bruijn test

An educational implementation of Succinct de Bruijn as described in this paper:
http://alexbowe.com/succinct-debruijn-graphs/

Educational = just uses string and vector types instead of succinct bitvectors etc

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
[passed] backward(1) (desired = 7, actual = 7)
[passed] backward(1) (desired = 3, actual = 3)
[passed] backward(1) (desired = 18446744073709551615, actual = 18446744073709551615)
[passed] outdegree(6) (desired = 2, actual = 2)
[passed] forward(8) (desired = 12, actual = 12)
[passed] outgoing(6,T) (desired = 10, actual = 10)
[passed] forward(i) (desired = 10, actual = 10)
[passed] forward(i) (desired = 4, actual = 4)
[passed] forward(i) (desired = 5, actual = 5)
[passed] forward(i) (desired = 8, actual = 8)
[passed] forward(i) (desired = 11, actual = 11)
[passed] forward(i) (desired = 8, actual = 8)
[passed] forward(i) (desired = 9, actual = 9)
[passed] forward(i) (desired = 1, actual = 1)
[passed] forward(i) (desired = 12, actual = 12)
[passed] forward(i) (desired = 1, actual = 1)
[passed] forward(i) (desired = 2, actual = 2)
[passed] forward(i) (desired = 18446744073709551615, actual = 18446744073709551615)
[passed] forward(i) (desired = 6, actual = 6)
[passed] label(0) (desired = $$$, actual = $$$)
[passed] label(1) (desired = CGA, actual = CGA)
[passed] label(2) (desired = $TA, actual = $TA)
[passed] label(3) (desired = GAC, actual = GAC)
[passed] label(4) (desired = TAC, actual = TAC)
[passed] label(5) (desired = GTC, actual = GTC)
[passed] label(6) (desired = ACG, actual = ACG)
[passed] label(7) (desired = TCG, actual = TCG)
[passed] label(8) (desired = $$T, actual = $$T)
[passed] label(9) (desired = ACT, actual = ACT)
[passed] label(10) (desired = CGT, actual = CGT)
[passed] indegree(0) (desired = 0, actual = 0)
[passed] indegree(1) (desired = 2, actual = 2)
[passed] indegree(6) (desired = 2, actual = 2)
[passed] indegree(2) (desired = 1, actual = 1)
```