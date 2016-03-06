# Succinct de Bruijn test

An educational implementation of Succinct de Bruijn as described in this paper:
http://alexbowe.com/succinct-debruijn-graphs/

Educational = just uses string and vector types instead of succinct bitvectors etc

Default make target runs the tests

Output
```
[section] simple debruijn test based on the example data in the paper
[passed] forward(2) (desired = 5, actual = 5)
[passed] backward(5) (desired = 2, actual = 2)
[passed] backward(1) (desired = 7, actual = 7)
[passed] backward(1) (desired = 3, actual = 3)
[passed] backward(1) (desired = 18446744073709551615, actual = 18446744073709551615)
[passed] outdegree(6) (desired = 2, actual = 2)
[passed] outdegree(0) (desired = 1, actual = 1)
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
[passed] incoming(6,T) (desired = 4, actual = 4)
[passed] num_nodes (desired = 11, actual = 11)
[section] test succinct against basic debruijn
[section] checking node 0
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 0, actual = 0)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(0,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(0,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(0,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(0,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(0,T)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = $$T, actual = $$T)
[section] incoming(0,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(0,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(0,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(0,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(0,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 1
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 2, actual = 2)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(1,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(1,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(1,C)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = GAC, actual = GAC)
[section] outgoing(1,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(1,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(1,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(1,A)
db_basic.label(incoming_basic) = ACG
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = ACG, actual = ACG)
[section] incoming(1,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(1,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(1,T)
db_basic.label(incoming_basic) = TCG
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = TCG, actual = TCG)
[section] checking node 2
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(2,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(2,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(2,C)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = TAC, actual = TAC)
[section] outgoing(2,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(2,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(2,$)
db_basic.label(incoming_basic) = $$T
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = $$T, actual = $$T)
[section] incoming(2,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(2,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(2,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(2,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 3
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 2, actual = 2)
[section] outgoing(3,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(3,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(3,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(3,G)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = ACG, actual = ACG)
[section] outgoing(3,T)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = ACT, actual = ACT)
[section] incoming(3,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(3,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(3,C)
db_basic.label(incoming_basic) = CGA
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = CGA, actual = CGA)
[section] incoming(3,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(3,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 4
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(4,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(4,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(4,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(4,G)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = ACG, actual = ACG)
[section] outgoing(4,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(4,$)
db_basic.label(incoming_basic) = $TA
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = $TA, actual = $TA)
[section] incoming(4,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(4,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(4,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(4,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 5
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(5,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(5,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(5,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(5,G)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = TCG, actual = TCG)
[section] outgoing(5,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(5,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(5,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(5,C)
db_basic.label(incoming_basic) = CGT
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = CGT, actual = CGT)
[section] incoming(5,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(5,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 6
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 2, actual = 2)
[passed] outdegree (desired = 2, actual = 2)
[section] outgoing(6,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(6,A)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = CGA, actual = CGA)
[section] outgoing(6,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(6,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(6,T)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = CGT, actual = CGT)
[section] incoming(6,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(6,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(6,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(6,G)
db_basic.label(incoming_basic) = GAC
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = GAC, actual = GAC)
[section] incoming(6,T)
db_basic.label(incoming_basic) = TAC
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = TAC, actual = TAC)
[section] checking node 7
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(7,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(7,A)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = CGA, actual = CGA)
[section] outgoing(7,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(7,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(7,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(7,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(7,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(7,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(7,G)
db_basic.label(incoming_basic) = GTC
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = GTC, actual = GTC)
[section] incoming(7,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 8
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(8,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(8,A)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = $TA, actual = $TA)
[section] outgoing(8,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(8,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(8,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(8,$)
db_basic.label(incoming_basic) = $$$
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = $$$, actual = $$$)
[section] incoming(8,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(8,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(8,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(8,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 9
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 0, actual = 0)
[section] outgoing(9,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(9,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(9,C)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(9,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(9,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(9,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(9,A)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(9,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(9,G)
db_basic.label(incoming_basic) = GAC
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = GAC, actual = GAC)
[section] incoming(9,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] checking node 10
[passed] found the label in basic (desired = 1, actual = 1)
[passed] indegree (desired = 1, actual = 1)
[passed] outdegree (desired = 1, actual = 1)
[section] outgoing(10,$)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(10,A)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(10,C)
[passed] outgoing shouldn't return no_node (desired = 1, actual = 1)
[passed] outgoing (desired = GTC, actual = GTC)
[section] outgoing(10,G)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] outgoing(10,T)
[passed] outgoing expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(10,$)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(10,A)
db_basic.label(incoming_basic) = ACG
[passed] incoming shouldn't return no_node (desired = 1, actual = 1)
[passed] incoming (desired = ACG, actual = ACG)
[section] incoming(10,C)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(10,G)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] incoming(10,T)
incoming_basic = 18446744073709551615
[passed] incoming expecting no_node (desired = 18446744073709551615, actual = 18446744073709551615)
[section] rank select tests
[passed] rank(5) (desired = 3, actual = 3)
[passed] select(4) (desired = 7, actual = 7)
```